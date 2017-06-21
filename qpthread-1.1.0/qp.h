/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_H
#define __QP_H

#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <vector>
#include <list>
#include <deque>

#ifdef WIN32
#include <sched.h>
#include <iostream>
using namespace std;
#endif

#if !defined( _REENTRANT ) && !defined( WIN32 )
#error you have to compile with -D_REENTRANT
#endif

#define QP_PRIO_DFL	0
#define QP_PRIO_MIN	1
#define QP_PRIO_AVGMIN	2
#define QP_PRIO_AVG	3
#define QP_PRIO_AVGMAX	4
#define QP_PRIO_MAX	5

#include <qpthr/qp_base.h>
#include <qpthr/qp_except.h>
#include <qpthr/qp_synch.h>
#include <qpthr/qp_thr.h>
#include <qpthr/qp_sig.h>
#include <qpthr/qp_queue.h>
#include <qpthr/qp_timer.h>
#include <qpthr/qp_misc.h>
#include <qpthr/qp_rwlock.h>

class QpInit {
    public:
	enum QP_CAP {QP_NONE = 0, QP_SIGNAL = 1, QP_TIMER = 2, QP_ALL=255};
	enum QP_SCOPE {QP_SCOPE_SYSTEM = 0, QP_SCOPE_PROCESS = 1};
    private:
	static QpInit 	*qp_initialized;
	static QP_CAP   qp_capability;
	static QP_SCOPE qp_scope;
	static int	qp_sched_policy;
	static int	qp_prio_table[QP_PRIO_MAX + 1];
	static QpSignal *qp_signal;
	static QpTimer  *qp_timer;

    public:	
	QpInit(QP_CAP capability = QP_NONE, QP_SCOPE scope = QP_SCOPE_SYSTEM);
	~QpInit();

	static bool Initialized() { if (qp_initialized) return true; else return false;}
	static QP_CAP Capability(QP_CAP mask = QP_ALL);
	static int DflSchedPolicy();
	static int DflScope();
	static void AdjustSchedParam(unsigned int prio, struct sched_param *sch);
};

#endif
