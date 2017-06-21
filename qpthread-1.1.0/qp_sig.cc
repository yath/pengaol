/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#include <signal.h>
#include <qpthr/qp.h>

int sync_signals[] = {
	SIGILL,
#ifndef WIN32  // JBR
	SIGIOT,
#endif
	SIGABRT,
#if !defined( LINUX ) && !defined( WIN32 ) // JBR
	SIGEMT,
#endif
	SIGFPE,
	SIGSEGV,
#ifndef WIN32  // JBR
	SIGBUS,
#endif
#if !defined( LINUX ) && !defined( WIN32 ) // JBR
	SIGSYS,
#endif
#ifndef WIN32
	SIGPIPE
#endif
};

int sync_signals_size = sizeof(sync_signals) / sizeof(sync_signals[0]);

#if 0
static void sigsyncaddset(sigset_t *set)
{
	int *s;
	
	for (s = sync_signals; s < sync_signals + sync_signals_size; s++)
		sigaddset(set, *s);
}
#endif

static int sigissync(int sig)
{
	int *s;
	
	for (s = sync_signals; s < sync_signals + sync_signals_size; s++)
		if (*s == sig)
			return 1;
	return 0;
}

#if 0
static void sigsyncdelset(sigset_t *set)
{
	int *s;
	
	for (s = sync_signals; s < sync_signals + sync_signals_size; s++)
		sigdelset(set, *s);
}
#endif

/*
 * 
 * QpStackReg
 * 
 */
QpStackReg::QpStackReg()
{
#ifdef ARCH_TAS
	r_lock.r_spinlock = new QpSpinLock;
#else
	r_lock.r_sem = new QpAsyncSafeSem;
#endif
}

QpStackReg::~QpStackReg()
{
#ifdef ARCH_TAS
	delete r_lock.r_spinlock;
#else
	delete r_lock.r_sem;
#endif
}

void QpStackReg::Lock()
{
#ifdef ARCH_TAS
	r_lock.r_spinlock->Lock();
#else
	r_lock.r_sem->Lock();
#endif
}

void QpStackReg::Unlock()
{
#ifdef ARCH_TAS
	r_lock.r_spinlock->Unlock();
#else
	r_lock.r_sem->Unlock();
#endif
}


#if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x420)
reg_item QpStackReg::Find(void *addr)
#else
QpStackReg::reg_item QpStackReg::Find(void *addr)
#endif
{
	vector<struct reg_item>::iterator vi;
	struct reg_item retval;
	
	Lock();
	for (vi = r_table.begin(); vi != r_table.end(); vi++)
		if (addr <= (*vi).r_addr) {
			retval = *vi;
			Unlock();
			return retval;
		}
	Unlock();
	retval.r_addr = NULL;
	retval.r_thread = NULL;
	return retval;
}

void QpStackReg::Register(QpThread *t, void *addr)
{
	struct reg_item r;
	vector<struct reg_item>::iterator vi;

	Lock();
	for (vi = r_table.begin(); vi != r_table.end(); vi++)
		if (addr <= (*vi).r_addr)
			break;
	r.r_addr = addr;
	r.r_thread = t;
	r_table.insert(vi, r);
	Unlock();
}

void QpStackReg::Unregister(QpThread *t, void *addr)
{
	vector<struct reg_item>::iterator vi;
	
	Lock();
	for (vi = r_table.begin(); vi != r_table.end(); vi++) {
		if (addr <= (*vi).r_addr) {
			r_table.erase(vi);
			break;
		} 
	}
	Unlock();
}

void QpStackReg::Print()
{
	vector<struct reg_item>::iterator vi;
	
	for (vi = r_table.begin(); vi != r_table.end(); vi++)
		cout << (hex) << (*vi).r_addr << " " << (*vi).r_thread->ThreadSeq() << endl;
}
/*
 * 
 * signal handlers
 * 
 */
QpStackReg 		*QpSignal::s_stack;
QpSignal::SIG_HANDLER   QpSignal::s_handler[NSIG + 1];
struct sigaction	QpSignal::s_orig_h[NSIG + 1];
bool			QpSignal::s_orig_h_valid[NSIG + 1];
QpMonitor	 	QpSignal::s_async_monitor;
list<reg_sig *>  	QpSignal::s_async_list;
int			QpSignal::s_async_process_sig = -1;
pthread_t    		QpSignal::s_sig_id;
const unsigned int 	QpSignal::REG_SIG_MAGIC = 0xAADD4423;
QpSignal::QpSignalThread *QpSignal::s_sig_thread;
int			QpSignal::s_req_pipe[2];

static void CheckCapability(char *msg)
{
	if (!QpInit::Capability(QpInit::QP_SIGNAL))
		throw QpErrorException(QP_ERROR_DISABLED, msg);
}

