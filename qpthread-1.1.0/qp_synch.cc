/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#include <fcntl.h>
#include <qpthr/qp.h>


bool QpCondBase::Wait(const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return WaitAbs(&ts_abs, throw_ex);
	} else
		return WaitAbs(NULL, throw_ex);
}

bool QpCondBase::Wait(unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return Wait(&ts, throw_ex);
	} else
		return WaitAbs(NULL, throw_ex);
}

/*
 * QpAsyncSafeSem
 */
QpAsyncSafeSem::QpAsyncSafeSem(const char *name): QpLockBase(name)
{
#ifdef WIN32
	throw QpUnsupportedException();
#else
	QPERR_ERRNO(pipe(fd));
	QPERR_ERRNO(write(fd[1], "", 1));
#endif
}

QpAsyncSafeSem::~QpAsyncSafeSem()
{
#ifdef WIN32
	throw QpUnsupportedException();
#else
	close(fd[0]);
	close(fd[1]);
#endif
}

void QpAsyncSafeSem::Lock()
{
#ifdef WIN32
	throw QpUnsupportedException();
#else
	int save_errno, retval;
	char buf[1];
	
	save_errno = errno;
	while ((retval = read(fd[0], buf, 1)) < 0 &&
	       (errno == EINTR || errno == EAGAIN))
		;
	if (retval <= 0)
		__QPERR_ERRNO(read);
	errno = save_errno;
#endif
}

void QpAsyncSafeSem::Unlock()
{
#ifdef WIN32
	throw QpUnsupportedException();
#else
	int save_errno, retval;
	
	save_errno = errno;
	while ((retval = write(fd[1], "", 1)) < 0 &&
	       (errno == EINTR || errno == EAGAIN))
		;
	if (retval <= 0)
		__QPERR_ERRNO(write);
	errno = save_errno;
#endif
}

/*
 * TryLock on QpAsyncSafeSem is really, really slow
 */
bool QpAsyncSafeSem::TryLock()
{
#ifdef WIN32
	throw QpUnsupportedException();
#else
#if 1
	int save_errno, read_errno;
	int retval;
	char buf[1];
	
	save_errno = errno;
	try {
		/*
		 * OK, switch to O_NONBLOCK mode for any further operation
		 */
		set_fd_flags(fd[0], O_NONBLOCK, 0);
	}
	catch (...) {
		errno = save_errno;
		throw;
	}
	while ((retval = read(fd[0], buf, 1)) < 0 && errno == EINTR)
		;
	read_errno = errno;
	errno = save_errno;
	if (retval == 1)
		return true;
	if (retval < 0 && read_errno == EAGAIN)
		return false;
	__QPERR(read_errno, read);
//	return false;
#else
	/*
	 * this implementation have one major problem - 
	 * the flags are shared across file descriptors
	 * so if you set O_NONBLOCK for one the other will have
	 * also O_NONBLOCK, so we can not avoid race condition
	 * when unseting O_NONBLOCK.
	 */
	char buf[1];
	int fdnb;
	int save_errno, read_errno;
	int retval;

	/*
	 * we cannot set/unset NONBLOCK on fd[1] - race
	 * condition when several TryLock are in action
	 */
	save_errno = errno;
	QPERR_ERRNO(fdnb = dup(fd[0]));
	try {
		set_fd_flags(fdnb, O_NONBLOCK, 0);
	}
	catch (...) {
		close(fdnb);
		errno = save_errno;
		throw;
	}
	while ((retval = read(fdnb, buf, 1)) < 0 && errno == EINTR)
		;
	read_errno = errno;
	close(fdnb);
	errno = save_errno;
	if (retval == 1) {
		return true;
	}
	if (retval < 0 && read_errno == EAGAIN) {
		return false;
	}
	__QPERR(read_errno, read);
	return false;
#endif
#endif
}

QpCondBase *QpAsyncSafeSem::CondFactory()
{
#ifdef WIN32
	throw QpUnsupportedException();
#else
	QpCondBase *retval = new QpCondUni(this, GetName());
	if (!retval)
		throw QpOutOfMemoryException();
	return retval;
#endif
}

/*
 * QpSpinLock
 */
QpSpinLock::QpSpinLock(bool yield, const char *name):
	QpLockBase(name),
	sp_lock(0),
	sp_yield(yield)
{
	/* well the Digital compiler doesn't like ?: construction, uhhh */
#ifndef ARCH_TAS
	throw QpUnsupportedException();
#endif
}

