	.file	"test2.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	2
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	2
	.globl	c
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.long	7
	.globl	d
	.align 32
	.type	d, @object
	.size	d, 40
d:
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.long	9
	.globl	e
	.align 8
	.type	e, @object
	.size	e, 12
e:
	.long	7
	.long	21
	.long	43
	.globl	f
	.type	f, @object
	.size	f, 1
f:
	.byte	83
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp

	movzbl	f(%rip), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar@PLT
	
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