#ifndef WIN32
void QpSignal::QpInit()
{
	if (!(s_stack = new QpStackReg))
		throw QpOutOfMemoryException();
}

void QpSignal::QpDone()
{
	delete s_stack;
	s_stack = NULL;
}

QpSignal::QpSignal()
{
	struct sigaction sa;
	int *s;
	int i;

	for (i = 0; i <= NSIG; i++)
		s_orig_h_valid[i] = false;
	
	for (s = sync_signals; s < sync_signals + sync_signals_size; s++) {
		QPERR_ERRNO(sigaction(*s, NULL, &sa));
		if (sa.sa_handler != SIG_IGN) {
			InstallHandler(*s, EXIT);
		}
	}
	QPERR_ERRNO(pipe(s_req_pipe));
	if (!(s_sig_thread = new QpSignalThread(this)))
		throw QpOutOfMemoryException();
	s_sig_thread->Start();
}

QpSignal::~QpSignal()
{
	async_signal_handler(ASYNC_THR_FINISH);
	s_sig_thread->Join();
	delete s_sig_thread;
	s_sig_thread = NULL;
}

void QpSignal::Exit()
{
	QpStackReg::reg_item sr;
	
	CheckCapability("QpSignal::Exit");
	sr = s_stack->Find(&sr);
	if (sr.r_thread == QpSignal::SignalThread()) {
		// the signal was received by signal thread,
		// we can not do _exit() and then continue with
		// the read and exit in the loop, so we do exit() and _exit() here
		async_signal_handler(ASYNC_THR_EXIT);
		return;
#if 0
#ifdef LINUX
		exit(1); /* Linux has some thread cleaning on on_exit */
#endif
	 	_exit(1);
#endif
	}
	async_signal_handler(ASYNC_THR_EXIT);
	_exit(0);
}

void QpSignal::async_signal_handler(int sig)
{
	struct sig_request req;
	int retval;
	
#ifdef QP_DEBUG
	do_msg("async_signal_handler: signal %d\n", sig);
#endif
	req.sig_nr = sig;
again:
	retval = write(QpSignal::s_req_pipe[1], &req, sizeof(struct sig_request));
	if (retval < 0 && errno == EINTR)
		goto again;
	if (retval < 0 || retval != sizeof(struct sig_request))
		do_msg("QPTHR error: signal %s lost\n", sig);
}

void QpSignal::process_async_signal_handler(int sig)
{
	list<struct reg_sig *>::iterator li;
	vector<struct reg_sig *>::iterator vi;
	vector<struct reg_sig *> h_sched;
	struct reg_sig *r;

#ifdef QP_DEBUG
	do_msg("process_async_signal_handler: signal %d delivered to %lu\n", sig, QpThread::CurrentSeq());
#endif
    { /* lock */
	QpSLock lock(&s_async_monitor);
	
	for (li = s_async_list.begin(); li != s_async_list.end(); ++li) {
		r = *li;
		if (r->r_magic != REG_SIG_MAGIC)
			do_exit("QPTHR FATAL ERROR: reg_sig bad magic in async_signal_handler\n");
		if (r->r_sig == sig)
			h_sched.push_back(r);
	}
	s_async_process_sig = sig;
    } /* unlock */
	for (vi = h_sched.begin(); vi != h_sched.end(); ++vi) {
		r = *vi;
		if (r->r_func) {
			try {
				(*r->r_func)(sig, r->r_func_arg);
			}
			catch (...) {
				do_exit("QPTHR FATAL ERROR: async_sig_handler: Unknown exception in func");
			}
		} else
			r->r_job->InvokeMain();
	}
    { /* lock */
	QpSLock lock(&s_async_monitor);
	h_sched.erase(h_sched.begin(), h_sched.end());
	s_async_process_sig = -1;
    } /* unlock */
	s_async_monitor.Broadcast();
	
	if (s_handler[sig] == HANDLER_WITHORIG && s_orig_h_valid[sig]) {
		if (s_orig_h[sig].sa_handler != SIG_IGN &&
		    s_orig_h[sig].sa_handler != SIG_DFL) {
			s_orig_h[sig].sa_handler(sig);
		}
	}
}

