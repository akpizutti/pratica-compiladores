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

## TAC PRINT
DATATYPE = 0

## TAC END FUN
	leave
	ret

## DATA SECTION

