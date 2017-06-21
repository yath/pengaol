/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_WORK_H
#define __QP_WORK_H

/*
 * 
 * Jobs executing procedures
 * 
 */
class QpWork_p0: public QpRunnable {
    public:
	typedef void (*PR0)();
	
	QpWork_p0(PR0 pr0, const char *name = NULL):
        	QpRunnable(name), m_pr0(pr0) {}

    private:
	PR0 m_pr0;
	virtual void Main() {m_pr0();}
};

template <class P1>
class QpWork_p1: public QpRunnable {
    public:
	typedef void (*PR1)(const P1);
	
	QpWork_p1(PR1 pr1, P1 p1, const char *name = NULL):
        	QpRunnable(name), m_pr1(pr1), m_p1(p1) {}

    private:
	PR1 m_pr1;
	P1  m_p1;
	virtual void Main() {m_pr1(m_p1);}

};

template <class P1, class P2>
class QpWork_p2 : public QpRunnable {
    public:
	typedef void (*PR2)(const P1, const P2);

	QpWork_p2(PR2 pr2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_pr2(pr2), m_p1(p1), m_p2(p2) {}

    private:
	PR2 m_pr2;
	P1  m_p1;
	P2  m_p2;
	virtual void Main() {m_pr2(m_p1,m_p2);}
}; 

template <class P1, class P2, class P3>
class QpWork_p3: public QpRunnable {
    public:
	typedef void (*PR3)(P1, P2, P3);

	QpWork_p3(PR3 pr3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_pr3(pr3), m_p1(p1), m_p2(p2), m_p3(p3) {}

    private:
	PR3 m_pr3;
	P1  m_p1;
	P2  m_p2;
	P3  m_p3;
	virtual void Main() {m_pr3(m_p1,m_p2,m_p3);}
}; 

template <class P1, class P2, class P3, class P4>
class QpWork_p4: public QpRunnable {
    public:
	typedef void (*PR4)(P1, P2, P3, P4);

	QpWork_p4(PR4 pr4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_pr4(pr4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}

    private:
	PR4 m_pr4;
	P1  m_p1;
	P2  m_p2;
	P3  m_p3;
	P4  m_p4;
	virtual void Main() {m_pr4(m_p1,m_p2,m_p3,m_p4);}
};

template <class P1, class P2, class P3, class P4, class P5>
class QpWork_p5: public QpRunnable {
    public:
	typedef void (*PR5)(P1, P2, P3, P4, P5);

	QpWork_p5(PR5 pr5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_pr5(pr5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}

    private:
	PR5 m_pr5;
	P1  m_p1;
	P2  m_p2;
	P3  m_p3;
	P4  m_p4;
	P5  m_p5;
	virtual void Main() {m_pr5(m_p1, m_p2, m_p3, m_p4, m_p5);}
};

/*
 * 
 * Jobs executing functions
 * 
 */
template <class T>
class QpWork_f0: public QpRunnable {
    public:
	typedef T (*FN0)();

	QpWork_f0(FN0 fn0, const char *name = NULL):
        	QpRunnable(name), m_fn0(fn0) {}
	T& Result() {return m_result;}

    private:
	T   m_result;
	FN0 m_fn0;
	virtual void Main() {m_result = m_fn0();}
};

template <class T, class P1>
class QpWork_f1: public QpRunnable {
    public:
	typedef T (*FN1)(P1);

	QpWork_f1(FN1 fn1, P1 p1, const char *name = NULL):
	        QpRunnable(name), m_fn1(fn1), m_p1(p1) {}
	T& Result() {return m_result;}

    private:
	T   m_result;
	FN1 m_fn1;
	P1  m_p1;
	virtual void Main() {m_result = m_fn1(m_p1);}

};

template <class T, class P1, class P2>
class QpWork_f2 : public QpRunnable {
    public:
	typedef T (*FN2)(P1, P2);
	
	QpWork_f2(FN2 fn2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_fn2(fn2), m_p1(p1), m_p2(p2) {}
	T& Result() {return m_result;}

    private:
	virtual void Main() {m_result = m_fn2(m_p1,m_p2);}
	T   m_result;
	FN2 m_fn2;
	P1  m_p1;
	P2  m_p2;
};

template <class T, class P1, class P2, class P3>
class QpWork_f3: public QpRunnable {
    public:
	typedef T (*FN3)(P1, P2, P3);
	
