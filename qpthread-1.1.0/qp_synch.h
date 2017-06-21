/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_SYNCH_H
#define __QP_SYNCH_H

#ifdef WIN32 // disable some warnings on Win32 (VC++ 6.0)
/*
 * Disable following warnings:
 *          
 * warning C4355: 'this' : used in base member initializer list
 */
#pragma warning( disable : 4355 )
#endif // WIN32

#include <qpthr/qp_spinlock.h>

/*
 * 
 * Synchronization Primitives
 * 
 */

class QpCondBase: public QpBase {
    public:
	QpCondBase(const char *name): QpBase(name) {}
	virtual ~QpCondBase() {}
	virtual bool WaitAbs(const struct timespec *ts, bool throw_ex = true) = 0;
	virtual void Signal() = 0;
	virtual void Broadcast() = 0;
	
	virtual void Wait() { WaitAbs(NULL);}
	virtual bool Wait(const struct timespec *ts, bool throw_ex = true);
	virtual bool Wait(unsigned int msec, bool throw_ex = true);
};

class QpLockBase: public QpBase {
    public:
	QpLockBase(const char *name): QpBase(name) {}
	virtual ~QpLockBase() {}
	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	virtual bool TryLock() = 0;
	virtual QpCondBase *CondFactory() = 0;
};

class QpLockExBase: public QpLockBase {
    public:
	QpLockExBase(const char *name): QpLockBase(name) {}
	virtual ~QpLockExBase() {}
	
	virtual bool LockAbs(const struct timespec *ts, bool throw_ex = true) = 0;
	
	virtual void Lock() { LockAbs(NULL);}
	virtual bool Lock(const struct timespec *ts, bool throw_ex = true);
	virtual bool Lock(unsigned int msec, bool throw_ex = true);
};

/*
 * Universal condition
 */
class QpCondUni: public QpCondBase {
    protected:
	QpLockBase 	*c_lock;
	pthread_mutex_t c_mutex;
	pthread_cond_t  c_cond;
    public:
	QpCondUni(QpLockBase *l, const char *name = NULL);
	QpCondUni(QpLockBase& l, const char *name = NULL);
	virtual ~QpCondUni();
	virtual bool WaitAbs(const struct timespec *ts, bool throw_ex = true);
	virtual void Signal();
	virtual void Broadcast();
    private:
	QpCondUni(const QpCondUni&);
	QpCondUni &operator = (const QpCondUni&);
};

/*
 * Async Safe Semaphore
 * 
 * Async Safe Semaphore provides Asynchronous Signal Safe
 * semaphore. (every QpAsyncSafeSem allocates two file descriptors
 * and is slow so use it only when you need async feature)
 * returned QpCond is not async signal safe
 */
class QpAsyncSafeSem: public QpLockBase {
    private:
	int fd[2];
    public:
	QpAsyncSafeSem(const char *name = NULL);
	virtual ~QpAsyncSafeSem();
	
	virtual void Lock();
	virtual void Unlock();
	virtual bool TryLock();
	virtual QpCondBase *CondFactory();
    private:
	QpAsyncSafeSem(const QpAsyncSafeSem&);
	QpAsyncSafeSem& operator = (const QpAsyncSafeSem&);
};

/*
 * SpinLock
 * 
 * SpinLock is supported only on HW platforms that have test and set
 * instruction implemented within QpThread (see documentation
 * which platforms support QpSpinLock). Use QpSpinLock only when
 * the benefits of QpSpinLock outweights the portability issue
 */
class QpSpinLock: public QpLockBase {
    private:
	spinlock_t sp_lock;
	bool 	   sp_yield;
    public:
	QpSpinLock(bool yield = true, const char *name = NULL);
	
	virtual void Lock();
	virtual void Unlock();
	virtual bool TryLock();
	virtual QpCondBase *CondFactory();
    private:
	QpSpinLock(const QpSpinLock&);
	QpSpinLock& operator = (const QpSpinLock&);
};

/*
 * Mutex
 * 
 * This is the mutex you will probably use mostly
 */
class QpMutex: public QpLockBase {
    protected:
	pthread_mutex_t m_mutex;
    public:
	QpMutex(const char *name = NULL);
	virtual ~QpMutex();
	virtual void Lock();
	virtual void Unlock();
	virtual bool TryLock();
	virtual QpCondBase *CondFactory();
    friend class QpMutexCond;
    private:
	QpMutex(const QpMutex&);
	QpMutex& operator = (const QpMutex&);
};

class QpMutexCond: public QpCondBase {
    protected:
	QpMutex	       *c_mutex;
	pthread_cond_t c_cond;
    public:
	QpMutexCond(QpMutex *m, const char *name = NULL);
	QpMutexCond(QpMutex& m, const char *name = NULL);
	virtual bool WaitAbs(const struct timespec *ts, bool throw_ex = true);
	virtual void Signal();
	virtual void Broadcast();
    friend class QpMutex;
    private:
	QpMutexCond(const QpMutexCond&);
	QpMutexCond& operator = (const QpMutexCond&);
};

