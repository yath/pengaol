/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */
#ifndef __QP_EXCEPT_H
#define __QP_EXCEPT_H

#include <string.h>
#if ! (defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x420))
#include <exception>
#endif
/*
 * 
 * Exceptions
 * 
 */
#define EXCEPTION_TEXTLEN	80

class QpException
#if ! (defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x420))
: public std::exception
#endif
{
    friend class exception;
    protected:
	int  e_code;
	char e_text[EXCEPTION_TEXTLEN];
    public:
	QpException(int code = 0, const char *text = NULL): e_code(code) {
		if (text) {
			strncpy(e_text, text, EXCEPTION_TEXTLEN);
			e_text[EXCEPTION_TEXTLEN - 1] = 0;
		} else
			e_text[0] = 0;
	}
	QpException(const QpException &e): e_code(e.e_code) {
		strcpy(e_text, e.e_text);
	}
	virtual ~QpException() throw() {} 
	
	QpException& operator = (const QpException &e) {
		e_code = e.e_code;
		strcpy(e_text, e.e_text);
		return *this;
	}
	int         ErrCode() const { return e_code;}
	const char *ErrText() const { return e_text;}
	virtual const char *ErrType() const = 0;
};

inline ostream& operator << (ostream& os, const QpException &e) {
	os << e.ErrType() << ":" << e.ErrCode() << ":" << e.ErrText();
	return os;
}

class QpTimeoutException: public QpException {
    public:
	QpTimeoutException(int code = 0, const char *text = NULL): QpException(code, text) {}
	virtual const char *ErrType() const { return "QpTimeoutException";}
};

class QpSysException: public QpException {
    public:
	QpSysException(int code = 0, const char *text = NULL): QpException(code, text) {}
	virtual const char *ErrType() const { return "QpSysException";}
};

class QpUnsupportedException: public QpException {
    public:
	QpUnsupportedException(int code = 0, const char *text = NULL): QpException(code, text) {}
	virtual const char *ErrType() const { return "QpUnsupportedException";}
};

class QpOutOfMemoryException: public QpException {
    public:
	QpOutOfMemoryException(int code = 0, const char *text = NULL): QpException(code, text) {}
	virtual const char *ErrType() const { return "QpOutOfMemoryException";}
};

#define QP_ERROR_STATE		1
#define QP_ERROR_ARG		2
#define QP_ERROR_DEADLOCK	3
#define QP_ERROR_DISABLED	4

class QpErrorException: public QpException {
    public:
	QpErrorException(int code = 0, const char *text = NULL): QpException(code, text) {}
	virtual const char *ErrType() const { return "QpErrorException";}
};

class QpClonedException: public QpException {
    public:
	QpClonedException(int code = 0, const char *text = NULL): QpException(code, text) {}
	virtual ~QpClonedException() throw() {}
	virtual void Raise() const = 0;
	virtual QpClonedException *Clone() const = 0;
};

class QpUserException: public QpClonedException {
    public:
	QpUserException(int code = 0, const char *text = NULL): QpClonedException(code, text) {}
	virtual ~QpUserException() throw() {}
	virtual void Raise() const { throw *this;}
	virtual QpClonedException *Clone() const { return new QpUserException(e_code, e_text);}
	virtual const char *ErrType() const { return "QpUserException";}
};

class QpCancelException: public QpClonedException {
    public:
	QpCancelException(int code = 0, const char *text = NULL): QpClonedException(code, text) {}
	virtual void Raise() const { throw *this;}
	virtual QpClonedException *Clone() const { return new QpCancelException(e_code, e_text);}
	virtual const char *ErrType() const { return "QpCancelException";}
};

#endif
