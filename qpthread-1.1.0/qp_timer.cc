/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#include <qpthr/qp.h>

const unsigned int QpTimer::TIMERJOB_MAGIC = 0xAB23AB33;
QpMutex		  QpTimer::t_mutex;
QpCond		  QpTimer::t_timer_cond(&QpTimer::t_mutex);
#if ! (defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x420))
list<QpTimer::TimerJob *>  QpTimer::t_job_list;
QpTimer::QpTimerThread	  *QpTimer::t_timer_thread;
#else
list<TimerJob *>	  QpTimer::t_job_list;
QpTimerThread		  *QpTimer::t_timer_thread;
#endif
unsigned int      QpTimer::t_curr_seq = 0;
QpCond		  QpTimer::t_curr_cond(&QpTimer::t_mutex);
bool		  QpTimer::t_running = true;
unsigned int	  QpTimer::t_seq = 0;
const int	  QpTimer::INFINITE_SHOTS = -1;

QpTimer::QpTimer()
{
	t_running = true;
	if (!(t_timer_thread = new QpTimerThread(this)))
		throw QpOutOfMemoryException();
	t_timer_thread->Start();
}

QpTimer::~QpTimer()
{
	t_mutex.Lock();
	t_running = false;
	t_mutex.Unlock();
	t_timer_cond.Broadcast();
	t_timer_thread->Join();
	delete t_timer_thread;
	t_timer_thread = NULL;
}

static void CheckCapability(char *msg)
{
	if (!QpInit::Capability(QpInit::QP_TIMER))
		throw QpErrorException(QP_ERROR_DISABLED, msg);
}

QpRegItem QpTimer::RegisterAbs(QpRunnable *r, const struct timespec *ts,
		     unsigned int msec, int shots)
{
	struct timespec ts_period;
	
	ts_period.tv_sec = msec / 1000;
	ts_period.tv_nsec = (msec % 1000) * 1000000;
	return RegisterAbs(r, ts, &ts_period, shots);
}

QpRegItem QpTimer::Register(QpRunnable *r, const struct timespec *ts, int shots)
{
	struct timespec ts_abs;
	
	reltime_to_abs(ts, &ts_abs);
	return RegisterAbs(r, &ts_abs, ts, shots);
}

QpRegItem QpTimer::Register(QpRunnable *r, unsigned int msec, int shots)
{
	struct timespec ts;

	msectime_to_ts(msec, &ts);
	return Register(r, &ts, shots);
}

bool QpTimer::__RegisterAbs(TimerJob *tj)
{
	list<TimerJob *>::iterator li;
	bool inserted, reschedule;
	bool was_empty;
	TimerJob *j;

	inserted = reschedule = false;
	was_empty = t_job_list.empty();
	for (li = t_job_list.begin(); li != t_job_list.end(); ++li) {
		j = *li;
		if (j->t_ts.tv_sec > tj->t_ts.tv_sec ||
		    (j->t_ts.tv_sec == tj->t_ts.tv_sec && j->t_ts.tv_nsec > tj->t_ts.tv_nsec)) {
			if (li == t_job_list.begin())
				reschedule = true;
			t_job_list.insert(li, tj);
			inserted = true;
			break;
		}
	}
	if (!inserted) {
		t_job_list.push_back(tj);
	}
	if (reschedule || was_empty)
		return true;
	else
		return false;
}

QpRegItem QpTimer::RegisterAbs(QpRunnable *r, const struct timespec *ts_abs,
			   const struct timespec *ts_rel_period, int shots)
{
	TimerJob *tj;
	unsigned int seq;
	QpRegItem retval;
	
	CheckCapability("QpTimer::Register");
	if (!r || !ts_abs || (shots < 0 && shots != INFINITE_SHOTS))
		throw QpErrorException(QP_ERROR_ARG, "RegisterAbs");
	if (!(tj = new TimerJob))
		throw QpOutOfMemoryException();
	tj->t_job = r;
	tj->t_shots = shots;
	tj->t_ts = *ts_abs;
	tj->t_magic = TIMERJOB_MAGIC;
	t_mutex.Lock();
	if (++t_seq == 0)
		++t_seq;
	seq = tj->t_seq = t_seq;
	t_mutex.Unlock();
	if (ts_rel_period)
		tj->t_ts_period = *ts_rel_period;
	else {
		tj->t_ts_period.tv_sec = 0;
		tj->t_ts_period.tv_nsec = 0;
		tj->t_shots = 1;
	}
	QpSLock lock(&t_mutex);
	if (tj->t_shots || tj->t_shots == INFINITE_SHOTS) {
		if (__RegisterAbs(tj))
			t_timer_cond.Signal();
	}
	retval.unr = seq;
	return retval;
}

void QpTimer::Unregister(QpRegItem arg)
{
	bool reschedule;
	TimerJob *tj;
	list<TimerJob *>::iterator li;
	unsigned int seq = arg.unr;
	
	CheckCapability("QpTimer::Unregister");
	QpSLock lock(&t_mutex);
	while (t_curr_seq == seq)
		t_curr_cond.Wait();
	reschedule = false;
	if (!t_job_list.empty()) {
		if (t_job_list.front()->t_seq == seq)
			reschedule = true;
		for (li = t_job_list.begin(); li != t_job_list.end(); ++li) {
			tj = *li;
			if (tj->t_seq == seq) {
				t_job_list.erase(li);
				delete tj;
				break;
			}
		}
	}
	if (reschedule)
		t_timer_cond.Signal();
}

void QpTimer::QpTimerThread::Main()
{
	struct timespec timeout;
	struct timeval tv;
	TimerJob *tj;
	
	QpSLock lock(&t_timer->t_mutex);
	while (t_timer->t_running) {
		if (!t_timer->t_job_list.empty()) {
			tj = t_timer->t_job_list.front();
			if (tj->t_magic != TIMERJOB_MAGIC)
				do_exit("QPTHR FATAL ERROR: Main: timerjob has bad magic\n");
			QPERR_ERRNO(gettimeofday(&tv, NULL));
			if (tv.tv_sec > tj->t_ts.tv_sec ||
			    (tv.tv_sec == tj->t_ts.tv_sec && tv.tv_usec * 1000 >= tj->t_ts.tv_nsec)) {
				t_timer->t_curr_seq = tj->t_seq;
				t_timer->t_job_list.pop_front();
				t_timer->t_mutex.Unlock();
				tj->t_job->InvokeMain();
				t_timer->t_mutex.Lock();
				if (tj->t_shots == INFINITE_SHOTS || --(tj->t_shots) > 0) {
					reltime_to_abs(&tj->t_ts_period, &tj->t_ts);
					QpTimer::__RegisterAbs(tj);
				} else
					delete tj;
				t_timer->t_curr_seq = 0;
				t_timer->t_curr_cond.Broadcast();
			} else {
				/* copy the timeout as the structure can be removed */
				timeout = tj->t_ts;
				t_timer->t_timer_cond.WaitAbs(&timeout, false);
			}
		} else
			t_timer->t_timer_cond.Wait();
	}
}

QpThread *QpTimer::TimerThread()
{
	CheckCapability("QpTimer::TimerThread");
	return t_timer_thread;
}

