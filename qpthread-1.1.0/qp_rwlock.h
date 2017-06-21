/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 *
 *	Copyright (C) 1999 by kra
 * 
 * 	based on work:
 *	Copyright (C) 1999 by Vivek Venugopalan
 *
 */

#ifndef __QP_RWLOCK_H__
#define __QP_RWLOCK_H__


class QpRWLockBase: public QpBase {
    public:
	QpRWLockBase(const char *name): QpBase(name) {}
	virtual ~QpRWLockBase() {}
	
	virtual void ReadLock() = 0;
	virtual void WriteLock() = 0;
	virtual bool TryReadLock() = 0;
	virtual bool TryWriteLock() = 0;
	virtual void Unlock() = 0;
};

class QpRWLockExBase: public QpRWLockBase {
    public:
	QpRWLockExBase(const char *name): QpRWLockBase(name) {}
	virtual ~QpRWLockExBase() {}
	
	virtual bool ReadLockAbs(const struct timespec *ts, bool throw_ex = true) = 0;
	virtual bool WriteLockAbs(const struct timespec *ts, bool throw_ex = true) = 0;
	
	virtual void ReadLock() { ReadLockAbs(NULL); }
	virtual bool ReadLock(const struct timespec *ts, bool throw_ex = true);
	virtual bool ReadLock(unsigned int msec, bool throw_ex = true);
	
	virtual void WriteLock() { WriteLockAbs(NULL); }
	virtual bool WriteLock(const struct timespec *ts, bool throw_ex = true);
	virtual bool WriteLock(unsigned int msec, bool throw_ex = true);
};

class QpRWLockEx2Base: public QpRWLockExBase {
    public:
	QpRWLockEx2Base(const char *name): QpRWLockExBase(name) {}
	virtual ~QpRWLockEx2Base() {}
	
	virtual bool ReadForWriteLockAbs(const struct timespec *ts, bool throw_ex = true) = 0;
	virtual bool TryReadForWriteLock() = 0;
	virtual void ReadForWriteUnlock() = 0;
	
	virtual bool PromoteWriteLockAbs(const struct timespec *ts, bool throw_ex = true) = 0;
	virtual bool TryPromoteWriteLock() = 0;
	
	virtual void ReadForWriteLock() { ReadForWriteLockAbs(NULL); }
	virtual bool ReadForWriteLock(const struct timespec *ts, bool throw_ex = true);
	virtual bool ReadForWriteLock(unsigned int msec, bool throw_ex = true);
	
	virtual void PromoteWriteLock() { PromoteWriteLockAbs(NULL); }
	virtual bool PromoteWriteLock(const struct timespec *ts, bool throw_ex = true);
	virtual bool PromoteWriteLock(unsigned int msec, bool throw_ex = true);
};

/*
 * Read/Write Lock (waiting writers have priority over waiting readers)
 */
class QpRWLock: public QpRWLockBase {
    private:
	void *rw_lock;
    public:
	QpRWLock(const char *name = NULL);
	virtual ~QpRWLock();

	virtual void ReadLock();
	virtual void WriteLock();
	virtual bool TryReadLock();
	virtual bool TryWriteLock();
	virtual void Unlock();
    private:
	QpRWLock(const QpRWLock&);
	QpRWLock& operator = (const QpRWLock&);
};

/*
 * 
 * Extended Read/Write Lock
 *
 */
class QpRWLockEx : public QpRWLockEx2Base {
    public:
	enum RW_PRIO {RW_PRIO_WRITE, RW_PRIO_READ };
    private:
	pthread_mutex_t l_mutex;
	pthread_cond_t	l_cond_readers, l_cond_writers;
	RW_PRIO		l_prio;
	int		l_ref_count;
	int		l_wait_readers, l_wait_writers;
    private:
	void WakeWriters();
	void Unlock(bool read_for_write);
	
    public:
	QpRWLockEx(RW_PRIO prio = RW_PRIO_WRITE, const char *name = NULL);
	virtual ~QpRWLockEx();
	
	virtual bool ReadLockAbs(const struct timespec *ts, bool throw_ex = true);
	virtual bool WriteLockAbs(const struct timespec *ts, bool throw_ex = true);

	virtual bool TryReadLock();
	virtual bool TryWriteLock();
	virtual void Unlock() 			{ Unlock(false); }

	virtual bool ReadForWriteLockAbs(const struct timespec *ts, bool throw_ex = true);
	virtual bool PromoteWriteLockAbs(const struct timespec *ts, bool throw_ex = true);
	virtual bool TryReadForWriteLock();
	virtual bool TryPromoteWriteLock();
	virtual void ReadForWriteUnlock()	{ Unlock(true); }

    private:
	QpRWLockEx(const QpRWLockEx&);
	QpRWLockEx& operator = (const QpRWLockEx&);
};

/*
 * Synchronization Read Lock
 */
class QpReadSLock {
    protected:
	QpRWLockBase *l_lock;
    public:
	inline QpReadSLock(QpRWLockBase *s): l_lock(s)  {s->ReadLock();}
	inline QpReadSLock(QpRWLockBase &s): l_lock(&s) {s.ReadLock();}
	inline QpReadSLock(QpRWLockExBase *s, unsigned int msec): l_lock(s)  {s->ReadLock(msec, true);}
	inline QpReadSLock(QpRWLockExBase &s, unsigned int msec): l_lock(&s) {s.ReadLock(msec, true);}
	inline ~QpReadSLock() {l_lock->Unlock();}
    private:
	QpReadSLock(const QpReadSLock&);
	QpReadSLock& operator = (const QpReadSLock&);
};

/*
 * Synchronization Write Lock
 */
class QpWriteSLock {
    protected:
	QpRWLockBase *l_lock;
    public:
	inline QpWriteSLock(QpRWLockBase *s): l_lock(s)  {s->WriteLock();}
	inline QpWriteSLock(QpRWLockBase &s): l_lock(&s) {s.WriteLock();}
	inline QpWriteSLock(QpRWLockExBase *s, unsigned int msec): l_lock(s)  {s->WriteLock(msec, true);}
	inline QpWriteSLock(QpRWLockExBase &s, unsigned int msec): l_lock(&s) {s.WriteLock(msec, true);}
	inline ~QpWriteSLock() {l_lock->Unlock();}
    private:
	QpWriteSLock(const QpWriteSLock&);
	QpWriteSLock& operator = (const QpWriteSLock&);
};

/*
 * Synchronization ReadForWrite Lock
 */
class QpReadForWriteSLock {
    protected:
	QpRWLockEx2Base *l_lock;
    public:
	inline QpReadForWriteSLock(QpRWLockEx2Base *s): l_lock(s)  {s->ReadForWriteLock();}
	inline QpReadForWriteSLock(QpRWLockEx2Base &s): l_lock(&s) {s.ReadForWriteLock();}
	inline QpReadForWriteSLock(QpRWLockEx2Base *s, unsigned int msec): l_lock(s)  {s->ReadForWriteLock(msec, true);}
	inline QpReadForWriteSLock(QpRWLockEx2Base &s, unsigned int msec): l_lock(&s) {s.ReadForWriteLock(msec, true);}
	inline ~QpReadForWriteSLock() {l_lock->ReadForWriteUnlock();}
    private:
	QpReadForWriteSLock(const QpReadForWriteSLock&);
	QpReadForWriteSLock& operator = (const QpReadForWriteSLock&);
};

#endif
