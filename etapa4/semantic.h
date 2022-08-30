#include "hash.h"
#include "ast.h"

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

int SemanticErrors;

void check_and_set_declarations(AST* node);
void check_undeclared();
void check_operands(AST* node);
void check_array(AST* node);
int get_semantic_errors();
int convert_ast_type(AST* node);
void verifySemantic(AST* node);
int is_number(AST* son);
int is_integer(AST* son);
int is_float(AST* son);
int is_char(AST* son);
int is_bool(AST* son);


#endif