void QpSignal::sync_signal_handler(int sig)
{
	QpStackReg::reg_item sr;
	QpThread *t;
	struct reg_sig *r;
	list <struct reg_sig *>::iterator li;
	int found;
	
	/*
	 * It isn't necessary to lock operation with synch handlers
	 * because it could be done only by synch handler own thread
	 * and it is protected by sigmasking
	 */
#ifdef QP_DEBUG
	do_msg("sync_signal_handler: signal %d delivered to %lu pid %lu\n", sig, QpThread::CurrentSeq(), getpid());
#endif
	sr = s_stack->Find(&sr);
	t = sr.r_thread;
	found = 0;
	for (li = t->s_sync_list.begin(); li != t->s_sync_list.end(); ++li) {
		r = *li;
		if (r->r_magic != REG_SIG_MAGIC)
			do_exit("QPTHR FATAL ERROR: reg_sig bad magic in sync_signal_handler\n");
		if (r->r_sig == sig) {
			found = 1;
			if (r->r_func) {
				try {
					(*r->r_func)(sig, r->r_func_arg);
				}
				catch (...) {
					do_exit("QPTHR FATAL ERROR: sync_sig_handler: Unknown exception in func");
				}
			} else
				r->r_job->InvokeMain();
		}
	}
	if (s_handler[sig] == HANDLER && !found) {
		do_exit("signal %d is not handled in thread %lu\n", sig, t->ThreadSeq());
	}
}

void QpSignal::exit_signal_handler(int sig)
{
#ifdef QP_DEBUG
	do_exit("exit_signal_handler: signal %d received by thread %lu, pid %d\n", sig, pthread_self(), getpid());
#else
	do_exit(NULL);
#endif
}

void QpSignal::EraseSyncID(QpThread *t)
{
	struct reg_sig *r;
	list<reg_sig *>::iterator li;

	for (li = t->s_sync_list.begin(); li != t->s_sync_list.end(); ++li) {
		r = *li;
		if (r->r_magic != REG_SIG_MAGIC)
			do_exit("QPTHR FATAL ERROR: reg_sig bad magic in EraseSyncID\n");
		r->r_magic = 0;
		delete r;
	}
	t->s_sync_list.erase(t->s_sync_list.begin(), t->s_sync_list.end());
}

void QpSignal::ThreadSigmask()
{
/*
 * OK, Linux doesn't support delivery of async signals to signal
 * thread if other threads have it blocked
 * 
 * On Solaris the ctrl-c (SIGINT) is not properly delivered
 * to the signal thread if other threads have it blocked.
 */
#if 0
	sigset_t set;
	
	sigfillset(&set);
	sigsyncdelset(&set);
	QPERR(pthread_sigmask(SIG_BLOCK, &set, NULL));
#endif
}

void QpSignal::UnregisterStack(QpThread *t, void *arg)
{
	EraseSyncID(t);
	s_stack->Unregister(t, arg);
}

void (*QpSignal::OrigHandler(int sig))(int)
{
	CheckCapability("QpSignal::OrigHandler");
	if (s_orig_h_valid[sig])
		return s_orig_h[sig].sa_handler;
	else {
		throw QpErrorException(QP_ERROR_STATE, "OrigHandler");
	}
}

void QpSignal::InstallHandler(int sig, SIG_HANDLER sh)
{
	struct sigaction sa, *sa_act, *sa_old;
	void (*func)(int);

	CheckCapability("QpSignal::InstallHandler");
	if (sig < 1 || sig > NSIG)
		throw QpErrorException(QP_ERROR_ARG, "InstallHandler");
	sa_act = sa_old = NULL;
	func = NULL;
	switch (sh) {
	    case HANDLER_WITHORIG:
	    case HANDLER:
	    case HANDLER_SYNCIGN:
		if (sigissync(sig))
			func = sync_signal_handler;
		else
			func = async_signal_handler;
		break;
	    case IGN:
		func = SIG_IGN;
		break;
	    case DFL:
		if (sigissync(sig))
			func = exit_signal_handler;
		else
			func = SIG_DFL;
		break;
	    case EXIT:
		func = exit_signal_handler;
		break;
	    case ORIG:
		if (s_orig_h_valid[sig])
			sa_act = &s_orig_h[sig];
		else
			throw QpErrorException(QP_ERROR_STATE, "InstallHandler");
		break;
	    default:
		throw QpErrorException(QP_ERROR_ARG, "InstallHandler");
	}
	s_handler[sig] = sh;
	if (func) {
		sa.sa_handler = func;
		sigfillset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sa_act = &sa;
	}
	if (!s_orig_h_valid[sig])
		sa_old = &s_orig_h[sig];

	QPERR_ERRNO(sigaction(sig, sa_act, sa_old));
	if (sa_old)
		s_orig_h_valid[sig] = true;
}

