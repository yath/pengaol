/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999, 2000 by kra
 *
 */
#ifndef __QP_THR_H
#define __QP_THR_H

/*
 * 
 * Threads
 * 
 */

class QpRunnable: public QpBase {
    private:
	QpClonedException *r_except;
    public:
	void InvokeMain();
	
	QpRunnable(const char *name = NULL);
	virtual ~QpRunnable();
	virtual void Raise();

	virtual void Main() = 0;
};

typedef void (*signal_func)(int sig_nr, void *arg);
struct reg_sig {
	int 	     r_sig;
	signal_func  r_func;
	void	     *r_func_arg;
	QpRunnable  *r_job;
	unsigned int r_magic;
};

class QpThread: public QpBase {
    private:
	enum THR_STATE { T_INIT, T_RUNNING, T_FINISHED };
    private:
	pthread_t t_id;
	QpMutex	t_mutex;
	QpCond	t_join_cond;
	QpCond	t_sleep_cond;

	THR_STATE t_state;
	bool	t_sleep;
	bool	t_cancel;

	unsigned int t_prio;
	unsigned int t_stack_size;
	void 	     *t_stack_addr;
	
	QpClonedException *t_except;
	QpRunnable  	*t_runnable;

	list<reg_sig *> s_sync_list;
	
	static pthread_t 	t_main_id;
	static QpThread		*t_main_thread;
	static pthread_key_t 	t_curr_key;

	static QpMonitor	t_count_monitor;
	static int		t_created;
	static int		t_started;
	static sigset_t		t_orig_sigmask;
	
	static void *invoke_main(void *arg);
    protected:
	virtual void Main();	/* The Main function */
    private:
	static void WaitForAllThreads();
	void ThrInitFirst(void *stack_addr);
	void ThrInit(unsigned int prio = QP_PRIO_DFL, unsigned int stack_size = 0);
	void __Begin(void *stack_addr);
	void __Finish();
    protected:
	QpThread(unsigned int prio = QP_PRIO_DFL,
		 unsigned int stack_size = 0, const char *name = NULL);
	void __OrigSigmask();
    public:
	QpThread(QpRunnable *r, unsigned int prio = QP_PRIO_DFL,
		unsigned int stack_size = 0, const char *name = NULL);
	virtual ~QpThread();
	
	void Start();
	
	bool Join(unsigned int msec, bool throw_ex = true);
	bool Join(const struct timespec *ts = NULL, bool throw_ex = true);
	bool JoinAbs(const struct timespec *ts = NULL, bool throw_ex = true);

	static bool Sleep(unsigned int msec);
	static bool Sleep(const struct timespec *ts = NULL);
	static bool SleepAbs(const struct timespec *ts = NULL);
	
	void WakeUp();

    // JBR HACK BEGIN
    // There is empty macro Yield in WINBASE.H on Win32. Turn it off!
#ifdef Yield
#undef Yield
#endif
    static void Yield() { sched_yield();}
    // JBR HACK END
    
	void Cancel();
	static void TestCancel();
	
	void Raise();

	bool IsAlive();

	unsigned int SetPriority(unsigned int prio);
	unsigned int GetPriority();
	
	static QpThread *MainThread();
	static QpThread *Current();
	static unsigned long CurrentSeq();
	unsigned long ThreadSeq();
	
    friend class QpInit;
    friend class QpSignal;
    private:
	QpThread(const QpThread&);
	QpThread& operator = (const QpThread&);
};


class QpThreadPool;

class QpPThread: public QpThread {
    private:
	QpThreadPool   *p_pool;
	unsigned int	p_life_time;
	QpRunnable     *p_cur_job;
    public:
	QpPThread(QpThreadPool *pool, unsigned int prio = QP_PRIO_DFL,
		  unsigned int stack_size = 0,
		  unsigned int msec_life_time = 0,
		  const char *name = NULL);
	virtual ~QpPThread() { Join();}
	virtual void Main();
    friend class QpThreadPool;
    private:
	QpPThread(const QpPThread&);
	QpPThread& operator = (const QpPThread&);
};

class QpThreadPool: public QpBase {
    private:
	QpMutex		p_mutex;
	bool		p_running;
	
	deque<QpRunnable *> p_job;
	QpCond 	   	    p_job_cond;
	
	list<QpRunnable *>  p_stat;
	QpCond		    p_stat_cond;

	vector<QpPThread *> p_threads;
	vector<QpPThread *> p_threads_clean;
	QpCond		    p_clean_cond;
	
	unsigned int 	  p_prio;
	unsigned int 	  p_stack_size;
	unsigned int 	  p_max_threads;
	unsigned int 	  p_life_time;
    private:
	void ThreadsClean();
    public:
	QpThreadPool(unsigned int prio = QP_PRIO_DFL,
		  unsigned int stack_size = 0,
		  unsigned int max_threads = 1, 
		  bool on_demand = false,
		  unsigned int life_time = 0,
		  const char *name = NULL);
	~QpThreadPool();
	
	void Do(QpRunnable *j);
	QpRunnable *Wait(QpRunnable *what, unsigned int msec, bool throw_ex = true);
	QpRunnable *Wait(QpRunnable *what = NULL, const struct timespec *ts_rel = NULL, bool throw_ex = true);
	QpRunnable *WaitAbs(QpRunnable *what = NULL, const struct timespec *ts_abs = NULL, bool throw_ex = true);
	
	int CurrentThreads();
	QpRunnable *CurrentJob(unsigned int thr_nr = 0);
    friend class QpPThread;
    private:
	QpThreadPool(const QpThreadPool&);
	QpThreadPool& operator = (const QpThreadPool&);
};

#endif
