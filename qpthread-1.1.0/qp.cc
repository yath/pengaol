/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#include <qpthr/qp.h>

QpInit		 *QpInit::qp_initialized = NULL;
QpInit::QP_CAP   QpInit::qp_capability = QpInit::QP_NONE;
QpInit::QP_SCOPE QpInit::qp_scope = QpInit::QP_SCOPE_SYSTEM;
int		 QpInit::qp_sched_policy = SCHED_OTHER;
int 		 QpInit::qp_prio_table[QP_PRIO_MAX + 1];
QpSignal 	 *QpInit::qp_signal = NULL;
QpTimer  	 *QpInit::qp_timer = NULL;

QpInit::QpInit(QP_CAP capability, QP_SCOPE scope)
{
	int sched_min, sched_max, sched_4;
	
	if (!qp_initialized) {
		qp_capability = capability;
		qp_scope = scope;
		qp_sched_policy = SCHED_OTHER;
		
		sched_min = sched_get_priority_min(DflSchedPolicy());
		sched_max = sched_get_priority_max(DflSchedPolicy());
		sched_4 = (sched_max - sched_min) / 4;
		qp_prio_table[QP_PRIO_MIN] = sched_min;
		qp_prio_table[QP_PRIO_AVGMIN] = sched_min + sched_4;
		qp_prio_table[QP_PRIO_AVG] = sched_min + (sched_max - sched_min) / 2;
		qp_prio_table[QP_PRIO_AVGMAX] = sched_max - sched_4;
		qp_prio_table[QP_PRIO_MAX] = sched_max;
		qp_prio_table[QP_PRIO_DFL] = qp_prio_table[QP_PRIO_AVG];
		
		if (capability & QP_SIGNAL)
			QpSignal::QpInit();

		QpThread *qp_main_thr = new QpThread(QP_PRIO_DFL, 0, "Main"); /* Main thread */
		if (!qp_main_thr)
			throw QpOutOfMemoryException();
		qp_main_thr->ThrInitFirst(&capability);
		qp_initialized = this;
		
		if (capability & QP_SIGNAL) {
#ifdef WIN32
			throw QpUnsupportedException();
#else
			qp_signal = new QpSignal;
			if (!qp_signal)
				throw QpOutOfMemoryException();
#endif
		}		
		if (capability & QP_TIMER) {
			qp_timer = new QpTimer;
			if (!qp_timer)
				throw QpOutOfMemoryException();
		}
	}
}

QpInit::~QpInit()
{
	if (qp_initialized == this) {
		if (qp_capability & QP_TIMER)
			delete qp_timer;
		if (qp_capability & QP_SIGNAL)
			delete qp_signal;
		delete QpThread::MainThread();
		QpThread::WaitForAllThreads();
		qp_initialized = NULL;
		
		if (qp_capability)
			QpSignal::QpDone();
	}
}

QpInit::QP_CAP QpInit::Capability(QP_CAP mask)
{
	return (QP_CAP) (qp_capability & mask);
}

int QpInit::DflSchedPolicy()
{
	return qp_sched_policy;
}

int QpInit::DflScope()
{
	return qp_scope;
}

void QpInit::AdjustSchedParam(unsigned int prio, struct sched_param *sch)
{
	if (prio > QP_PRIO_MAX)
		throw QpErrorException(QP_ERROR_ARG, "AdjustSchedParam");
	sch->sched_priority = qp_prio_table[prio];
}

