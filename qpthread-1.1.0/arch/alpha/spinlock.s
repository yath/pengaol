#ifdef __LANGUAGE_C__
# undef __LANGUAGE_C__
#endif

#ifndef __LANGUAGE_ASSEMBLY__
# define __LANGUAGE_ASSEMBLY__
#endif

#include <syscall.h>
#include <machine/regdef.h>
#include <machine/asm.h>
#include <machine/inst.h>
#include <machine/pal.h>


/*
  * qp_arch_testandset
  *
  * FUNCTIONAL DESCRIPTION:
  * 
  *	This routine performs an atomic test-and-set operation on the data
  *	at a specified address. If the address is locked (currently 1), then
  *	a value of 1 is returned. If the address is currently free (0) then
  *	a value of 1 is written, and the value 0 is returned. If the STL_C
  *	instructions fail (because the lock has been cleared), then the lock
  *	is retried up to some limit.
  *
  * FORMAL PARAMETERS:
  * 
  *	r16	Address of an int on which to perform test-and-set function
  * 
  * IMPLICIT INPUTS:
  *
  *	none
  * 
  * IMPLICIT OUTPUTS:
  * 
  *	none
  *
  * FUNCTION VALUE:
  * 
  *	r0 == 0 if word was previously clear, 1 if previously set.
  * 
  * SIDE EFFECTS:
  * 
  *	Sets 0(r16) to 1.
  */
    .align	4
LEAF (qp_arch_testandset)
$$10:	ldl_l	$0, ($16)	/*  Load flag value, and set CPU lock bit */
	blbs	$0, $$20	/*  If lock was set, return 1 */
	or	$0, 1, $1	/*  Set new lock value */
	stl_c	$1, ($16)	/*  Write it out */
	beq	$1, $$30	/*  If lock was already clear, retry */
	mb
$$20:
	RET			/*  Done! */
$$30:	br	$31, $$10	/*  For now, loop forever if necessary */
END(qp_arch_testandset)

    .align	4

 /*
  * qp_arch_memory_barrier
  *
  * FUNCTIONAL DESCRIPTION:
  * 
  *	This function causes a hardware memory barrier (only called on true
  *	MP hardware).
  *
  * FORMAL PARAMETERS:
  * 
  *	none
  * 
  * IMPLICIT INPUTS:
  * 
  *	none
  * 
  * IMPLICIT OUTPUTS:
  * 
  *	none
  *
  * FUNCTION VALUE:
  * 
  *	none
  * 
  * SIDE EFFECTS:
  * 
  *	none
  */
    .align	4
LEAF (qp_arch_memory_barrier)
	mb
	RET			/*  Done! */
END (qp_arch_memory_barrier)
