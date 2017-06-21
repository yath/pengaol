/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999, 2000 by kra
 *
 */
#if ! (defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x420))
#include <algorithm>
#else
#include <algo.h>
#endif
#include <qpthr/qp.h>

static inline void CheckCapability(char *msg)
{
	if (!QpInit::Initialized()) {
		throw QpErrorException(QP_ERROR_DISABLED, msg);
	}
}

QpRunnable::QpRunnable(const char *name):
	QpBase(name), r_except(NULL)
{
}

QpRunnable::~QpRunnable()
{
	if (r_except)
		delete r_except;
}

void QpRunnable::InvokeMain()
{
	try {
		Main();
	}
	catch (QpClonedException &e) {
		if (r_except)
			delete r_except;
		r_except = e.Clone();
	}
	catch (...) {
		do_exit("Unknown exception in QpRunnable: %s", GetName() ? GetName() : "(null)");
	}
}

void QpRunnable::Raise()
{
	if (r_except)
		r_except->Raise();
}


pthread_t 	QpThread::t_main_id;
QpThread	*QpThread::t_main_thread;
pthread_key_t 	QpThread::t_curr_key;
QpMonitor	QpThread::t_count_monitor;
int		QpThread::t_created = 0;	/* could be less than 0 */
int		QpThread::t_started = 0;
sigset_t	QpThread::t_orig_sigmask;


QpThread::QpThread(unsigned int prio, unsigned int stack_size, const char *name):
	QpBase(name), t_mutex(name), t_join_cond(&t_mutex, name), t_sleep_cond(&t_mutex, name),
	t_state(T_INIT), t_sleep(false), t_cancel(false),
	t_prio(prio), t_stack_size(stack_size), t_stack_addr(NULL),
	t_except(NULL),
	t_runnable(NULL)
{
}

QpThread::QpThread(QpRunnable *r, unsigned int prio,
		   unsigned int stack_size, const char *name):
	QpBase(name), t_mutex(name), t_join_cond(&t_mutex, name), t_sleep_cond(&t_mutex, name),
	t_state(T_INIT), t_sleep(false), t_cancel(false),
	t_prio(prio), t_stack_size(stack_size), t_stack_addr(NULL),
	t_except(NULL),
	t_runnable(r)
{
}

void QpThread::ThrInitFirst(void *stack_addr)
{
	QPERR(pthread_key_create(&t_curr_key, NULL));
#ifndef WIN32
	QPERR(pthread_sigmask(SIG_SETMASK, NULL, &t_orig_sigmask));
#endif
	if (QpInit::Capability(QpInit::QP_SIGNAL)) {
		t_stack_addr = (char *) stack_addr + 128;
		QpSignal::RegisterStack(this, t_stack_addr);
		QpSignal::ThreadSigmask();
	}
	t_id = t_main_id = pthread_self();
	QPERR(pthread_setspecific(t_curr_key, this));
	t_main_thread = this;
}