void QpSpinLock::Lock()
{
#ifdef ARCH_TAS
	while (qp_arch_testandset(&sp_lock))
		if (sp_yield) {
			QPERR_ERRNO(sched_yield());
		}
#else
	throw QpUnsupportedException();
#endif
}

void QpSpinLock::Unlock()
{
#ifdef ARCH_TAS
	qp_arch_release(&sp_lock);
#else
	throw QpUnsupportedException();
#endif
}

bool QpSpinLock::TryLock()
{
#ifdef ARCH_TAS
	if (qp_arch_testandset(&sp_lock))
		return false;
	else
		return true;
#else
	throw QpUnsupportedException();
#endif
}

QpCondBase *QpSpinLock::CondFactory()
{
#ifdef ARCH_TAS
	QpCondBase *retval = new QpCondUni(this, GetName());
	if (!retval)
		throw QpOutOfMemoryException();
	return retval;
#else
	throw QpUnsupportedException();
	return NULL;
#endif
}

/*
 * QpLockExBase
 */
bool QpLockExBase::Lock(const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return LockAbs(&ts_abs, throw_ex);
	} else
		return LockAbs(NULL, throw_ex);
}

bool QpLockExBase::Lock(unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return Lock(&ts, throw_ex);
	} else
		return LockAbs(NULL, throw_ex);
}

/*
 * QpMutex
 */
QpMutex::QpMutex(const char *name): QpLockBase(name)
{
	QPERR(pthread_mutex_init(&m_mutex, NULL));
}

QpMutex::~QpMutex()
{ 
	QPERR(pthread_mutex_destroy(&m_mutex));
}

void QpMutex::Lock()
{ 
	QPERR(pthread_mutex_lock(&m_mutex));
}

void QpMutex::Unlock()
{ 
	QPERR(pthread_mutex_unlock(&m_mutex));
}

bool QpMutex::TryLock()
{ 
	int retval;

	retval = pthread_mutex_trylock(&m_mutex);
	if (retval < 0 && retval != EBUSY) {
		__QPERR(retval, pthread_mutex_trylock);
	}
	if (retval)
		return false;
	else
		return true;
}

QpCondBase *QpMutex::CondFactory()
{
	QpCondBase *retval = new QpMutexCond(this, GetName());
	if (!retval)
		throw QpOutOfMemoryException();
	return retval;
}

QpMutexCond::QpMutexCond(QpMutex *m, const char *name):
	QpCondBase(name), c_mutex(m)
{
	QPERR(pthread_cond_init(&c_cond, NULL));
}

QpMutexCond::QpMutexCond(QpMutex& m, const char *name):
	QpCondBase(name), c_mutex(&m)
{
	QPERR(pthread_cond_init(&c_cond, NULL));
}

bool QpMutexCond::WaitAbs(const struct timespec *ts, bool throw_ex)
{
	int retval;
	
	if (ts)
		retval = pthread_cond_timedwait(&c_cond, &c_mutex->m_mutex, ts);
	else
		retval = pthread_cond_wait(&c_cond, &c_mutex->m_mutex);

	if (retval != 0 && retval != ETIMEDOUT && retval != EINTR) {
		__QPERR(retval, pthread_cond_timedwait/wait);
	}
	if (retval == ETIMEDOUT) {
		if (throw_ex)
			throw QpTimeoutException();
		else
			return false;
	} else
		return true;
}

void QpMutexCond::Signal()
{ 
	QPERR(pthread_cond_signal(&c_cond));
}

void QpMutexCond::Broadcast()
{ 
	QPERR(pthread_cond_broadcast(&c_cond));
}

/*
 * QpCondUni
 */
QpCondUni::QpCondUni(QpLockBase *l, const char *name):
	QpCondBase(name), c_lock(l)
{
	QPERR(pthread_mutex_init(&c_mutex, NULL));
	QPERR(pthread_cond_init(&c_cond, NULL));
}

QpCondUni::QpCondUni(QpLockBase& l, const char *name):
	QpCondBase(name), c_lock(&l)
{
	QPERR(pthread_mutex_init(&c_mutex, NULL));
	QPERR(pthread_cond_init(&c_cond, NULL));
}

