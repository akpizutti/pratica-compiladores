//Créditos: Prof. Marcelo Johann

//Alunos:
//Arthur Casagrande Paixão - 00288561
//Akim Lee Pizutti - 00302937

#ifndef HASH
#define HASH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 997

//node types
#define SYMBOL_LIT_INT		1
#define SYMBOL_LIT_CHAR		2
#define SYMBOL_LIT_FLOAT	3
#define SYMBOL_LIT_STRING	4
#define SYMBOL_IDENTIFIER	5
#define SYMBOL_VARIABLE	6
#define SYMBOL_FUNCTION	7
#define SYMBOL_VECTOR	8
#define SYMBOL_ARRAY 9
#define SYMBOL_PARAMETER 10
#define SYMBOL_LABEL 11

//data types
#define DATATYPE_INT 1
#define DATATYPE_CHAR 2
#define DATATYPE_FLOAT 3
#define DATATYPE_STRING 4

typedef struct hash_node{
	int type;
    int datatype;
	char *text;
	struct hash_node *next;
}	HASH_NODE;

void hashInit(void);

int hashAddress(char *text);

HASH_NODE *hashInsert(char *text, int type);

HASH_NODE *hashFind(char *text);

void hashPrint(void);

int hash_check_undeclared(void);

HASH_NODE* makeTemp(void);
HASH_NODE* makeLabel(void);

void printASM(FILE* fout);





#endif