void QpThread::ThrInit(unsigned int prio, unsigned int stack_size)
{
	pthread_attr_t attr;
	sched_param sch_attr;
	QPERR(pthread_attr_init(&attr));
	if (prio != QP_PRIO_DFL) {
#ifndef WIN32
		QPERR(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
		QPERR(pthread_attr_setschedpolicy(&attr, QpInit::DflSchedPolicy()));
#endif
		QpInit::AdjustSchedParam(prio, &sch_attr);
		QPERR(pthread_attr_setschedparam(&attr, &sch_attr));
	}
#ifndef WIN32
	QPERR(pthread_attr_setscope(&attr, QpInit::DflScope()));
#endif
	QPERR(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
#ifdef WIN32
	if (stack_size > 0) {
#else
	if ((int) stack_size >= sysconf(_SC_THREAD_STACK_MIN)) {
#endif
		QPERR(pthread_attr_setstacksize(&attr, stack_size));
		/*
		 * don't allocate the stack - let the system allocate and mprotect it
		 * in order to detect stack overflow
		 */
	}
	QPERR(pthread_create(&t_id, &attr, &invoke_main, this));
	t_count_monitor.Lock();
	t_created++;
	if (t_created == 0 && t_started == 0)
		t_count_monitor.Signal();
	t_count_monitor.Unlock();
	QPERR(pthread_attr_destroy(&attr));
}

QpThread::~QpThread()
{
	if (pthread_equal(t_id, t_main_id)) {
		if (QpInit::Capability(QpInit::QP_SIGNAL))
			QpSignal::UnregisterStack(this, t_stack_addr);
		QPERR(pthread_setspecific(t_curr_key, NULL));
		t_main_thread = NULL;
		pthread_key_delete(t_curr_key);
	} else {
		QpSLock lock(&t_mutex);
		while (t_state == T_RUNNING)
			t_join_cond.Wait();
	}
	if (t_except) {
		delete t_except;
		t_except = NULL;
	}
}

void *QpThread::invoke_main(void *arg)
{
	QpThread *t = (QpThread *) arg;
	t->__Begin(&t);
	try {
		t->Main();
	}
	catch (QpClonedException &e) {
		if (t->t_except)
			delete t->t_except;
		t->t_except = e.Clone();
	}
	catch (...) {
		do_exit("Unknown exception in %s::Main() (thread_id=%lu)\n",
			t->GetName() ? t->GetName() : "(null)", t->ThreadSeq());
	}
	t->__Finish();
	return NULL;
}

void QpThread::__Begin(void *stack_addr)
{
	t_count_monitor.Lock();
	t_started++;
	t_count_monitor.Unlock();
	if (QpInit::Capability(QpInit::QP_SIGNAL)) {
		t_stack_addr = stack_addr;
		QpSignal::RegisterStack(this, t_stack_addr);
		QpSignal::ThreadSigmask();
	}
	QPERR(pthread_setspecific(t_curr_key, this));
	QPERR(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL));
}

void QpThread::__Finish()
{
	if (QpInit::Capability(QpInit::QP_SIGNAL))
		QpSignal::UnregisterStack(this, t_stack_addr);
	
	QpSLock lock(&t_mutex);
	t_state = T_FINISHED;
	pthread_setspecific(t_curr_key, NULL);
	
	t_count_monitor.Lock();
	t_created--;
	t_started--;
	if (t_created == 0 && t_started == 0)
		t_count_monitor.Signal();
	t_count_monitor.Unlock();
	
	t_join_cond.Broadcast();
}

void QpThread::__OrigSigmask()
{
#ifndef WIN32
	QPERR(pthread_sigmask(SIG_SETMASK, &t_orig_sigmask, NULL));
#endif
}

void QpThread::Main()
{
	if (t_runnable)
		t_runnable->InvokeMain();
}

void QpThread::Start()
{
	CheckCapability("QpThread::Start");
	QpSLock lock(&t_mutex);
	if (t_state != T_INIT) {
		while (t_state != T_FINISHED)
			t_join_cond.Wait();
		/* like in constructor */
		t_cancel = false;
		t_sleep = false;
		if (t_except) {
			delete t_except;
			t_except = NULL;
		}
	}
	ThrInit(t_prio, t_stack_size);
	t_state = T_RUNNING;
}

bool QpThread::Join(unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return Join(&ts, throw_ex);
	} else
		return JoinAbs(NULL, throw_ex);
}

bool QpThread::Join(const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return JoinAbs(&ts_abs, throw_ex);
	} else
		return JoinAbs(NULL, throw_ex);
}

bool QpThread::JoinAbs(const struct timespec *ts, bool throw_ex) {
	QpSLock lock(&t_mutex);
	while (t_state != T_FINISHED) {
		if (t_join_cond.WaitAbs(ts, false) == false) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return false;
		}
	}
	return true;
}

bool QpThread::Sleep(unsigned int msec) 
{
	if (msec) {
		struct timespec ts_rel;
		msectime_to_ts(msec, &ts_rel);
		return Sleep(&ts_rel);
	} else
		return SleepAbs();
}

bool QpThread::Sleep(const struct timespec *ts_rel)
{
	if (ts_rel) {
		struct timespec ts_abs;
		reltime_to_abs(ts_rel, &ts_abs);
		return SleepAbs(&ts_abs);
	} else
		return SleepAbs();
}

bool QpThread::SleepAbs(const struct timespec *ts)
{
	CheckCapability("QpThread::SleepAbs");
	QpThread *curr = Current();
	int retval;

	QpSLock lock(&curr->t_mutex);
	curr->t_sleep = true;
	retval = true;
	while (curr->t_sleep && retval) {
		retval = curr->t_sleep_cond.WaitAbs(ts, false); /* false on timeout */
	}
	if (curr->t_sleep)
		return true;	/* timeout */
	else
		return false;	/* waked up */
}

void QpThread::WakeUp()
{
	CheckCapability("QpThread::WakeUp");
	QpSLock lock(&t_mutex);
	t_sleep = false;
	t_sleep_cond.Signal();
}

