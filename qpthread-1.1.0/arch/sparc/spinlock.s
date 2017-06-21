	.file	"spinlock_gnu.c"
gcc2_compiled.:
.section	".text"
	.align 4
	.global qp_arch_testandset
	.type	 qp_arch_testandset,#function
	.proc	04
qp_arch_testandset:
	!#PROLOGUE# 0
	save %sp,-120,%sp
	!#PROLOGUE# 1
	st %i0,[%fp+68]
	ld [%fp+68],%o0
	ld [%fp+68],%o1
	ldstub [%o0],%o2
	st %o2,[%fp-20]
	ld [%fp-20],%o0
	mov %o0,%i0
	b .LL1
	nop
.LL1:
	ret
	restore
.LLfe1:
	.size	 qp_arch_testandset,.LLfe1-qp_arch_testandset
	.align 4
	.global qp_arch_release
	.type	 qp_arch_release,#function
	.proc	020
qp_arch_release:
	!#PROLOGUE# 0
	save %sp,-112,%sp
	!#PROLOGUE# 1
	st %i0,[%fp+68]
	ld [%fp+68],%o0
	mov 0,%o1
	stbar; stb %o1,[%o0]
.LL2:
	ret
	restore
.LLfe2:
	.size	 qp_arch_release,.LLfe2-qp_arch_release
	.ident	"GCC: (GNU) 2.8.1"
