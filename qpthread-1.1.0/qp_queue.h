/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_QUEUE_H
#define __QP_QUEUE_H
/*
 * 
 * Producer/Consumer support
 * 
 */
template <class Container>
class QpQueue: public QpBase {
    public:
#if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x420)
	typedef Container::value_type T;
#else
	typedef typename Container::value_type T;
#endif
    protected:
	Container    q_cont;
	QpMonitor    q_monitor;
	QpSem        q_sema;
	bool 	     q_readysend;
    public:
	QpQueue(unsigned int capacity = 1024, const char *name = NULL):
		QpBase(name), q_sema(capacity, name), q_readysend(true) {}
	bool Send(T &p, const struct timespec *ts = NULL, bool throw_ex = true) {
		if (ts) {
			struct timespec ts_abs;
			reltime_to_abs(ts, &ts_abs);
			return SendAbs(p, &ts_abs, throw_ex);
		} else
			return SendAbs(p, NULL, throw_ex);
	}
	bool Send(T &p, unsigned int msec, bool throw_ex = true) {
		if (msec) {
			struct timespec ts;
			msectime_to_ts(msec, &ts);
			return Send(p, &ts, throw_ex);
		} else
			return SendAbs(p, NULL, throw_ex);
	}
	bool SendAbs(T &p, const struct timespec *ts_abs = NULL, bool throw_ex = true) {
		if (!q_sema.LockAbs(ts_abs, false)) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return false;
		}
		QpSLock lock(&q_monitor);
		q_readysend = true;
		q_cont.push_back(p);
		q_monitor.Signal();
		return true;
	}
	void SendDone() {
		QpSLock lock(&q_monitor);
		q_readysend = false;
		q_monitor.Broadcast();
	}
	bool CanReceive() {
		QpSLock lock(&q_monitor);
		return q_readysend;
	}
	bool Receive(T *retval, const struct timespec *ts = NULL, bool throw_ex = true) {
		if (ts) {
			struct timespec ts_abs;
			reltime_to_abs(ts, &ts_abs);
			return ReceiveAbs(retval, &ts_abs, throw_ex);
		} else
			return ReceiveAbs(retval, NULL, throw_ex);
	}
	bool Receive(T *retval, unsigned int msec, bool throw_ex = true) {
		if (msec) {
			struct timespec ts;
			msectime_to_ts(msec, &ts);
			return Receive(retval, &ts, throw_ex);
		} else
			return ReceiveAbs(retval, NULL, throw_ex);
	}
	bool ReceiveAbs(T *retval, const struct timespec *ts_abs, bool throw_ex = true) {
		QpSLock lock(&q_monitor);
		while (q_cont.size() == 0) {
			if (!q_readysend)
				return false; // return after the q_cont is empty
			if (q_monitor.WaitAbs(ts_abs, false) == false) {
				if (throw_ex && q_readysend)
					throw QpTimeoutException();
				else
					return false;
			}
		}
		*retval = q_cont.front();
		q_cont.pop_front();
		q_sema.Unlock();
		return true;
	}
	bool TryReceive(T *retval) {
		QpSLock lock(&q_monitor);
		if (q_cont.size()) {
			*retval =  q_cont.front();
			q_cont.pop_front();
			q_sema.Unlock();
			return true;
		} else
			return false;
	}
	unsigned int Size() {
		QpSLock lock(&q_monitor);
		return q_cont.size();
	}
};


template <class Container>
class QpStack: public QpBase {
    public:
#if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x420)
	typedef Container::value_type T;
#else
	typedef typename Container::value_type T;
#endif
    protected:
	Container    q_cont;
	QpMonitor    q_monitor;
	QpSem        q_sema;
	bool 	     q_readysend;
    public:
	QpStack(unsigned int capacity = 1024, const char *name = NULL):
		QpBase(name), q_sema(capacity, name), q_readysend(true) {}
	bool Push(T &p, const struct timespec *ts = NULL, bool throw_ex = true) {
		if (ts) {
			struct timespec ts_abs;
			reltime_to_abs(ts, &ts_abs);
			return PushAbs(p, &ts_abs, throw_ex);
		} else
			return PushAbs(p, NULL, throw_ex);
	}
	bool Push(T &p, unsigned int msec, bool throw_ex = true) {
		if (msec) {
			struct timespec ts;
			msectime_to_ts(msec, &ts);
			return Push(p, &ts, throw_ex);
		} else
			return PushAbs(p, NULL, throw_ex);
	}
	bool PushAbs(T &p, const struct timespec *ts_abs = NULL, bool throw_ex = true) {
		if (!q_sema.LockAbs(ts_abs, false)) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return false;
		}
		QpSLock lock(&q_monitor);
		q_readysend = true;
		q_cont.push_back(p);
		q_monitor.Signal();
		return true;
	}
	void PushDone() {
		QpSLock lock(&q_monitor);
		q_readysend = false;
		q_monitor.Broadcast();
	}
	bool CanPop() {
		QpSLock lock(&q_monitor);
		return q_readysend;
	}
	bool Pop(T *retval, const struct timespec *ts = NULL, bool throw_ex = true) {
		if (ts) {
			struct timespec ts_abs;
			reltime_to_abs(ts, &ts_abs);
			return PopAbs(retval, &ts_abs, throw_ex);
		} else
			return PopAbs(retval, NULL, throw_ex);
	}
	bool Pop(T *retval, unsigned int msec, bool throw_ex = true) {
		if (msec) {
			struct timespec ts;
			msectime_to_ts(msec, &ts);
			return Pop(retval, &ts, throw_ex);
		} else
			return PopAbs(retval, NULL, throw_ex);
	}
	bool PopAbs(T *retval, const struct timespec *ts_abs = NULL, bool throw_ex = true) {
		QpSLock lock(&q_monitor);
		while (q_cont.size() == 0) {
			if (!q_readysend)
				return false; // return after the q_cont is empty
			if (q_monitor.WaitAbs(ts_abs, false) == false) {
				if (throw_ex && q_readysend)
					throw QpTimeoutException();
				else
					return false;
			}
		}
		*retval = q_cont.back();
		q_cont.pop_back();
		q_sema.Unlock();
		return true;
	}
	bool TryPop(T *retval) {
		QpSLock lock(&q_monitor);
		if (q_cont.size()) {
			*retval =  q_cont.back();
			q_cont.pop_back();
			q_sema.Unlock();
			return true;
		} else
			return false;
	}
	unsigned int Size() {
		QpSLock lock(&q_monitor);
		return q_cont.size();
	}
};

#endif

