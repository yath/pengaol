/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU Library General Public License version 2.
 * 
 *	Copyright (C) 1999 by kra
 *
 */

#include <qpthr/qp_spinlock.h>

void qp_arch_memory_barier(void);

/* Spinlock release; default is just set to zero.  */
void qp_arch_release(spinlock_t *spinlock)
{
	qp_arch_memory_barrier();
	*spinlock = 0;
}

