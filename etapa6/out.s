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

	## TAC EQ
	movl	_10(%rip), %edx
	movl	_10(%rip), %eax
	cmpl	%eax, %edx
	sete	%al
	movzbl	%al, _temp0(%rip)

	## TAC JZ
	jz   .label0

	## TAC PRINT
	leaq	_str7(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label0:

	## TAC DIF
	movl	_10(%rip), %edx
	movl	_5(%rip), %eax
	cmpl	%eax, %edx
	setne	%al
	movzbl	%al, _temp1(%rip)

	## TAC JZ
	jz   .label1

	## TAC PRINT
	leaq	_str9(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label1:

	## TAC GEQ
	movl	_10(%rip), %edx
	movl	_10(%rip), %eax
	cmpl	%eax, %edx
	setge	%al
	movzbl	%al, _temp2(%rip)

	## TAC JZ
	jz   .label2

	## TAC PRINT
	leaq	_str10(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label2:

	## TAC LEQ
	movl	_10(%rip), %edx
	movl	_10(%rip), %eax
	cmpl	%eax, %edx
	setle	%al
	movzbl	%al, _temp3(%rip)

	## TAC JZ
	jz   .label3

	## TAC PRINT
	leaq	_str11(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label3:

	## TAC LES
	movl	_10(%rip), %edx
	movl	_5(%rip), %eax
	cmpl	%eax, %edx
	setl	%al
	movzbl	%al, _temp4(%rip)

	## TAC JZ
	jz   .label4

	## TAC PRINT
	leaq	_str12(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label4:

	## TAC GRT
	movl	_5(%rip), %edx
	movl	_10(%rip), %eax
	cmpl	%eax, %edx
	setg	%al
	movzbl	%al, _temp5(%rip)

	## TAC JZ
	jz   .label5

	## TAC PRINT
	leaq	_str13(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label5:

	## TAC AND
	movl	_10(%rip), %eax
	movl	_10(%rip), %edx
	andl	%eax, %edx
	movl	%edx, _temp6(%rip)

	## TAC JZ
	jz   .label6

	## TAC PRINT
	leaq	_str14(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label6:

	## TAC OR
	movl	_10(%rip), %eax
	movl	_10(%rip), %edx
	orl	%eax, %edx
	movl	%edx, _temp7(%rip)

	## TAC JZ
	jz   .label7

	## TAC PRINT
	leaq	_str15(%rip), %rsi
	leaq	printStr(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

	## TAC LABEL
.label7:

	## TAC END FUN
	leave
	ret

## DATA SECTION
	.data
	.align	4
_a:	.long	10
_b:	.long	3
_c:	.long	0
_temp7:	.zero	4
_0:	.int	0
_3:	.int	3
_5:	.int	5
_str15:	.string	"Deu bom 8\n"
_temp2:	.zero	4
_str12:	.string	"Deu bom 5\n"
_str9:	.string	"Deu bom 2\n"
_temp5:	.zero	4
_temp0:	.zero	4
_10:	.int	10
_str13:	.string	"Deu bom 6\n"
_temp3:	.zero	4
_str10:	.string	"Deu bom 3\n"
_temp6:	.zero	4
_temp1:	.zero	4
_str14:	.string	"Deu bom 7\n"
_str11:	.string	"Deu bom 4\n"
_temp4:	.zero	4
_str7:	.string	"Deu bom 1\n"

