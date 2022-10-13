	.file	"test2.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.int	6
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.float	8.5
	.globl	c
	.type	c, @object
	.size	c, 1
c:
	.byte	97
	.section	.rodata
.LC1:
	.string	"%d"
.LC2:
	.string	"%f"
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
	.cfi_def_cfa_register 6
	movl	a(%rip), %eax
	addl	$2, %eax
	movl	%eax, a(%rip)
	movss	b(%rip), %xmm1
	movss	.LC0(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, b(%rip)
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movss	b(%rip), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.LC2(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movzbl	c(%rip), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar@PLT
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1073741824
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
