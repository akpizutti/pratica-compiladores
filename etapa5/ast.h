#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"

#define MAXSONS 4

#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MUL 4
#define AST_DIV 5
#define AST_LES 6
#define AST_GRT 7
#define AST_LEQ 8
#define AST_GEQ 9
#define AST_EQ 10
#define AST_DIF 11
#define AST_AND 12
#define AST_OR 13
#define AST_NOT 14

#define AST_IF 15
#define AST_IF_ELSE 16
#define AST_WHILE 17

#define AST_PROGRAM 18
#define AST_LIST 19
#define AST_DECLARATION 20

#define AST_VARIABLE 21
#define AST_ARRAY_VAL_LIST 22


#define AST_FUNCTION 23
#define AST_PARAMETERS 24
#define AST_BLOCK 25
#define AST_COMMAND_LIST 26
#define AST_COMMAND_LIST_TAIL 27
#define AST_COMMAND 28

#define AST_ASSIGNMENT 29

#define AST_READ 30

#define AST_PRINT 31
#define AST_ELEMENT_LIST 32

#define AST_RETURN 34

#define AST_FUNCTION_CALL 35
#define AST_ARRAY_ACC 36
#define AST_VALUE 37
#define AST_ARGUMENT_LIST 38

#define AST_KW_INT 39
#define AST_KW_FLOAT 40
#define AST_KW_CHAR 41

#define AST_VARIABLE_ARRAY 42
#define AST_ASSIGNMENT_ARRAY 43
#define AST_READ_ARRAY 44

#define AST_PRINT_ARGUMENT_STR 45
#define AST_PRINT_ARGUMENT_EXPR 46

#define AST_NESTED_EXPR 47

#define AST_ARGUMENT 48


typedef struct astNode{
    int type;
    struct astNode* son[MAXSONS];
    HASH_NODE* symbol;
}   AST;

AST* astCreate(int type, HASH_NODE *symbol, AST* son0, AST* son1, AST* son2, AST* son3);

void astPrint(AST* node, int level);
void descompila(FILE* saida, AST* node);

void testabc();

#endif
