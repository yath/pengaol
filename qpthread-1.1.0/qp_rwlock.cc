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
#include <qpthr/qp.h>
#include <pthread.h>

#define RW_RFW_MARK	(1<<30)

#ifndef __USE_UNIX98
#include "qp_pthread_rwlock.h"
#endif

/*
 * QpRWLockExBase
 */
bool QpRWLockExBase::ReadLock(const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return ReadLockAbs(&ts_abs, throw_ex);
	} else
		return ReadLockAbs(NULL, throw_ex);
}

bool QpRWLockExBase::ReadLock(unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return ReadLock(&ts, throw_ex);
	} else
		return ReadLockAbs(NULL, throw_ex);
}

bool QpRWLockExBase::WriteLock(const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return WriteLockAbs(&ts_abs, throw_ex);
	} else
		return WriteLockAbs(NULL, throw_ex);
}

bool QpRWLockExBase::WriteLock(unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return WriteLock(&ts, throw_ex);
	} else
		return WriteLockAbs(NULL, throw_ex);
}

/*
 * QpRWLockEx2Base
 */
bool QpRWLockEx2Base::ReadForWriteLock(const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return ReadForWriteLockAbs(&ts_abs, throw_ex);
	} else
		return ReadForWriteLockAbs(NULL, throw_ex);
}

bool QpRWLockEx2Base::ReadForWriteLock(unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return ReadForWriteLock(&ts, throw_ex);
	} else
		return ReadForWriteLockAbs(NULL, throw_ex);
}

bool QpRWLockEx2Base::PromoteWriteLock(const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return PromoteWriteLockAbs(&ts_abs, throw_ex);
	} else
		return PromoteWriteLockAbs(NULL, throw_ex);
}

bool QpRWLockEx2Base::PromoteWriteLock(unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return PromoteWriteLock(&ts, throw_ex);
	} else
		return PromoteWriteLockAbs(NULL, throw_ex);
}

/*
 * 
 * QpRWLock
 * 
 */
QpRWLock::QpRWLock(const char *name): QpRWLockBase(name)
{
#ifdef __USE_UNIX98
	rw_lock = new pthread_rwlock_t;
	QPERR(pthread_rwlock_init((pthread_rwlock_t *) rw_lock, NULL));
#else
	rw_lock = new qp_rwlock_t;
	QPERR(qp_rwlock_init((qp_rwlock_t *) rw_lock, NULL));
#endif
}

QpRWLock::~QpRWLock()
{
#ifdef __USE_UNIX98
	QPERR(pthread_rwlock_destroy((pthread_rwlock_t *) rw_lock));
	delete (pthread_rwlock_t *) rw_lock;
#else
	QPERR(qp_rwlock_destroy((qp_rwlock_t *) rw_lock));
	delete (qp_rwlock_t *) rw_lock;
#endif
}

void QpRWLock::ReadLock()
{
#ifdef __USE_UNIX98
	QPERR(pthread_rwlock_rdlock((pthread_rwlock_t *) rw_lock));
#else
	QPERR(qp_rwlock_rdlock((qp_rwlock_t *) rw_lock));
#endif
}

void QpRWLock::WriteLock()
{
#ifdef __USE_UNIX98
	QPERR(pthread_rwlock_wrlock((pthread_rwlock_t *) rw_lock));
#else
	QPERR(qp_rwlock_wrlock((qp_rwlock_t *) rw_lock));
#endif
}

bool QpRWLock::TryReadLock()
{
	int retval;
	
#ifdef __USE_UNIX98
	retval = pthread_rwlock_tryrdlock((pthread_rwlock_t *) rw_lock);
	if (retval < 0 && retval != EBUSY) {
		__QPERR(retval, pthread_rwlock_tryrdlock);
	}
#else
	retval = qp_rwlock_tryrdlock((qp_rwlock_t *) rw_lock);
	if (retval < 0 && retval != EBUSY) {
		__QPERR(retval, qp_rwlock_tryrdlock);
	}
#endif
	if (retval)
		return false;
	else
		return true;
}