/*
 * Mutex with timeout
 */
class QpThread;

class QpMutexEx: public QpLockExBase {
    protected:
	int 		m_count;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
	QpThread	*m_owner;
	bool		m_recursive;
    public:
	QpMutexEx(const char *name = NULL);
	QpMutexEx(bool recursive, const char *name = NULL);
	virtual ~QpMutexEx();
	virtual bool LockAbs(const struct timespec *ts, bool throw_ex = true);
	virtual void Unlock();
	virtual bool TryLock();
	virtual QpThread *Owner();
	virtual QpCondBase *CondFactory();
    private:
	QpMutexEx(const QpMutexEx&);
	QpMutexEx& operator = (const QpMutexEx&);
};


class QpCond: public QpCondBase {
    protected:
	QpCondBase *c_cond;
    public:
	QpCond(QpLockBase *s, const char *name = NULL);
	QpCond(QpLockBase &s, const char *name = NULL);
	virtual ~QpCond() { if (c_cond) delete c_cond;}
	virtual bool WaitAbs(const struct timespec *ts, bool throw_ex = true) { 
		return c_cond->WaitAbs(ts, throw_ex);
	}
	virtual void Signal()    { c_cond->Signal();}
	virtual void Broadcast() { c_cond->Broadcast();}
    private:
	QpCond(const QpCond&);
	QpCond& operator = (const QpCond&);
};


/*
 * Synchronization Lock
 */
class QpSLock {
    private:
	QpLockBase *l_lock;
    public:
	inline QpSLock(QpLockBase *s): l_lock(s)  {s->Lock();}
	inline QpSLock(QpLockBase &s): l_lock(&s) {s.Lock();}
	inline QpSLock(QpLockExBase *s, unsigned int msec): l_lock(s)  {s->Lock(msec, true);}
	inline QpSLock(QpLockExBase &s, unsigned int msec): l_lock(&s) {s.Lock(msec, true);}
	inline ~QpSLock() {l_lock->Unlock();}
    private:
	QpSLock(const QpSLock&);
	QpSLock& operator = (const QpSLock&);
};

/*
 * Semaphore
 */
class QpSem: public QpLockExBase {
    protected:
	int 		m_count;
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
    public:
	QpSem(int val = 1, const char *name = NULL);
	QpSem(const char *name);
	virtual ~QpSem();
	
	virtual bool LockAbs(const struct timespec *ts, bool throw_ex = true) {
		return LockValAbs(1, ts, throw_ex);
	}
	virtual void Lock() { 
		LockValAbs(1, NULL);
	}
	virtual bool Lock(const struct timespec *ts, bool throw_ex = true) {
		return LockVal(1, ts, throw_ex);
	}
	virtual bool Lock(unsigned int msec, bool throw_ex = true) {
		return LockVal(1, msec, throw_ex);
	}
	
	virtual bool LockValAbs(int val, const struct timespec *ts, bool throw_ex = true);
	virtual bool LockVal(int val, const struct timespec *ts, bool throw_ex = true);
	virtual bool LockVal(int val, unsigned int msec, bool throw_ex = true);
	
	virtual void Unlock() { UnlockVal(1);}
	virtual void UnlockVal(int val);
	virtual bool TryLock() { return TryLockVal(1);}
	virtual bool TryLockVal(int val);
	virtual int  CurVal();
	virtual QpCondBase *CondFactory();
    private:
	QpSem(const QpSem&);
	QpSem& operator = (const QpSem&);
};

/*
 * Monitor
 */
template <class T>
class QpMonitorT: public T {
    protected:
	QpCond m_cond;
    public:
	QpMonitorT(const char *name = NULL): T(name), m_cond(this, name) {}
	virtual ~QpMonitorT() {}
	/* Lock, Unlock, TryLock inherited from T */
	
	virtual bool WaitAbs(const struct timespec *ts, bool throw_ex = true) { 
		return m_cond.WaitAbs(ts, throw_ex);
	}
	virtual void Wait() {WaitAbs(NULL);}
	virtual bool Wait(const struct timespec *ts, bool throw_ex = true) {
		return m_cond.Wait(ts, throw_ex);
	}
	virtual bool Wait(unsigned int msec, bool throw_ex = true) {
		return m_cond.Wait(msec, throw_ex);
	}
	virtual void Signal()    { m_cond.Signal();}
	virtual void Broadcast() { m_cond.Broadcast();}
    private:
	QpMonitorT(const QpMonitorT&);
	QpMonitorT& operator = (const QpMonitorT&);
};

typedef QpMonitorT<QpMutex>    QpMonitor;
typedef QpMonitorT<QpMutexEx>  QpMonitorEx;
typedef QpMonitorT<QpSpinLock> QpMonitorSL;

#endif
