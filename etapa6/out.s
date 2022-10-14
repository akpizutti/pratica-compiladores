## INIT
printInt:
	.string	"%d"
printFlt:
	.string	"%f"
printStr:
	.string	"%s"

	## TAC BEGIN FUN
	.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp

	## TAC COPY
	movl	_0(%rip), %eax
	movl	%eax, _a(%rip)

	## TAC SUB
	movl	_a(%rip), %edx
	movl	_i(%rip), %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, _temp0(%rip)

	## TAC COPY
	movl	_temp0(%rip), %eax
	movl	%eax, _a(%rip)

	## TAC COPY
	movl	_5(%rip), %eax
	movl	%eax, _a(%rip)

	## TAC COPY
	movl	_a(%rip), %eax
	movl	_55(%rip), %edx
	cltq
	leaq	0(,%rax,4), %rcx
	leaq	_v(%rip), %rax
	movl	%edx, (%rcx,%rax)

	## TAC ARRAY ACC
	movl	_5(%rip), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	_v(%rip), %rax
	movl	(%rdx,%rax), %eax
	movl	%eax, _temp1(%rip)

	## TAC PRINT
	movl	_temp1(%rip), %eax
	movl	%eax, %esi
	leaq	printInt(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC PRINT
	movl	_a(%rip), %eax
	movl	%eax, %esi
	leaq	printInt(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC COPY
	movl	_2(%rip), %eax
	movl	%eax, _i(%rip)

	## TAC GRT
	movl	_1(%rip), %edx
	movl	_2(%rip), %eax
	cmpl	%edx, %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, _temp2(%rip)

	## TAC EQ
	movl	_3(%rip), %edx
	movl	_3(%rip), %eax
	cmpl	%edx, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, _temp3(%rip)

	## TAC AND
	movl	_temp2(%rip), %eax
	movl	_temp3(%rip), %edx
	andl	%eax, %edx
	movl	%edx, _temp4(%rip)

	## TAC JZ
	movl    $0, %edx
	movl    _temp4(%rip), %eax
	cmpl    %eax, %edx
	jz   .label0

	## TAC LABEL
.label0:

	## TAC PRINT
	leaq	_str21(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC PRINT
	leaq	_str22(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC EQ
	movl	_a(%rip), %edx
	movl	_15(%rip), %eax
	cmpl	%edx, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, _temp5(%rip)

	## TAC DIF
	movl	_a(%rip), %edx
	movl	_7(%rip), %eax
	cmpl	%edx, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, _temp6(%rip)

	## TAC AND
	movl	_temp5(%rip), %eax
	movl	_temp6(%rip), %edx
	andl	%eax, %edx
	movl	%edx, _temp7(%rip)

	## TAC JZ
	movl    $0, %edx
	movl    _temp7(%rip), %eax
	cmpl    %eax, %edx
	jz   .label1

	## TAC ADD
	movl	_a(%rip), %edx
	movl	_1(%rip), %eax
	addl	%edx, %eax
	movl	%eax, _temp8(%rip)

	## TAC COPY
	movl	_temp8(%rip), %eax
	movl	%eax, _a(%rip)

	## TAC PRINT
	leaq	_str25(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label1:

	## TAC EQ
	movl	_i(%rip), %edx
	movl	_100(%rip), %eax
	cmpl	%edx, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, _temp9(%rip)

	## TAC JZ
	movl    $0, %edx
	movl    _temp9(%rip), %eax
	cmpl    %eax, %edx
	jz   .label2

	## TAC PRINT
	leaq	_str26(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC JMP
	jmp   .label3

	## TAC LABEL
.label2:

	## TAC PRINT
	leaq	_str27(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label3:

	## TAC END FUN
	leave
	ret

	## TAC BEGIN FUN
	.globl	incn
incn:
	pushq	%rbp
	movq	%rsp, %rbp

	## TAC ADD
	movl	_x(%rip), %edx
	movl	_n(%rip), %eax
	addl	%edx, %eax
	movl	%eax, _temp10(%rip)

	## TAC RETURN
	movl	_temp10(%rip), %eax

	## TAC END FUN
	leave
	ret

## DATA SECTION
	.data
	.align	4
_c:	.byte	120
_d:	.byte	48
_a:	.int	'A'
_i:	.int	1
_v:	.long	'a'
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
_f:	.float	2.5
_k:	.int	0
_temp7:	.zero	4
_0:	.int	0
_1:	.int	1
_2:	.int	2
_3:	.int	3
_4:	.int	4
_5:	.int	5
_6:	.int	6
_7:	.int	7
_8:	.int	8
_9:	.int	9
_str25:	.string	"A era=15\n"
_temp2:	.zero	4
_str22:	.string	"Digite mais um numero: \n"
_temp5:	.zero	4
_temp0:	.zero	4
_2.5:	.float	2.5
_str27:	.string	"OK!\n"
_temp8:	.zero	4
_10:	.int	10
_temp3:	.zero	4
_100:	.int	100
_temp6:	.zero	4
_15:	.int	15
_str21:	.string	"Digite um numero: \n"
_temp1:	.zero	4
_'A':	.byte	65
_temp9:	.zero	4
_'x':	.byte	120
_'a':	.byte	97
_str26:	.string	"Nao tem como isso...\n"
_temp10:	.zero	4
_55:	.int	55
_temp4:	.zero	4

