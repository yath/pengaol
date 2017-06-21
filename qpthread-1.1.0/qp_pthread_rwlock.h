#ifndef	__QP_PTHREAD_RWLOCK_H
#define __QP_PTHREAD_RWLOCK_H

#ifndef WIN32
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct {
	pthread_mutex_t	rw_mutex;
	pthread_cond_t	rw_cond_readers;
	pthread_cond_t	rw_cond_writers;
	int		rw_wait_readers;
	int		rw_wait_writers;
	int		rw_refcount;
} qp_rwlock_t;

typedef	int qp_rwlockattr_t;


inline int qp_rwlock_init(qp_rwlock_t *rw, qp_rwlockattr_t *attr)
{
	int retval;

	if (attr != NULL)
		return EINVAL;

	if ((retval = pthread_mutex_init(&rw->rw_mutex, NULL)) != 0)
		goto err1;
	if ((retval = pthread_cond_init(&rw->rw_cond_readers, NULL)) != 0)
		goto err2;
	if ((retval = pthread_cond_init(&rw->rw_cond_writers, NULL)) != 0)
		goto err3;
	rw->rw_wait_readers = 0;
	rw->rw_wait_writers = 0;
	rw->rw_refcount = 0;

	return 0;
err3:
	pthread_cond_destroy(&rw->rw_cond_readers);
err2:
	pthread_mutex_destroy(&rw->rw_mutex);
err1:
	return retval;
}

inline int qp_rwlock_destroy(qp_rwlock_t *rw)
{
	if (rw->rw_refcount != 0 || rw->rw_wait_readers != 0 || rw->rw_wait_writers != 0)
		return EBUSY;

	pthread_mutex_destroy(&rw->rw_mutex);
	pthread_cond_destroy(&rw->rw_cond_readers);
	pthread_cond_destroy(&rw->rw_cond_writers);

	return 0;
}

inline int qp_rwlock_rdlock(qp_rwlock_t *rw)
{
	int retval;

	if ((retval = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return retval;

	while (rw->rw_refcount < 0 || rw->rw_wait_writers > 0) {
		rw->rw_wait_readers++;
		retval = pthread_cond_wait(&rw->rw_cond_readers, &rw->rw_mutex);
		rw->rw_wait_readers--;
		if (retval != 0)
			break;
	}
	if (retval == 0)
		rw->rw_refcount++;

	pthread_mutex_unlock(&rw->rw_mutex);
	return retval;
}

inline int qp_rwlock_tryrdlock(qp_rwlock_t *rw)
{
	int retval;

	if ((retval = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return retval;

	if (rw->rw_refcount < 0 || rw->rw_wait_writers > 0)
		retval = EBUSY;
	else
		rw->rw_refcount++;

	pthread_mutex_unlock(&rw->rw_mutex);
	return retval;
}

inline int qp_rwlock_trywrlock(qp_rwlock_t *rw)
{
	int retval;

	if ((retval = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return retval;

	if (rw->rw_refcount != 0)
		retval = EBUSY;
	else
		rw->rw_refcount = -1;

	pthread_mutex_unlock(&rw->rw_mutex);
	return retval;
}

inline int qp_rwlock_unlock(qp_rwlock_t *rw)
{
	int retval;

	if ((retval = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return retval;

	if (rw->rw_refcount > 0)
		rw->rw_refcount--;
	else if (rw->rw_refcount == -1)
		rw->rw_refcount = 0;
	else {
		fprintf(stderr, "qp_rwlock_unlock error: rw_refcount = %d", rw->rw_refcount);
		exit(1);
	}
		

	/* 4give preference to waiting writers over waiting readers */
	if (rw->rw_wait_writers > 0) {
		if (rw->rw_refcount == 0)
			retval = pthread_cond_signal(&rw->rw_cond_writers);
	} else if (rw->rw_wait_readers > 0)
		retval = pthread_cond_broadcast(&rw->rw_cond_readers);

	pthread_mutex_unlock(&rw->rw_mutex);
	return retval;
}

inline int qp_rwlock_wrlock(qp_rwlock_t *rw)
{
	int retval;

	if ((retval = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return retval;

	while (rw->rw_refcount != 0) {
		rw->rw_wait_writers++;
		retval = pthread_cond_wait(&rw->rw_cond_writers, &rw->rw_mutex);
		rw->rw_wait_writers--;
		if (retval != 0)
			break;
	}
	if (retval == 0)
		rw->rw_refcount = -1;

	pthread_mutex_unlock(&rw->rw_mutex);
	return retval;
}


#ifdef __cplusplus
}
#endif

#endif
