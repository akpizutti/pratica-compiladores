printInt:
	.string	"%d"
printFlt:
	.string	"%f"
printStr:
	.string	"%s"

	.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp

	movl	_99(%rip), %eax
	movl	%eax, %esi
	leaq	printInt(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	nop
	popq	%rbp
	ret

varName: .long	69
_99: .long 57