QpCondUni::~QpCondUni()
{
	QPERR(pthread_cond_destroy(&c_cond));
	QPERR(pthread_mutex_destroy(&c_mutex));
}

bool QpCondUni::WaitAbs(const struct timespec *ts, bool throw_ex)
{
	int retval = 0;
	
	QPERR(pthread_mutex_lock(&c_mutex));
	c_lock->Unlock();
	if (ts)
		retval = pthread_cond_timedwait(&c_cond, &c_mutex, ts);
	else
		retval = pthread_cond_wait(&c_cond, &c_mutex);
	if (retval != 0 && retval != ETIMEDOUT && retval != EINTR) {
		__QPERR(retval, pthread_cond_timedwait/wait);
	}
	QPERR(pthread_mutex_unlock(&c_mutex));
	c_lock->Lock();
	if (retval == ETIMEDOUT) {
		if (throw_ex)
			throw QpTimeoutException();
		else
			return false;
	} else
		return true;
}

void QpCondUni::Signal()
{
	QPERR(pthread_mutex_lock(&c_mutex));
	QPERR(pthread_cond_signal(&c_cond));
	QPERR(pthread_mutex_unlock(&c_mutex));
}

void QpCondUni::Broadcast() 
{
	QPERR(pthread_mutex_lock(&c_mutex));
	QPERR(pthread_cond_broadcast(&c_cond));
	QPERR(pthread_mutex_unlock(&c_mutex));
}

/*
 * QpMutexEx
 */
QpMutexEx::QpMutexEx(const char *name):
	QpLockExBase(name), m_count(0), m_owner(NULL),
	m_recursive(false)
{
	QPERR(pthread_mutex_init(&m_mutex, NULL));
	QPERR(pthread_cond_init(&m_cond, NULL));
}

QpMutexEx::QpMutexEx(bool recursive, const char *name):
	QpLockExBase(name), m_count(0), m_owner(NULL),
	m_recursive(recursive)
{
	QPERR(pthread_mutex_init(&m_mutex, NULL));
	QPERR(pthread_cond_init(&m_cond, NULL));
}

QpMutexEx::~QpMutexEx()
{
	QPERR(pthread_cond_destroy(&m_cond));
	QPERR(pthread_mutex_destroy(&m_mutex));
}

QpCondBase *QpMutexEx::CondFactory()
{
	QpCondBase *retval = new QpCondUni(this, GetName());
	if (!retval)
		throw QpOutOfMemoryException();
	return retval;
}

bool QpMutexEx::LockAbs(const struct timespec *ts, bool throw_ex)
{
	int retval;
	QpThread *current = QpThread::Current();
	
	QPERR(pthread_mutex_lock(&m_mutex));
	retval = 0;
	while (m_count > 0 && m_owner != current && retval != ETIMEDOUT) {
		if (ts)
			retval = pthread_cond_timedwait(&m_cond, &m_mutex, ts);
		else
			retval = pthread_cond_wait(&m_cond, &m_mutex);
	}
	if (retval != 0 && retval != ETIMEDOUT && retval != EINTR) {
		__QPERR(retval, pthread_cond_timedwait/wait);
	}
	if (m_count > 0) {
		if (m_owner == current && m_recursive == false)
			throw QpErrorException(QP_ERROR_DEADLOCK, "QpMutexEx::LockAbs");
		if (m_owner != current)
			do_exit("QPTHR FATAL ERROR: QpMutexEx::LockAbs: m_owner != current");
	}
	if (m_count == 0 || (m_count > 0 && m_owner == current)) {
		m_count++;
		m_owner = current;
		retval = 0;
	}
	QPERR(pthread_mutex_unlock(&m_mutex));
	if (retval == ETIMEDOUT) {
		if (throw_ex)
			throw QpTimeoutException();
		else
			return false;
	} else
		return true;
}

void QpMutexEx::Unlock() 
{
	int signal_it;
	
	QPERR(pthread_mutex_lock(&m_mutex));
	if (m_count == 0 || m_owner != QpThread::Current())
		throw QpErrorException(QP_ERROR_STATE, "QpMutexEx::Unlock");
	if (--m_count == 0) {
		m_owner = NULL;
		signal_it = 1;
	} else
		signal_it = 0;
	QPERR(pthread_mutex_unlock(&m_mutex));
	if (signal_it)
		QPERR(pthread_cond_signal(&m_cond));
}