bool QpRWLock::TryWriteLock()
{
 	int retval;
	
#ifdef __USE_UNIX98
	retval = pthread_rwlock_trywrlock((pthread_rwlock_t *) rw_lock);
	if (retval < 0 && retval != EBUSY) {
		__QPERR(retval, pthread_rwlock_trywrlock);
	}
#else
	retval = qp_rwlock_trywrlock((qp_rwlock_t *) rw_lock);
	if (retval < 0 && retval != EBUSY) {
		__QPERR(retval, qp_rwlock_trywrlock);
	}
#endif
	if (retval)
		return false;
	else
		return true;
}

void QpRWLock::Unlock()
{
#ifdef __USE_UNIX98
	QPERR(pthread_rwlock_unlock((pthread_rwlock_t *) rw_lock));
#else
	QPERR(qp_rwlock_unlock((qp_rwlock_t *) rw_lock));
#endif
}

/*
 * 
 * QpRWLockEx
 * 
 */
QpRWLockEx::QpRWLockEx(RW_PRIO prio, const char *name): 
	QpRWLockEx2Base(name),
	l_prio(prio),
	l_ref_count(0), l_wait_readers(0),  l_wait_writers(0)
{
	QPERR(pthread_mutex_init(&l_mutex, NULL));
	QPERR(pthread_cond_init(&l_cond_readers, NULL));
	QPERR(pthread_cond_init(&l_cond_writers, NULL));
}

QpRWLockEx::~QpRWLockEx()
{
	QPERR(pthread_cond_destroy(&l_cond_writers));
	QPERR(pthread_cond_destroy(&l_cond_readers));
	QPERR(pthread_mutex_destroy(&l_mutex));
}

class pthread_lock {
    private:
	pthread_mutex_t *l_lock;
    public:
	inline pthread_lock(pthread_mutex_t *s): l_lock(s) {
		QPERR(pthread_mutex_lock(l_lock));
	}
	inline ~pthread_lock() {
		QPERR(pthread_mutex_unlock(l_lock));
	}
    private:
	pthread_lock(const pthread_lock&);
	pthread_lock& operator = (const pthread_lock&);
};

inline bool pthread_cwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *ts)
{
	int retval;
	
	if (ts)
		retval = pthread_cond_timedwait(c, m, ts);
	else
		retval = pthread_cond_wait(c, m);

	if (retval != 0 && retval != ETIMEDOUT && retval != EINTR) {
		__QPERR(retval, pthread_cond_timedwait/wait);
	}
	if (retval == ETIMEDOUT)
		return false;
	else
		return true;
}

bool QpRWLockEx::ReadLockAbs(const struct timespec *ts, bool throw_ex)
{
	bool retval;
	pthread_lock lock(&l_mutex);
	
	while (l_ref_count < 0 || 
	       (l_prio == RW_PRIO_WRITE && l_wait_writers > 0)) {
		++l_wait_readers;
		retval = pthread_cwait(&l_cond_readers, &l_mutex, ts);
		--l_wait_readers;
		
		if (!retval) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return false;
		}
	}
	++l_ref_count;
	return true;
}

bool QpRWLockEx::WriteLockAbs(const struct timespec *ts, bool throw_ex)
{
	bool retval;
	pthread_lock lock(&l_mutex);
	
	while (l_ref_count != 0 ||
	       (l_prio == RW_PRIO_READ && l_wait_readers > 0)) {
		++l_wait_writers;
		retval = pthread_cwait(&l_cond_writers, &l_mutex, ts);
		--l_wait_writers;
		
		if (!retval) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return false;
		}
	}
	l_ref_count = -1;
	return true;
}

bool QpRWLockEx::TryReadLock()
{
	pthread_lock lock(&l_mutex);
	
	if (l_ref_count < 0 ||
	    (l_prio == RW_PRIO_WRITE && l_wait_writers > 0))
		return false;
	else {
		++l_ref_count;
		return true;
	}
}