	QpWork_f3(FN3 fn3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_fn3(fn3), m_p1(p1), m_p2(p2), m_p3(p3) {}
	T& Result() {return m_result;}

    private:
	T   m_result;
	FN3 m_fn3;
	P1  m_p1;
	P2  m_p2;
	P3  m_p3;
	virtual void Main() {m_result = m_fn3(m_p1,m_p2,m_p3);}
};

template <class T, class P1, class P2, class P3, class P4>
class QpWork_f4: public QpRunnable {
    public:
	typedef T (*FN4)(P1, P2, P3, P4);

	QpWork_f4(FN4 fn4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_fn4(fn4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
	T& Result() {return m_result;}

    private:
	T   m_result;
	FN4 m_fn4;
	P1  m_p1;
	P2  m_p2;
	P3  m_p3;
	P4  m_p4;
	virtual void Main() {m_result = m_fn4(m_p1,m_p2,m_p3,m_p4);}
};

template <class T, class P1, class P2, class P3, class P4, class P5>
class QpWork_f5: public QpRunnable {
    public:
	typedef T (*FN5)(P1, P2, P3, P4, P5);

	QpWork_f5(FN5 fn5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_fn5(fn5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}
	T& Result() {return m_result;}

    private:
	T   m_result;
	FN5 m_fn5;
	P1  m_p1;
	P2  m_p2;
	P3  m_p3;
	P4  m_p4;
	P5  m_p5;
	virtual void Main() {m_result = m_fn5(m_p1, m_p2, m_p3, m_p4, m_p5);}
};

/*
 * 
 * Jobs executing member procedures
 * 
 */
template <class C>
class QpWork_mp0: public QpRunnable {
    public:
	typedef void (C::*MPR0)();

	QpWork_mp0(const C& obj, MPR0 mpr0, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mpr0(mpr0) {}
	QpWork_mp0(const C *obj, MPR0 mpr0, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mpr0(mpr0) {}

    private:
	C    *m_obj;
	MPR0 m_mpr0;
	virtual void Main() {(m_obj->*m_mpr0)();}
};

template <class C, class P1>
class QpWork_mp1: public QpRunnable {
    public:
	typedef void (C::*MPR1)(P1);

	QpWork_mp1(const C& obj, MPR1 mpr1, P1 p1, const char *name = NULL):
	        QpRunnable(name), m_obj(&obj), m_mpr1(mpr1), m_p1(p1) {}
	QpWork_mp1(const C *obj, MPR1 mpr1, P1 p1, const char *name = NULL):
	        QpRunnable(name), m_obj(obj), m_mpr1(mpr1), m_p1(p1) {}
    private:
	C    *m_obj;
	MPR1 m_mpr1;
	P1   m_p1;
	virtual void Main() {(m_obj->*m_mpr1)(m_p1);}
}; 


template <class C, class P1, class P2>
class QpWork_mp2: public QpRunnable {
    public:
	typedef void (C::*MPR2)(P1, P2);

	QpWork_mp2(const C& obj, MPR2 mpr2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj),m_mpr2(mpr2), m_p1(p1), m_p2(p2) {}
	QpWork_mp2(const C *obj, MPR2 mpr2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(obj),m_mpr2(mpr2), m_p1(p1), m_p2(p2) {}

    private:
	C    *m_obj;
	MPR2 m_mpr2;
	P1   m_p1;
	P2   m_p2;
	virtual void Main() {(m_obj->*m_mpr2)(m_p1,m_p2);}
};

template <class C, class P1, class P2, class P3>
class QpWork_mp3: public QpRunnable {
    public:
	typedef void (C::*MPR3)(P1, P2, P3);

	QpWork_mp3( const C& obj, MPR3 mpr3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
	        QpRunnable(name), m_obj(&obj),m_mpr3(mpr3), m_p1(p1), m_p2(p2), m_p3(p3) {}
	QpWork_mp3( const C *obj, MPR3 mpr3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
	        QpRunnable(name), m_obj(obj),m_mpr3(mpr3), m_p1(p1), m_p2(p2), m_p3(p3) {}

    private:
	C    *m_obj;
	MPR3 m_mpr3;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	virtual void Main() {(m_obj->*m_mpr3)(m_p1,m_p2,m_p3);}
};

template <class C, class P1, class P2, class P3, class P4>
class QpWork_mp4: public QpRunnable {
    public:
	typedef void (C::*MPR4)(P1, P2, P3, P4);

	QpWork_mp4(const C& obj, MPR4 mpr4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mpr4(mpr4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
	QpWork_mp4(const C *obj, MPR4 mpr4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mpr4(mpr4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}

    private:
	C    *m_obj;
	MPR4 m_mpr4;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	virtual void Main() {(m_obj->*m_mpr4)(m_p1,m_p2,m_p3,m_p4);}
};

template <class C, class P1, class P2, class P3, class P4, class P5>
class QpWork_mp5: public QpRunnable {
    public:
	typedef void (C::*MPR5)(P1, P2, P3, P4, P5);

	QpWork_mp5(const C& obj, MPR5 mpr5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mpr5(mpr5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}
	QpWork_mp5(const C *obj, MPR5 mpr5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mpr5(mpr5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}

    private:
	C    *m_obj;
	MPR5 m_mpr5;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	P5   m_p5;
	virtual void Main() {(m_obj->*m_mpr5)(m_p1, m_p2, m_p3, m_p4, m_p5);}
};

/*
 * 
 * Jobs executing member functions
 * 
 */
template <class T, class C>
class QpWork_mf0: public QpRunnable {
    public:
	typedef T (C::*MFN0)();

	QpWork_mf0(const C& obj, MFN0 mfn0, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn0(mfn0) {}
	QpWork_mf0(const C *obj, MFN0 mfn0, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn0(mfn0) {}
	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN0 m_mfn0;
	virtual void Main() {m_result = (m_obj->*m_mfn0)();}
};

template <class T, class C, class P1>
class QpWork_mf1: public QpRunnable {
    public:
	typedef T (C::*MFN1)(P1);

	QpWork_mf1(const C& obj, MFN1 mfn1, P1 p1, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn1(mfn1), m_p1(p1) {}
	QpWork_mf1(const C *obj, MFN1 mfn1, P1 p1, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn1(mfn1), m_p1(p1) {}
	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN1 m_mfn1;
	P1   m_p1;
	virtual void Main() {m_result = (m_obj->*m_mfn1)(m_p1);}
};


template <class T, class C, class P1, class P2>
class QpWork_mf2: public QpRunnable {
    public:
	typedef T (C::*MFN2)(P1, P2);

    public:
	QpWork_mf2(const C& obj, MFN2 mfn2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj),m_mfn2(mfn2), m_p1(p1), m_p2(p2) {}
	QpWork_mf2(const C *obj, MFN2 mfn2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(obj),m_mfn2(mfn2), m_p1(p1), m_p2(p2) {}
	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN2 m_mfn2;
	P1   m_p1;
	P2   m_p2;
	virtual void Main() {m_result = (m_obj->*m_mfn2)(m_p1,m_p2);}
};

template <class T, class C, class P1, class P2, class P3>
class QpWork_mf3: public QpRunnable {
    public:
	typedef T (C::*MFN3)(P1, P2, P3);
	
	QpWork_mf3(const C& obj, MFN3 mfn3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn3(mfn3), m_p1(p1), m_p2(p2), m_p3(p3) {}
	QpWork_mf3(const C *obj, MFN3 mfn3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn3(mfn3), m_p1(p1), m_p2(p2), m_p3(p3) {}

	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN3 m_mfn3;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	virtual void Main() {m_result = (m_obj->*m_mfn3)(m_p1,m_p2,m_p3);}
};

template <class T, class C, class P1, class P2, class P3, class P4>
class QpWork_mf4: public QpRunnable {
    public:
	typedef T (C::*MFN4)(P1, P2, P3, P4);

	QpWork_mf4(const C& obj, MFN4 mfn4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn4(mfn4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
	QpWork_mf4(const C *obj, MFN4 mfn4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn4(mfn4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}

	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN4 m_mfn4;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	virtual void Main() {m_result = (m_obj->*m_mfn4)(m_p1,m_p2,m_p3,m_p4);}
};

template <class T, class C, class P1, class P2, class P3, class P4, class P5>
class QpWork_mf5: public QpRunnable {
    public:
	typedef T (C::*MFN5)(P1, P2, P3, P4, P5);

	QpWork_mf5(const C& obj, MFN5 mfn5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn5(mfn5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}
	QpWork_mf5(const C *obj, MFN5 mfn5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn5(mfn5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}

	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN5 m_mfn5;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	P5   m_p5;
	virtual void Main() {m_result = (m_obj->*m_mfn5)(m_p1, m_p2, m_p3, m_p4, m_p5);}
};

/*
 * 
 * Jobs executing const member procedures
 * 
 */
template <class C>
class QpWork_mcp0: public QpRunnable {
    public:
	typedef void (C::*MPN0)() const;

    public:
	QpWork_mcp0(const C& obj, MPN0 mpn0, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mpn0(mpn0) {}
	QpWork_mcp0(const C *obj, MPN0 mpn0, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mpn0(mpn0) {}

    private:
	const C *m_obj;
	MPN0    m_mpn0;
	virtual void Main() {(m_obj->*m_mpn0)();}
};

template <class C, class P1>
class QpWork_mcp1: public QpRunnable {
    public:
	typedef void (C::*MPN1)(P1) const;

    public:
	QpWork_mcp1(const C& obj, MPN1 mpn1, P1 p1, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mpn1(mpn1), m_p1(p1) {}
	QpWork_mcp1(const C *obj, MPN1 mpn1, P1 p1, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mpn1(mpn1), m_p1(p1) {}

    private:
	const C *m_obj;
	MPN1    m_mpn1;
	P1      m_p1;
	virtual void Main() {(m_obj->*m_mpn1)(m_p1);}
};

template <class C, class P1, class P2>
class QpWork_mcp2: public QpRunnable {
    public:
	typedef void (C::*MPN2)(P1, P2) const;

	QpWork_mcp2(const C& obj, MPN2 mpn2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mpn2(mpn2), m_p1(p1), m_p2(p2) {}
	QpWork_mcp2(const C *obj, MPN2 mpn2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mpn2(mpn2), m_p1(p1), m_p2(p2) {}

    private:
	C    *m_obj;
	MPN2 m_mpn2;
	P1   m_p1;
	P2   m_p2;
	virtual void Main() {(m_obj->*m_mpn2)(m_p1, m_p2);}
};

template <class C, class P1, class P2, class P3>
class QpWork_mcp3: public QpRunnable {
    public:
	typedef void (C::*MPN3)(P1, P2, P3) const;

	QpWork_mcp3(const C& obj, MPN3 mpn3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mpn3(mpn3), m_p1(p1), m_p2(p2), m_p3(p3) {}
	QpWork_mcp3(const C *obj, MPN3 mpn3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mpn3(mpn3), m_p1(p1), m_p2(p2), m_p3(p3) {}

    private:
	C    *m_obj;
	MPN3 m_mpn3;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	virtual void Main() {(m_obj->*m_mpn3)(m_p1, m_p2, m_p3);}
};

template <class C, class P1, class P2, class P3, class P4>
class QpWork_mcp4: public QpRunnable {
    public:
	typedef void (C::*MPN4)(P1, P2, P3, P4) const;

	QpWork_mcp4(const C& obj, MPN4 mpn4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj),m_mpn4(mpn4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
	QpWork_mcp4(const C *obj, MPN4 mpn4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(obj),m_mpn4(mpn4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
	
    private:
	C    *m_obj;
	MPN4 m_mpn4;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	virtual void Main() {(m_obj->*m_mpn4)(m_p1, m_p2, m_p3, m_p4);}
};

template <class C, class P1, class P2, class P3, class P4, class P5>
class QpWork_mcp5: public QpRunnable {
    public:
	typedef void (C::*MPN5)(P1, P2, P3, P4, P5) const;

	QpWork_mcp5(const C& obj, MPN5 mpn5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj),m_mpn5(mpn5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}
	QpWork_mcp5(const C *obj, MPN5 mpn5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(obj),m_mpn5(mpn5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}
	
    private:
	C    *m_obj;
	MPN5 m_mpn5;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	P5   m_p5;
	virtual void Main() {(m_obj->*m_mpn5)(m_p1, m_p2, m_p3, m_p4, m_p5);}
};

/*
 * 
 * Jobs executing const member functions
 * 
 */
template <class T, class C>
class QpWork_mcf0: public QpRunnable {
    public:
	typedef T (C::*MFN0)() const;

    public:
	QpWork_mcf0(const C& obj, MFN0 mfn0, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn0(mfn0) {}
	QpWork_mcf0(const C *obj, MFN0 mfn0, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn0(mfn0) {}

	T& Result() {return m_result;}

    private:
	T       m_result;
	const C *m_obj;
	MFN0    m_mfn0;
	virtual void Main() {m_result = (m_obj->*m_mfn0)();}
};

template <class T, class C, class P1>
class QpWork_mcf1: public QpRunnable {
    public:
	typedef T (C::*MFN1)(P1) const;

    public:
	QpWork_mcf1(const C& obj, MFN1 mfn1, P1 p1, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn1(mfn1), m_p1(p1) {}
	QpWork_mcf1(const C *obj, MFN1 mfn1, P1 p1, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn1(mfn1), m_p1(p1) {}

	T& Result() {return m_result;}

    private:
	T       m_result;
	const C *m_obj;
	MFN1    m_mfn1;
	P1      m_p1;
	virtual void Main() {m_result = (m_obj->*m_mfn1)(m_p1);}
};

template <class T, class C, class P1, class P2>
class QpWork_mcf2 : public QpRunnable {
    public:
	typedef T (C::*MFN2)(P1, P2) const;

	QpWork_mcf2(const C& obj, MFN2 mfn2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn2(mfn2), m_p1(p1), m_p2(p2) {}
	QpWork_mcf2(const C *obj, MFN2 mfn2, P1 p1, P2 p2, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn2(mfn2), m_p1(p1), m_p2(p2) {}

	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN2 m_mfn2;
	P1   m_p1;
	P2   m_p2;
	virtual void Main() {m_result = (m_obj->*m_mfn2)(m_p1, m_p2);}
};

template <class T, class C, class P1, class P2, class P3>
class QpWork_mcf3: public QpRunnable {
    public:
	typedef T (C::*MFN3)(P1, P2, P3) const;

	QpWork_mcf3(const C& obj, MFN3 mfn3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj), m_mfn3(mfn3), m_p1(p1), m_p2(p2), m_p3(p3) {}
	QpWork_mcf3(const C *obj, MFN3 mfn3, P1 p1, P2 p2, P3 p3, const char *name = NULL):
        	QpRunnable(name), m_obj(obj), m_mfn3(mfn3), m_p1(p1), m_p2(p2), m_p3(p3) {}

	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN3 m_mfn3;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	virtual void Main() {m_result = (m_obj->*m_mfn3)(m_p1, m_p2, m_p3);}
};

template <class T, class C, class P1, class P2, class P3, class P4>
class QpWork_mcf4: public QpRunnable {
    public:
	typedef T (C::*MFN4)(P1, P2, P3, P4) const;

	QpWork_mcf4(const C& obj, MFN4 mfn4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj),m_mfn4(mfn4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}
	QpWork_mcf4(const C *obj, MFN4 mfn4, P1 p1, P2 p2, P3 p3, P4 p4, const char *name = NULL):
        	QpRunnable(name), m_obj(obj),m_mfn4(mfn4), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {}

	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN4 m_mfn4;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	virtual void Main() {m_result = (m_obj->*m_mfn4)(m_p1, m_p2, m_p3, m_p4);}
};

template <class T, class C, class P1, class P2, class P3, class P4, class P5>
class QpWork_mcf5: public QpRunnable {
    public:
	typedef T (C::*MFN5)(P1, P2, P3, P4, P5) const;

	QpWork_mcf5(const C& obj, MFN5 mfn5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(&obj),m_mfn5(mfn5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}
	QpWork_mcf5(const C *obj, MFN5 mfn5, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, const char *name = NULL):
        	QpRunnable(name), m_obj(obj),m_mfn5(mfn5), m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4), m_p5(p5) {}

	T& Result() {return m_result;}

    private:
	T    m_result;
	C    *m_obj;
	MFN5 m_mfn5;
	P1   m_p1;
	P2   m_p2;
	P3   m_p3;
	P4   m_p4;
	P5   m_p5;
	virtual void Main() {m_result = (m_obj->*m_mfn5)(m_p1, m_p2, m_p3, m_p4, m_p5);}
};

#endif

