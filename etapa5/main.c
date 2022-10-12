//Alunos:
//Arthur Casagrande Paixão - 00288561
//Akim Lee Pizutti - 00302937

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash.h"
#include "ast.h"
#include "semantic.h"
extern AST* astRoot;
AST* astRoot2;
#include "tac.h"



//lex.yy.h
int yylex();
int yyparse();
extern char *yytext;
extern FILE *yyin;


int main(int argc, char** argv) {
	if (0 == (yyin = fopen(argv[1],"r"))){
		printf("Cannot open file %s... \n",argv[1]);
		exit(1);
	}
	else{
		if(!yyparse()){
            printf("sucesso\n");
            FILE* saida = fopen(argv[2],"w");
            descompila(saida,astRoot);
			//hashPrint();
			verifySemantic(astRoot);
			if(get_semantic_errors() > 0){
				printf("erro semantico\n");
				exit(4);
				}
			else {
				printf("eh pra printar a tac agora\n");
				astRoot2 = astRoot;
				tacPrintBack(generateCode(astRoot));
				exit(0);
				}
		}
		else{
			exit(2);
		}
	}
	return 0;
}




