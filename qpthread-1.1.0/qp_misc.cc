/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#include <qpthr/qp.h>

/*
 * 
 * QpOnce
 * 
 */
QpOnce::QpOnce(QpRunnable *r, const char *name):
	QpBase(name), o_monitor(name), o_stat(O_DOIT),
	o_runnable(r), o_except(NULL)
{
}

QpOnce::~QpOnce()
{
	if (o_except)
		delete o_except;
}

void QpOnce::Main()
{
	if (o_runnable)
		o_runnable->InvokeMain();
}

void QpOnce::Raise()
{
	if (o_except)
		o_except->Raise();
}

void QpOnce::Do()
{
	o_monitor.Lock();
	while (o_stat == O_RUNNING)
		o_monitor.Wait();
	if (o_stat == O_DOIT) {
		o_stat = O_RUNNING;
		o_monitor.Unlock();
		try {
			Main();
		}
		catch (QpClonedException &e) {
			if (o_except)
				delete o_except;
			o_except = e.Clone();
		}
		catch (...) {
			do_exit("Unknown exception in QpOnce: %s", GetName() ? GetName() : "(null)");
		}
		o_monitor.Lock();
		o_stat = O_DONE;
		o_monitor.Broadcast();
	}
	o_monitor.Unlock();
}

/*
 * 
 * QpTSD
 * 
 */
QpTSD::QpTSD(const char *name, tsd_cleanup_func func):
	QpBase(name)
{
	QPERR(pthread_key_create(&ts_key, func));
}

QpTSD::~QpTSD()
{
	QPERR(pthread_key_delete(ts_key));
}

void *QpTSD::Get()
{
	return pthread_getspecific(ts_key);
}

void QpTSD::Set(void *arg)
{
	QPERR(pthread_setspecific(ts_key, arg));
}