QpRegItem QpSignal::RegisterSignalF(int sig, signal_func func, void *arg, QpRunnable *j)
{
	struct reg_sig *r;
	QpRegItem retval;

	CheckCapability("QpSignal::RegisterSignalFunc");
	if (sig < 1 || sig > NSIG || (func == NULL && j == NULL))
		throw QpErrorException(QP_ERROR_ARG, "RegisterSignalFunc");
	if (!(r = new reg_sig))
		throw QpOutOfMemoryException();
	r->r_sig = sig;
	r->r_func = func;
	r->r_func_arg = arg;
	r->r_job = j;
	r->r_magic = REG_SIG_MAGIC;
	if (sigissync(sig)) {
		sigset_t block_set, old_set;
		/*
		 * It isn't necessary to lock operation with synch handlers
		 * because it could be done only by synch handler own thread
		 * and it is protected by sigmasking
		 */
		sigfillset(&block_set);
		pthread_sigmask(SIG_BLOCK, &block_set, &old_set);
		QpThread::Current()->s_sync_list.push_back(r);
		pthread_sigmask(SIG_SETMASK, &old_set, NULL);
	} else {
		QpSLock lock(&s_async_monitor);
		s_async_list.push_back(r);
	}
	retval.ptr = r;
	return retval;
}

void QpSignal::UnregisterSignalFunc(QpRegItem arg) 
{
	struct reg_sig *r = (struct reg_sig *) arg.ptr;
	list<struct reg_sig *>::iterator li;
	int found = 0;

	CheckCapability("QpSignal::UnregisterSignalFunc");
	if (!r || r->r_magic != REG_SIG_MAGIC)
		throw QpErrorException(QP_ERROR_ARG, "UnregisterSignalFunc");
	if (sigissync(r->r_sig)) {
		sigset_t block_set, old_set;
		QpThread *t = QpThread::Current();
		/*
		 * It isn't necessary to lock operation with synch handlers
		 * because it could be done only by synch handler own thread
		 * and it is protected by sigmasking
		 */
		sigfillset(&block_set);
		pthread_sigmask(SIG_BLOCK, &block_set, &old_set);
		for (li = t->s_sync_list.begin(); li != t->s_sync_list.end(); li++)
			if (*li == r) {
				t->s_sync_list.erase(li);
				found = 1;
				break;
			}
		pthread_sigmask(SIG_SETMASK, &old_set, NULL);
	} else {
		QpSLock lock(&s_async_monitor);
		while (r->r_sig == s_async_process_sig)
			s_async_monitor.Wait();
		for (li = s_async_list.begin(); li != s_async_list.end(); li++)
			if (*li == r) {
				s_async_list.erase(li);
				found = 1;
				break;
			}
	}
	if (found) {
		r->r_magic = 0;
		delete r;
	} else
		throw QpErrorException(QP_ERROR_ARG, "UnregisterSignalFunc");
}

void QpSignal::QpSignalThread::Main()
{
#ifdef QP_DEBUG
	cout << "signal thread " << QpThread::CurrentSeq() << " pid " << getpid() << endl;
#endif
	/* do nothing with signal mask as it is inherited from parent proces */
	
	struct sig_request req;
	int retval;
	
	__OrigSigmask();
	while (1) {
		retval = read(t_signal->s_req_pipe[0], &req, sizeof(struct sig_request));
		if (retval == sizeof(struct sig_request)) {
			if (req.sig_nr == ASYNC_THR_FINISH)
				break;
			if (req.sig_nr == ASYNC_THR_EXIT) {
				exit(1);
				_exit(1);
			}
			process_async_signal_handler(req.sig_nr);
		} else if (retval > 0) {
			do_msg("signal thread: bad request or error\n");
		} else if (retval == 0) {
			break;
		} else if (retval < 0 && errno == EINTR) {
			continue;
		} else { /* retval < 0 */
			do_msg("QPTHR FATAL ERROR: in signal thread\n");
			exit(1);
		}
	}
}

QpThread *QpSignal::SignalThread()
{
	CheckCapability("QpSignal::SignalThread");
	return s_sig_thread;
}

#else
// WIN32
void QpSignal::QpInit()
{
}

void QpSignal::QpDone()
{
}

QpSignal::QpSignal()
{
	throw QpUnsupportedException();
}

QpSignal::~QpSignal()
{
}

void QpSignal::ThreadSigmask()
{
	throw QpUnsupportedException();
}

void QpSignal::UnregisterStack(QpThread *t, void *arg)
{
	throw QpUnsupportedException();
}

void QpSignal::Exit()
{
	throw QpUnsupportedException();
}

void QpSignal::InstallHandler(int sig, SIG_HANDLER handler)
{
	throw QpUnsupportedException();
}

QpRegItem QpSignal::RegisterSignalF(int sig, signal_func func, void *arg, QpRunnable *j)
{
	throw QpUnsupportedException();
}

void QpSignal::UnregisterSignalFunc(QpRegItem)
{
	throw QpUnsupportedException();
}

QpThread *QpSignal::SignalThread()
{
	return NULL;
}

#endif
