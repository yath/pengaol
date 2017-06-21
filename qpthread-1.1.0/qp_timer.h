/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_TIMER_H
#define __QP_TIMER_H

/*
 * 
 * QpTimer doesn't have Signal support currently
 * 
 */

class QpTimer {
    private:
	class QpTimerThread: public QpThread {
	    public:
		QpTimer *t_timer;
		QpTimerThread(QpTimer *timer, unsigned int prio = QP_PRIO_DFL,
			      unsigned int stack_size = 0, const char *name = NULL): 
			QpThread(prio, stack_size, name), t_timer(timer) {}
		virtual ~QpTimerThread() { Join();}
		virtual void Main();
	    private:
		QpTimerThread(const QpTimerThread&);
		QpTimerThread& operator = (const QpTimerThread&);
	};
	static const unsigned int TIMERJOB_MAGIC;
	class TimerJob {
	    public:
		unsigned int	t_seq;
		QpRunnable 	*t_job;
		int	   	t_shots;
		struct timespec t_ts;
		struct timespec t_ts_period;
		unsigned int	t_magic;
	};
	static QpMutex		t_mutex;
	static QpCond		t_timer_cond;
	static bool 		t_running;
	static list<TimerJob *> t_job_list;
	static QpTimerThread 	*t_timer_thread;
	static unsigned int	t_curr_seq;
	static QpCond		t_curr_cond;
	static unsigned int	t_seq;
    private:
	static bool __RegisterAbs(TimerJob *tj);
	QpTimer();
	~QpTimer();
    public:
	static const int INFINITE_SHOTS;
	static QpRegItem Register(QpRunnable *r, const struct timespec *ts, int shots = 1);
	static QpRegItem Register(QpRunnable *r, unsigned int msec, int shots = 1);
	static QpRegItem RegisterAbs(QpRunnable *r, const struct timespec *ts_abs,
				 const struct timespec *ts_rel_period = NULL, int shots = 1);
	static QpRegItem RegisterAbs(QpRunnable *r, const struct timespec *ts_abs,
				 unsigned int msec_period, int shots);
	static void Unregister(QpRegItem arg);
	static QpThread *TimerThread();
    friend class QpTimerThread;
    friend class QpInit;
    private:
	QpTimer(const QpTimer&);
	QpTimer& operator = (const QpTimer&);
};

#endif
