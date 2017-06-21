/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_MISC_H
#define __QP_MISC_H

/*
 * 
 * QpOnce
 * 
 */
class QpOnce: public QpBase {
    private:
	enum O_STAT {O_DOIT = 0, O_RUNNING = 1, O_DONE = 2};
	
	QpMonitor 	  o_monitor;
	O_STAT 	  	  o_stat;
	QpRunnable 	  *o_runnable;
	QpClonedException *o_except;
    protected:
	virtual void Main();
    public:
	QpOnce(QpRunnable *r = NULL, const char *name = NULL);
	virtual ~QpOnce();
	void Do();
	
	void Raise();
    private:
	QpOnce(const QpOnce&);
	QpOnce& operator = (const QpOnce&);
};

/*
 * 
 * QpTSD - Thread Specific Data
 * 
 */
class QpTSD: public QpBase {
    private:
	pthread_key_t ts_key;
    public:
	typedef void (*tsd_cleanup_func)(void *);
	
	QpTSD(const char *name = NULL, tsd_cleanup_func = NULL);
	virtual ~QpTSD();
	
	void *Get();
	void Set(void *arg);
    private:
	QpTSD(const QpTSD&);
	QpTSD& operator = (const QpTSD&);
};

/*
 * 
 * QpTSDT - Thread Specific Data template
 * 
 */
template <class T>
class QpTSDT: public QpTSD {
    private:
	static void tsdt_cleanup(void *arg);
    public:
	QpTSDT(const char *name = NULL);
	virtual ~QpTSDT();
	
	T *operator -> ();
    private:
	QpTSDT(const QpTSDT&);
	QpTSDT& operator = (const QpTSDT&);
};


template <class T>
QpTSDT<T>::QpTSDT(const char *name): QpTSD(name, tsdt_cleanup)
{
}

template <class T>
QpTSDT<T>::~QpTSDT()
{
}

template <class T>
T *QpTSDT<T>::operator -> ()
{
	T *val;

	if (!(val = (T *) Get())) {
		if (!(val = new T))
			throw QpOutOfMemoryException();
		Set(val);
	}
	return val;
}

template <class T>
void QpTSDT<T>::tsdt_cleanup(void *arg)
{
	delete (T *) arg;
}


#endif

