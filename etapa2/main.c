//Alunos:
//Arthur Casagrande Paixão - 00288561
//Akim Lee Pizutti - 00302937

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//lex.yy.h
int yylex();
extern char *yytext;
extern FILE *yyin;

//void debug(int token);

int main(int argc, char** argv) {

	if (0 == (yyin = fopen(argv[1],"r"))){
		printf("Cannot open file %s... \n",argv[1]);
		exit(1);
	}
	else{
		if(!yyparse()){
			exit(0);
		}
		else{
			exit(2);
		}
	}
	return 0;
}


/*void debug(int token){
	switch(token){
		case KW_CHAR:		printf("KW_CHAR\n"); break;
		case KW_INT:		printf("KW_INT\n"); break;
		case KW_FLOAT:		printf("KW_FLOAT\n"); break;
		case KW_IF:		    printf("KW_IF\n"); break;
		case KW_ELSE:		printf("KW_ELSE\n"); break;
		case KW_WHILE:		printf("KW_WHILE\n"); break;
		case KW_READ:		printf("KW_READ\n"); break;
		case KW_PRINT:		printf("KW_PRINT\n"); break;
		case KW_RETURN:		printf("KW_RETURN\n"); break;
		case ASSIGNMENT:	printf("Assignment\n"); break;
		case OPERATOR_LE:	printf("Lesser or Equal!\n"); break;
		case OPERATOR_GE:	printf("Greater or Equal!\n"); break;
		case OPERATOR_EQ:	printf("Equal!\n"); break;
		case OPERATOR_DIF:	printf("Different!\n"); break;
		case TK_IDENTIFIER:	printf("Token: %s\n", yytext); break;
		case LIT_INTEGER:	printf("Integer: %s\n", yytext); break;
		case LIT_FLOAT:		printf("Float: %s\n", yytext); break;
		case LIT_CHAR:		printf("Char: %s\n",yytext); break;
		case LIT_STRING:	printf("String: %s\n", yytext); break;
		case TOKEN_ERROR:	printf("Error!\n"); break;
		case END_OF_FILE:   printf("End of file.\n"); break;
		default: 		printf("Special Char: %c\n",yytext[0]); break;
	}
}*/


