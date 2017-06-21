/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_SIG_H
#define __QP_SIG_H

#include <signal.h>

class QpThread;
class QpRunnable;
class QpSignal;

class QpStackReg {
    private:
	struct reg_item {
		void     *r_addr;
		QpThread *r_thread;
	};
	union {
		QpSpinLock	*r_spinlock;
		QpAsyncSafeSem	*r_sem;
	} r_lock;
	vector<struct reg_item> r_table;
    private:
	QpStackReg();
    public:
	~QpStackReg();
    private:
	void Lock();
	void Unlock();
	reg_item Find(void *addr);
	void Print();
	
	void Register(QpThread *t, void *addr);
	void Unregister(QpThread *t, void *addr);
    friend class QpSignal;
    private:
	QpStackReg(const QpStackReg&);
	QpStackReg& operator = (const QpStackReg&);
};

class QpSignal {
    public:
	enum SIG_HANDLER { HANDLER, HANDLER_SYNCIGN, HANDLER_WITHORIG,
			   IGN, DFL, EXIT, ORIG };
	struct sig_request {
		int sig_nr;
	};
	class QpSignalThread: public QpThread {
	    public:
		QpSignal *t_signal;
		QpSignalThread(QpSignal *sig, unsigned int prio = QP_PRIO_DFL,
			       unsigned int stack_size = 0, const char *name = NULL):
			QpThread(prio, stack_size, name), t_signal(sig) {}
		virtual ~QpSignalThread() { Join();}
		virtual void Main();
	    private:
		QpSignalThread(const QpSignalThread&);
		QpSignalThread& operator = (const QpSignalThread&);
	};
    private:
	static const unsigned int REG_SIG_MAGIC;
	static QpStackReg	*s_stack;
	static SIG_HANDLER	s_handler[NSIG + 1];
	static struct sigaction s_orig_h[NSIG + 1];
	static bool		s_orig_h_valid[NSIG + 1];

	static QpMonitor	      s_async_monitor;
	static int		      s_async_process_sig;
	static list<reg_sig *>        s_async_list; /* protected by async_sig_mutex */
	static pthread_t s_sig_id;		    /* signal thread */
	static QpSignalThread  *s_sig_thread;
	static int 	 s_req_pipe[2];
    private:
	static void *async_thread(void *);
	enum ASYNC_THR_CODES {ASYNC_THR_FINISH = -1, ASYNC_THR_EXIT = -2};
	static void async_signal_handler(int sig);
	static void process_async_signal_handler(int sig);
	static void sync_signal_handler(int sig);
	static void exit_signal_handler(int sig);
	static void ThreadSigmask();
	static void RegisterStack(QpThread *t, void *arg)   { s_stack->Register(t, arg);}
	static void UnregisterStack(QpThread *t, void *arg);
	static void EraseSyncID(QpThread *t);
	static void Print();
	static QpRegItem RegisterSignalF(int sig, signal_func func, void *arg, QpRunnable *j);

	static void QpInit();
	static void QpDone();
	QpSignal();
	~QpSignal();
    public:
	static void Exit();
	static void InstallHandler(int sig, SIG_HANDLER handler);
	static void (*OrigHandler(int sig))(int);
	static QpRegItem RegisterSignalFunc(int sig, signal_func func, void *arg = NULL) {
		return RegisterSignalF(sig, func, arg, NULL);
	}
	static QpRegItem RegisterSignalFunc(int sig, QpRunnable *j) {
		return RegisterSignalF(sig, NULL, NULL, j);
	}
	static void UnregisterSignalFunc(QpRegItem);
	static QpThread *SignalThread();
    friend class QpThread;
    friend class QpSignalThread;
    friend class QpInit;
    private:
	QpSignal(const QpSignal&);
	QpSignal& operator = (const QpSignal&);
};

#endif
