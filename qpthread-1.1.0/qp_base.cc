/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#include <string.h>
#include <fcntl.h>
#include <qpthr/qp.h>

void QpBase::SetName(const char *name)
{
	if (b_name)
		delete [] b_name;
	if (name) {
		if (!(b_name = new char[strlen(name) + 1]))
			throw QpOutOfMemoryException();
		strcpy(b_name, name);
	} else
		b_name = NULL;
}

/*
 * time conversions
 */
void reltime_to_abs(const struct timespec *ts_rel, struct timespec *ts_abs)
{
	struct timeval tv;
	
	QPERR_ERRNO(gettimeofday(&tv, NULL));
	ts_abs->tv_sec = tv.tv_sec + ts_rel->tv_sec;
	ts_abs->tv_nsec = tv.tv_usec * 1000 + ts_rel->tv_nsec;
	if (ts_abs->tv_nsec >= 1000000000) {
		ts_abs->tv_sec += ts_abs->tv_nsec / 1000000000;
		ts_abs->tv_nsec %= 1000000000;
	}
}

void msectime_to_ts(unsigned int msec, struct timespec *ts_rel)
{
	ts_rel->tv_sec = msec / 1000;
	ts_rel->tv_nsec = (msec % 1000) * 1000000;
}

#ifndef WIN32
int set_fd_flags(int fd, int flags, int mask)
{
	int oldval, val;
	
	QPERR_ERRNO(oldval = fcntl(fd, F_GETFL, 0));
	val = (oldval & (~mask)) | flags;
	if (val != oldval) {
		QPERR_ERRNO(fcntl(fd, F_SETFL, val));
	}
	return oldval;
}
#endif

extern "C" {
	
void do_msg(const char *fmt, ...)
{
        va_list ap;
	
	if (fmt) {
	        va_start(ap, fmt);
        	vfprintf(stderr, fmt, ap);
	        va_end(ap);
		fflush(stderr);
	}
}

void do_exit(const char *fmt, ...)
{
        va_list ap;

	if (fmt) {
	        va_start(ap, fmt);
        	vfprintf(stderr, fmt, ap);
	        va_end(ap);
		fflush(stderr);
	}
	if (QpInit::Capability(QpInit::QP_SIGNAL)) {
		/* 
		 * tell the signal thread to exit everything
		 * because it is not reliable to exit from
		 * signal handler within thread that received it
		 */
		QpSignal::Exit();
	} else {
#ifdef LINUX
		exit(1);	/* well, Linux has some thread cleaning on on_exit */
#else
		_exit(1);
#endif
	}
	_exit(1);
}

	
	
#ifdef WIN32
/*
 * gettimeofday()
 */
int gettimeofday(struct timeval *tp, void *pv)
{
  SYSTEMTIME st;
  struct tm  t;

  GetSystemTime(&st);

  memset((void *) &t, 0, sizeof(struct tm));
  /* typecast is the best trick to handle negative valuest */
  t.tm_sec  = (signed short ) st.wSecond;
  t.tm_min  = (signed short ) st.wMinute;
  t.tm_hour = (signed short ) st.wHour;
  t.tm_mday = (signed short ) st.wDay;
  t.tm_mon  = (signed short ) (st.wMonth - 1);
  t.tm_year = (signed short ) (st.wYear - 1900);
  t.tm_isdst = -1;

  tp->tv_sec  = mktime(&t);                   
  tp->tv_usec = st.wMilliseconds;
    
  return 0;
}

#endif
} /* extern "C" */