bool QpRWLockEx::TryWriteLock()
{
	pthread_lock lock(&l_mutex);
	
	if (l_ref_count ||
	    (l_prio == RW_PRIO_READ && l_wait_readers > 0))
		return false;
	else {
		l_ref_count = -1;
		return true;
	}
}

bool QpRWLockEx::ReadForWriteLockAbs(const struct timespec *ts, bool throw_ex)
{
	bool retval;
	pthread_lock lock(&l_mutex);
	
	while (l_ref_count < 0 || l_ref_count >= RW_RFW_MARK || 
	       (l_prio == RW_PRIO_WRITE && l_wait_writers > 0)) {
		++l_wait_readers;
		retval = pthread_cwait(&l_cond_readers, &l_mutex, ts);
		--l_wait_readers;
		
		if (!retval) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return false;
		}
	}
	l_ref_count += RW_RFW_MARK;
	return true;
}

bool QpRWLockEx::TryReadForWriteLock()
{
	pthread_lock lock(&l_mutex);
	
	if (l_ref_count < 0 || l_ref_count >= RW_RFW_MARK || 
	    (l_prio == RW_PRIO_WRITE && l_wait_writers > 0))
		return false;
	else {
		l_ref_count += RW_RFW_MARK;
		return true;
	}
}

bool QpRWLockEx::PromoteWriteLockAbs(const struct timespec *ts, bool throw_ex)
{
	bool retval;
	pthread_lock lock(&l_mutex);
	
	while (l_ref_count != 1 && l_ref_count != RW_RFW_MARK) {
		if (l_wait_writers >= RW_RFW_MARK)
			throw QpErrorException(QP_ERROR_DEADLOCK, "in PromoteWriteLockAbs");
		
		l_wait_writers += RW_RFW_MARK;
		retval = pthread_cwait(&l_cond_writers, &l_mutex, ts);
		l_wait_writers -= RW_RFW_MARK;
		
		if (!retval) {
			if (throw_ex)
				throw QpTimeoutException();
			else
				return false;
		}
	}
	l_ref_count = -1;
	return true;
}

bool QpRWLockEx::TryPromoteWriteLock()
{
	pthread_lock lock(&l_mutex);
	
	if (l_ref_count == 1 || l_ref_count == RW_RFW_MARK) {
		l_ref_count = -1;
		return true;
	} else
		return false;
}

void QpRWLockEx::WakeWriters()
{
	if (l_ref_count == 0) { /* no left readers - wake up */
		QPERR(pthread_cond_signal(&l_cond_writers));
	} else if ((l_ref_count == 1 || l_ref_count == RW_RFW_MARK) &&
	    l_wait_writers >= RW_RFW_MARK) { /* promote writer is waiting */
		QPERR(pthread_cond_broadcast(&l_cond_writers));
	}
}

void QpRWLockEx::Unlock(bool read_for_write)
{
	pthread_lock lock(&l_mutex);
	
	if (l_ref_count > 0) {
		if (read_for_write) {
			if (l_ref_count < RW_RFW_MARK)
				throw QpErrorException(QP_ERROR_STATE, "QpRWLockEx::ReadForWriteUnlock without ReadForWriteLock");
			l_ref_count -= RW_RFW_MARK;
		} else
			--l_ref_count;
	} else if (l_ref_count == -1) {
		l_ref_count = 0;
	} else
		throw QpErrorException(QP_ERROR_STATE, "QpRWLockEx bad ref_count");
	if (l_prio == RW_PRIO_WRITE) {
		if (l_wait_writers > 0)
			WakeWriters();
		else if (l_wait_readers > 0) {
			QPERR(pthread_cond_broadcast(&l_cond_readers));
		}
	} else {
		if (l_wait_readers > 0) {
			QPERR(pthread_cond_broadcast(&l_cond_readers));
		} else if (l_wait_writers > 0)
			WakeWriters();
	}
}

