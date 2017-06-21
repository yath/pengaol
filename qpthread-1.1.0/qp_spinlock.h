#ifndef __QP_SPINLOCK_H
#define __QP_SPINLOCK_H

typedef int spinlock_t;

#if defined (__cplusplus)
extern "C" {
#endif
	
int qp_arch_testandset(spinlock_t *spinlock);
void qp_arch_release(spinlock_t *spinlock);

#if defined (__cplusplus)
}
#endif

#endif
