#ifndef TAC_HEADER
#define TAC_HEADER


#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_LES 6
#define TAC_GRT 7
#define TAC_LEQ 8
#define TAC_GEQ 9
#define TAC_EQ 10
#define TAC_DIF 11
#define TAC_AND 12
#define TAC_OR 13
#define TAC_NOT 14

#define TAC_JZ 18
#define TAC_LABEL 19
#define TAC_JMP 20

#define TAC_BEGINFUN 21
#define TAC_ENDFUN 22
#define TAC_PRINT 23

#define TAC_ARRAY_ACC 24

#define TAC_READ 25
#define TAC_READ_ARRAY 26

#define TAC_RETURN 27
#define TAC_ARG 28
#define TAC_FUNCALL 29


#define TAC_COPY 30
#define TAC_SET_ARR 31

extern AST* astRoot2;

typedef struct tac_node 
{
	int type;
	HASH_NODE* res;
	HASH_NODE* op1;
	HASH_NODE* op2;
	struct tac_node* prev;
	struct tac_node* next;


} TAC; 

//extern AST* astRoot;

TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2);
void tacPrint(TAC* tac);
void tacPrintBack(TAC *tac);
TAC* tacJoin(TAC* l1, TAC* l2);

TAC* generateCode(AST* node); 


#endif
