	.file	"test2.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	6
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	8
	.section	.rodata
.LC0:
	.string	"%d"
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
	
	movl	$3, a(%rip)
	
	movl	a(%rip), %eax
	addl	$5, %eax
	movl	%eax, b(%rip)	
	
	movl	a(%rip), %eax
	addl	$5, %eax
	movl	%eax, b(%rip)
	
	movl	a(%rip), %edx
	movl	b(%rip), %eax
	addl	%edx, %eax
	movl	%eax, a(%rip)
	
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
