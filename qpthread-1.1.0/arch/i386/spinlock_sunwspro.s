
	.section	.text,"ax" 

/
/------ qp_arch_testadnset
/
	.align	4
	.globl	qp_arch_testandset
	.type	qp_arch_testandset,@function
	.align	16
	.volatile
qp_arch_testandset:
	pushl   %ebx
	movl	8(%esp), %ebx
	movl    $1, %eax
	xchgl   %eax, (%ebx)
	popl	%ebx
	ret
	.size	qp_arch_testandset,.-qp_arch_testandset

/
/------ qp_arch_release
/
	.align	4
	.globl	qp_arch_release
	.type	qp_arch_release,@function
	.align	16
	.volatile
qp_arch_release:
	movl	4(%esp), %eax
	movl	$0, (%eax)
	ret
	.size	qp_arch_release,.-qp_arch_release




	.file	"qp_sig_sunwspro_i386.s"
	.ident	"(C) 1999 by kra"

