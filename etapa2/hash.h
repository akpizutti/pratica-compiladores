//Créditos: Prof. Marcelo Johann

//Alunos:
//Arthur Casagrande Paixão - 00288561
//Akim Lee Pizutti - 00302937

#ifndef HASH
#define HASH

#include <stdio.h>

#define HASH_SIZE 997

#define SYMBOL_LIT_INT		1
#define SYMBOL_LIT_CHAR		2
#define SYMBOL_LIT_FLOAT	3
#define SYMBOL_LIT_STRING	4
#define SYMBOL_IDENTIFIER	5	

typedef struct hash_node{
	int type;
	char *text;
	struct hash_node *next;
}	HASH_NODE;

void hashInit(void);

int hashAddress(char *text);

HASH_NODE *hashInsert(char *text, int type);

HASH_NODE *hashFind(char *text);

void hashPrint(void);

#endif