void QpThread::Cancel()
{
	CheckCapability("QpThread::Cancel");
	QpSLock lock(&t_mutex);
	t_cancel = true;
}

void QpThread::TestCancel()
{
	CheckCapability("QpThread::TestCancel");
	QpThread *curr = Current();
	
	QpSLock lock(&curr->t_mutex);
	if (curr->t_cancel)
		throw QpCancelException();
}

void QpThread::Raise()
{
	CheckCapability("QpThread::Raise");
	QpSLock lock(&t_mutex);
	if (t_except)
		t_except->Raise();
}

bool QpThread::IsAlive()
{
	CheckCapability("QpThread::IsAlive");
	QpSLock lock(&t_mutex);
	if (t_id == t_main_id)
		return true;
	if (t_state == T_RUNNING)
		return true;
	else
		return false;
}

QpThread *QpThread::Current()
{
	CheckCapability("QpThread::Current");
	return (QpThread *) pthread_getspecific(t_curr_key);
}

unsigned long QpThread::CurrentSeq()
{
#ifdef OSF1
	return pthread_getsequence_np(pthread_self());
#elif WIN32 // JBR
	return reinterpret_cast<unsigned long>(pthread_self());
#else
	return pthread_self();
#endif
}

unsigned long QpThread::ThreadSeq()
{
#ifdef OSF1
	return pthread_getsequence_np(t_id);
#elif WIN32 // JBR
  return reinterpret_cast<unsigned long>(t_id);
#else
	return t_id;
#endif
}

unsigned int QpThread::SetPriority(unsigned int prio)
{
	unsigned int t_prio_old = t_prio;
	sched_param sch_attr;
	int policy;
	
	CheckCapability("QpThread::SetPriority");
	if (prio == QP_PRIO_DFL)
		return t_prio_old;
	if (prio > QP_PRIO_MAX)
		throw QpErrorException(QP_ERROR_ARG, "SetPriority");
	policy = QpInit::DflSchedPolicy();
	QpInit::AdjustSchedParam(prio, &sch_attr);
	QpSLock lock(&t_mutex);
	QPERR(pthread_setschedparam(t_id, policy, &sch_attr));
	t_prio = prio;
	return t_prio_old;
}

unsigned int QpThread::GetPriority()
{
	CheckCapability("QpThread::GetPriority");
	QpSLock lock(&t_mutex);
	return t_prio;
}

QpThread *QpThread::MainThread()
{
	CheckCapability("QpThread::MainThread");
	return t_main_thread;
}

void QpThread::WaitForAllThreads()
{
	QpSLock lock(&t_count_monitor);
	while (t_created != 0 || t_started != 0) 
		t_count_monitor.Wait();
}

/*
 * 
 * ThreadPool
 * 
 */

QpPThread::QpPThread(QpThreadPool *pool, unsigned int prio, unsigned int stack_size,
		     unsigned int msec_life_time, const char *name):
		QpThread(prio, stack_size, name), p_pool(pool),
		p_life_time(msec_life_time), p_cur_job(NULL)
{
}

void QpPThread::Main()
{
	struct timespec ts_rel, ts_abs, *ts_abs_ptr;

	msectime_to_ts(p_life_time, &ts_rel);
	QpSLock lock(&p_pool->p_mutex);
	while (p_pool->p_running) {
		if (p_life_time) {
			reltime_to_abs(&ts_rel, &ts_abs);
			ts_abs_ptr = &ts_abs;
		} else
			ts_abs_ptr = NULL;
		while (p_pool->p_job.empty() && p_pool->p_running)
			if (p_pool->p_job_cond.WaitAbs(ts_abs_ptr, false) == false)
				goto end;
		if (!p_pool->p_running)
			goto end;
		p_cur_job = p_pool->p_job.front();
		p_pool->p_job.pop_front();
	
		p_pool->p_mutex.Unlock();
		p_cur_job->InvokeMain();
		p_pool->p_mutex.Lock();
		
		p_pool->p_stat.push_back(p_cur_job);
		p_cur_job = NULL;
		p_pool->p_stat_cond.Signal();
	}
end:
	p_pool->p_threads.erase(find(p_pool->p_threads.begin(), p_pool->p_threads.end(), this));
	p_pool->p_threads_clean.push_back(this);
	p_pool->p_clean_cond.Signal();
}

