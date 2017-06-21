/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_BASE_H
#define __QP_BASE_H

#include <stdlib.h>
#include <stdio.h>
/*
 * 
 * QpBase
 * 
 */
class QpBase {
    protected:
	char *b_name;
    public:
	QpBase(const char *name = NULL): b_name(NULL) { SetName(name);}
	QpBase(const QpBase& b): b_name(NULL)         { SetName(b.b_name);}
	virtual ~QpBase() { if (b_name) delete [] b_name;}
	
	QpBase& operator = (const QpBase& b) { SetName(b.b_name); return *this;}
	
	const char *GetName() { return b_name ? b_name : "";}
	void SetName(const char *name = NULL);
};


union QpRegItem {
	void 		*ptr;
	int  		nr;
	unsigned int 	unr;
};

void reltime_to_abs(const struct timespec *ts_rel, struct timespec *ts_abs);
void msectime_to_ts(unsigned int msec, struct timespec *ts_rel);


int set_fd_flags(int fd, int flags, int mask);

extern "C" {
#include <stdarg.h>

void do_msg(const char *fmt, ...);
void do_exit(const char *fmt, ...);
	
}

#define __QPERR(retval, x) { \
	do_msg("%s:%s/%d:%s\n", __FILE__, __LINE__, (retval), strerror(retval)); \
	throw QpSysException((retval), strerror(retval)); \
}

#define QPERR(x) { \
	int retvaltemp; \
	if ((retvaltemp = (x)) < 0) { \
		__QPERR(retvaltemp, x); \
	} \
}

#define __QPERR_ERRNO(x) { \
	int save_errno = errno; \
	do_msg("%s:%s/%d:%s\n", __FILE__, __LINE__, save_errno, strerror(save_errno)); \
	throw QpSysException(save_errno, strerror(save_errno)); \
}

#define QPERR_ERRNO(x) { \
	if ((x) == -1) { \
		__QPERR_ERRNO(x); \
	} \
}


#ifdef WIN32
extern "C" {

typedef unsigned long   sigset_t;

struct sigaction {
  void     (*sa_handler)(int); /* signal handler, or action value   */
  sigset_t  sa_mask;           /* signals to block while in handler */
  int       sa_flags;          /* signal action flags               */
  int       sa_signo;          /* sigal number                      */
};


int gettimeofday(struct timeval *tp, void * );

} // extern "C"

#endif

#endif
