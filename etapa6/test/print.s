.LC0:
	.string	"STR"
.LC1:
	.string	"STR\t"
.LC2:
	.string	"\"\n\t"
.LC3:
	.string	"%d"
.LC4:
	.string	"%d \n"
.LC5:
	.string	"%c \n"
.LC6:
	.string	"%s"
.LC7:
	.string	"%s \n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
    #int main(){
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	
	#int var1 = 1;
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1, -28(%rbp)
	
	#char var2 = 'C';
	movb	$67, -29(%rbp)
	
	#char var3[] = "HELLO WORLD!";
	movabsq	$5717073776924706120, %rax
	movq	%rax, -21(%rbp)
	movl	$558124114, -13(%rbp)
	movb	$0, -9(%rbp)
	
	#float var4 = 4.5;
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -52(%rbp)
	
	#int var5[] = {1, 2, 3};
	movl	$1, -48(%rbp)
	movl	$2, -44(%rbp)
	movl	$3, -40(%rbp)
	
	#float var6[] = {1.2, 2.4, 3.6};
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, -36(%rbp)
	movss	.LC2(%rip), %xmm0
	movss	%xmm0, -32(%rbp)
	movss	.LC3(%rip), %xmm0
	movss	%xmm0, -28(%rbp)
	
	#char var7[] = {'A', 'B', 'C'};
	movb	$65, -24(%rbp)
	movb	$66, -23(%rbp)
	movb	$67, -22(%rbp)
	
	
	#printf("5");
	movl	$53, %edi
	call	putchar@PLT
	
	#printf("A");
	movl	$65, %edi
	call	putchar@PLT
	
	#printf("STR");
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("STR\t");
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("STR\n");
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	
	#printf("\n");
	movl	$10, %edi
	call	putchar@PLT
	
	#printf("\t");
	movl	$9, %edi
	call	putchar@PLT
	
	#printf("\\");
	movl	$92, %edi
	call	putchar@PLT
	
	#printf("\"\n\t");
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%d", 55);
	movl	$55, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%d \n", 55);
	movl	$55, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%d", var1);
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%d \n", var1);
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%c", var2);
	movsbl	-29(%rbp), %eax
	movl	%eax, %edi
	call	putchar@PLT
	
	#printf("%c \n", var2);
	movsbl	-29(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%s", var3);
	leaq	-21(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%s \n", var3);
	leaq	-21(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%f", var4);
	cvtss2sd	-96(%rbp), %xmm0
	leaq	.LC12(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	#printf("%f \n", var4);
	cvtss2sd	-96(%rbp), %xmm0
	leaq	.LC13(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	#printf("%d", var5[1]);
	movl	-88(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%d \n", var5[1]);
	movl	-88(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#printf("%f", var6[1]);
	movss	-76(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.LC12(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	#printf("%f \n", var6[1]);
	movss	-76(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.LC13(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	
	#printf("%c", var7[1]);
	movzbl	-66(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar@PLT
	
	#printf("%s \n", var7[1]);
	movzbl	-66(%rbp), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC11(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	
	#return 0;
	movl	$0, %eax
	
	#}
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	ret