QpThreadPool::QpThreadPool(unsigned int prio, unsigned int stack_size,
			   unsigned int max_threads, bool on_demand,
			   unsigned int life_time, const char *name):
	p_mutex(name), p_running(true),
	p_job_cond(&p_mutex, name), p_stat_cond(&p_mutex, name),
	p_clean_cond(&p_mutex, name),
	p_prio(prio), p_stack_size(stack_size),
	p_max_threads(max_threads), p_life_time(life_time)
{
	QpPThread *t;
	int i;
	
	if (!on_demand) {
		for (i = 0; i < (int) max_threads; i++) {
			t = new QpPThread(this, prio, stack_size, life_time, name);
			if (!t)
				throw QpOutOfMemoryException();
			p_mutex.Lock();
			p_threads.push_back(t);
			p_mutex.Unlock();
			
			t->Start();
		}
	}
}

void QpThreadPool::Do(QpRunnable *j)
{
	ThreadsClean();

	vector<QpPThread *>::iterator li;
	unsigned int idle_threads;
	QpPThread *t;

	CheckCapability("QpThreadPool::Do");
	p_mutex.Lock();
	idle_threads = 0;
	
	for (li = p_threads.begin(); li != p_threads.end(); li++)
		if ((*li)->p_cur_job == NULL)
			idle_threads++;
	
	if (p_threads.size() <  p_max_threads && p_job.size() >= idle_threads) {
		p_mutex.Unlock();
		t = new QpPThread(this, p_prio, p_stack_size, p_life_time, GetName());
		if (!t)
			throw QpOutOfMemoryException();
		p_mutex.Lock();
		p_threads.push_back(t);
		p_mutex.Unlock();
		t->Start();
		p_mutex.Lock();
	}
	p_job.push_back(j);
	p_job_cond.Signal();
	p_mutex.Unlock();
}

QpRunnable *QpThreadPool::Wait(QpRunnable *what, unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return Wait(what, &ts, throw_ex);
	} else
		return WaitAbs(what, NULL, throw_ex);
}

QpRunnable *QpThreadPool::Wait(QpRunnable *what, const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return WaitAbs(what, &ts_abs, throw_ex);
	} else
		return WaitAbs(what, NULL, throw_ex);
}

QpRunnable *QpThreadPool::WaitAbs(QpRunnable *what, const struct timespec *ts_abs, bool throw_ex)
{
	CheckCapability("QpThreadPool::WaitAbs");
	ThreadsClean();
	
	QpRunnable *j = NULL;
	{
	list<QpRunnable *>::iterator where;
	QpSLock lock(&p_mutex);

	while (p_stat.empty() ||
	       (what && (where = find(p_stat.begin(), p_stat.end(), what)) == p_stat.end())) {
		if (p_stat_cond.WaitAbs(ts_abs, false) == false) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return NULL;
		}
	}
	if (what) {
		j = *where;
		p_stat.erase(where);
	} else {
		j = p_stat.front();
		p_stat.pop_front();
	}
	} /* lock done */
	ThreadsClean();
	return j;
}


QpThreadPool::~QpThreadPool()
{
	p_mutex.Lock();
	p_running = false;
	p_job_cond.Broadcast();
	while (p_threads.size() || p_threads_clean.size()) {
		while (p_threads.size() > 0 && p_threads_clean.empty())
			p_clean_cond.Wait();
		p_mutex.Unlock();
		ThreadsClean();
		p_mutex.Lock();
	}
	p_mutex.Unlock();
}

void QpThreadPool::ThreadsClean()
{
	vector<QpPThread *>::iterator vi;
	vector<QpPThread *> th_clean;
	
	{
	QpSLock lock(&p_mutex);

	if (p_threads_clean.empty())
		return;
	th_clean = p_threads_clean;
	p_threads_clean.erase(p_threads_clean.begin(), p_threads_clean.end());
	} /* unlock */
	
	for (vi = th_clean.begin(); vi != th_clean.end(); vi++) {
		(*vi)->Join();
		delete (*vi);
	}
	th_clean.erase(th_clean.begin(), th_clean.end());
}

QpRunnable *QpThreadPool::CurrentJob(unsigned int thr_nr)
{
	CheckCapability("QpThreadPool::CurrentJob");
	QpSLock lock(&p_mutex);
	if (thr_nr < p_threads.size()) {
		return p_threads[thr_nr]->p_cur_job;
	} else
		return NULL;
}

int QpThreadPool::CurrentThreads()
{
	CheckCapability("QpThreadPool::CurrentThreads");
	QpSLock lock(&p_mutex);
	return p_threads.size();
}

