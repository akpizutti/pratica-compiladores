//Alunos:
//Arthur Casagrande Paixão - 00288561
//Akim Lee Pizutti - 00302937

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hash.h"
#include "ast.h"
#include "semantic.h"



//lex.yy.h
int yylex();
int yyparse();
extern char *yytext;
extern FILE *yyin;
extern AST* astRoot;


int main(int argc, char** argv) {
	if (0 == (yyin = fopen(argv[1],"r"))){
		printf("Cannot open file %s... \n",argv[1]);
		exit(1);
	}
	else{
		if(!yyparse()){
            printf("sucesso\n");
            if(argc >= 3)
            {
            	printf("tentando descompilar");
            	FILE* saida = fopen(argv[2],"w");
            	descompila(saida,astRoot);
        	}else{
        		printf("Arquivo de saída não especificado. Programa descompilado não foi gerado.\n");
        		}
			//hashPrint();
			verifySemantic(astRoot);
			if(get_semantic_errors > 0)
				exit(4);
			else
				exit(0);
		}
		else{
			exit(2);
		}
	}
	return 0;
}




