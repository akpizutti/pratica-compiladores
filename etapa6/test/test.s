	.file	"test.c"
	.text
	.section	.rodata
.LC4:
	.string	"STR"
.LC5:
	.string	"STR\t"
.LC6:
	.string	"\"\n\t"
.LC7:
	.string	"%d"
.LC8:
	.string	"%d \n"
.LC9:
	.string	"%c \n"
.LC10:
	.string	"%s"
.LC11:
	.string	"%s \n"
.LC12:
	.string	"%f"
.LC13:
	.string	"%f \n"
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
	subq	$112, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1, -100(%rbp)
	movb	$67, -101(%rbp)
	movabsq	$5717073776924706120, %rax
	movabsq	$6076276549873847378, %rdx
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movabsq	$4705510782929227329, %rax
	movabsq	$2325913809380987680, %rdx
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movabsq	$4705452650362980673, %rax
	movq	%rax, -32(%rbp)
	movl	$5394249, -24(%rbp)
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -96(%rbp)
	movl	$1, -92(%rbp)
	movl	$2, -88(%rbp)
	movl	$3, -84(%rbp)
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, -80(%rbp)
	movss	.LC2(%rip), %xmm0
	movss	%xmm0, -76(%rbp)
	movss	.LC3(%rip), %xmm0
	movss	%xmm0, -72(%rbp)
	movb	$65, -67(%rbp)
	movb	$66, -66(%rbp)
	movb	$67, -65(%rbp)
	movl	$53, %edi
	call	putchar@PLT
	movl	$65, %edi
	call	putchar@PLT
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	movl	$10, %edi
	call	putchar@PLT
	movl	$9, %edi
	call	putchar@PLT
	movl	$92, %edi
	call	putchar@PLT
	leaq	.LC6(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$55, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$55, %esi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-100(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-100(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movsbl	-101(%rbp), %eax
	movl	%eax, %edi
	call	putchar@PLT
	movsbl	-101(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC9(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-64(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC10(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-64(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	
	cvtss2sd	-96(%rbp), %xmm0
	leaq	.LC12(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	cvtss2sd	-96(%rbp), %xmm0
	leaq	.LC13(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	movl	-88(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	movl	-88(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	movss	-76(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.LC12(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	movss	-76(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.LC13(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	movzbl	-66(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar@PLT
	
	movzbl	-66(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	movl	$0, %eax
	
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1083179008
	.align 4
.LC1:
	.long	1067030938
	.align 4
.LC2:
	.long	1075419546
	.align 4
.LC3:
	.long	1080452710
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