bool QpMutexEx::TryLock() 
{
	bool retval;
	QpThread *current = QpThread::Current();
	
	QPERR(pthread_mutex_lock(&m_mutex));
	if (m_count == 0 || (m_count > 0 && m_owner == current && m_recursive)) {
		m_count++;
		m_owner = current;
		retval = true;
	} else
		retval = false;
	QPERR(pthread_mutex_unlock(&m_mutex));
	return retval;
}

QpThread *QpMutexEx::Owner()
{
	QpThread *retval;
	
	QPERR(pthread_mutex_lock(&m_mutex));
	retval = m_owner;
	QPERR(pthread_mutex_unlock(&m_mutex));
	return retval;
}

/*
 * QpCond
 */
QpCond::QpCond(QpLockBase *s, const char *name): 
	QpCondBase(name)
{
	c_cond = s->CondFactory();
}

QpCond::QpCond(QpLockBase& s, const char *name): 
	QpCondBase(name)
{
	c_cond = s.CondFactory();
}

/*
 * QpSem
 */
QpSem::QpSem(int val, const char *name):
	QpLockExBase(name), m_count(val)
{
 	QPERR(pthread_mutex_init(&m_mutex, NULL));
	QPERR(pthread_cond_init(&m_cond, NULL));
}

QpSem::QpSem(const char *name):
	QpLockExBase(name), m_count(1)
{
	QPERR(pthread_mutex_init(&m_mutex, NULL));
	QPERR(pthread_cond_init(&m_cond, NULL));
}

QpSem::~QpSem() 
{
	QPERR(pthread_cond_destroy(&m_cond));
	QPERR(pthread_mutex_destroy(&m_mutex));
}

bool QpSem::LockValAbs(int val, const struct timespec *ts, bool throw_ex)
{
	int retval;
	
	QPERR(pthread_mutex_lock(&m_mutex));
	retval = 0;
	while (m_count < (int) val && retval != ETIMEDOUT) {
		if (ts)
			retval = pthread_cond_timedwait(&m_cond, &m_mutex, ts);
		else
			retval = pthread_cond_wait(&m_cond, &m_mutex);
	}
	if (retval != 0 && retval != ETIMEDOUT && retval != EINTR) {
		__QPERR(retval, pthread_cond_timedwait/wait);
	}
	if (m_count >= (int) val) {
		m_count -= val;
		retval = 0;
	}
	QPERR(pthread_mutex_unlock(&m_mutex));
	if (retval == ETIMEDOUT) {
		if (throw_ex)
			throw QpTimeoutException();
		else
			return false;
	} else
		return true;
}

bool QpSem::LockVal(int val, const struct timespec *ts, bool throw_ex)
{
	if (ts) {
		struct timespec ts_abs;
		reltime_to_abs(ts, &ts_abs);
		return LockValAbs(val, &ts_abs, throw_ex);
	} else
		return LockValAbs(val, NULL, throw_ex);
}

bool QpSem::LockVal(int val, unsigned int msec, bool throw_ex)
{
	if (msec) {
		struct timespec ts;
		msectime_to_ts(msec, &ts);
		return LockVal(val, &ts, throw_ex);
	} else
		return LockValAbs(val, NULL, throw_ex);
}

void QpSem::UnlockVal(int val)
{
	QPERR(pthread_mutex_lock(&m_mutex));
	m_count += val;
	QPERR(pthread_mutex_unlock(&m_mutex));
	QPERR(pthread_cond_broadcast(&m_cond));
}

bool QpSem::TryLockVal(int val) 
{
	bool retval;
	
	QPERR(pthread_mutex_lock(&m_mutex));
	if (m_count >= (int) val) {
		m_count -= val;
		retval = true;
	} else
		retval = false;
	QPERR(pthread_mutex_unlock(&m_mutex));
	return retval;
}

int QpSem::CurVal()
{
	int retval;
	
	QPERR(pthread_mutex_lock(&m_mutex));
	retval = m_count;
	QPERR(pthread_mutex_unlock(&m_mutex));
	return retval;
}

QpCondBase *QpSem::CondFactory() {
	QpCondBase *retval = new QpCondUni(this, GetName());
	if (!retval)
		throw QpOutOfMemoryException();
	return retval;
}

