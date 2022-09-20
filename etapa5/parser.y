/*Alunos:
Arthur Casagrande Paixão - 00288561
Akim Lee Pizutti - 00302937
*/
%{
    #include "ast.c"
	#include "semantic.h"
	#include "tac.h"
    	int yyerror();
	int getLineNumber();
	int yylex();
	FILE* saida;
	AST* astRoot;
%}

%union
{
HASH_NODE* symbol;
AST* ast;
}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT

%token KW_IF
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token ASSIGNMENT
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token END_OF_FILE
%token TOKEN_ERROR

%type<ast> program
%type<ast> list
%type<ast> declaration

%type<ast> variable
%type<ast> array_val_list
%type<ast> array_size

%type<ast> function
%type<ast> parameters
%type<ast> block
%type<ast> command_list
%type<ast> command_list_tail
%type<ast> command

%type<ast> assignment

%type<ast> read

%type<ast> print
%type<ast> element_list
%type<ast> print_argument

%type<ast> return

%type<ast> expression
%type<ast> argument_list
%type<ast> argument

%type<ast> flux_control

%type<ast> value
%type<ast> type





%left '+' '-'
%left '.' '/'
%left '<' '>'
%left OPERATOR_LE OPERATOR_GE
%left OPERATOR_EQ OPERATOR_DIF
%left '&' '|' '~'
%nonassoc IFX
%nonassoc KW_ELSE


%%

program: list			{
						$$ = astCreate(AST_PROGRAM, 0, $1, 0, 0, 0);
						astRoot = $$;
						astPrint(astRoot, 0);
						//check_and_set_declarations($$);
						//check_undeclared();
						//check_operands($$);
						tacPrintBack(generateCode($1));
						} //saida = fopen("output.txt","w"); descompila(saida,$$); astPrint($$, 0);
       ;
list: declaration list 		{$$ = astCreate(AST_LIST, 0, $1, $2, 0, 0);}
    | 				{$$ = 0;} ;
declaration: variable 		{$$ = astCreate(AST_DECLARATION, 0, $1, 0, 0, 0);}
	   | function		{$$ = astCreate(AST_DECLARATION, 0, $1, 0, 0, 0);}
	   ;


variable: type TK_IDENTIFIER '(' value ')' ';'				{$$ = astCreate(AST_VARIABLE, $2, $1, $4, 0, 0);}
	| type TK_IDENTIFIER '[' array_size ']' array_val_list ';' 	{$$ = astCreate(AST_VARIABLE_ARRAY, $2, $1, $4, $6, 0);}
	;

array_size: LIT_INTEGER                                 {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
;

array_val_list: value array_val_list 					{$$ = astCreate(AST_ARRAY_VAL_LIST, 0, $1, $2, 0, 0);}
	      | 							{$$ = 0;} ;


function: type TK_IDENTIFIER '(' parameters ')' block		{$$ = astCreate(AST_FUNCTION, $2, $1, $4, $6, 0);}
	;
parameters: type TK_IDENTIFIER parameters 			{$$ = astCreate(AST_PARAMETERS, $2, $1, $3, 0, 0);}
	  | 							{$$ = 0;} ;
block: '{' command_list '}' 					{$$ = astCreate(AST_BLOCK, 0, $2, 0, 0, 0);}
     ;
command_list: command command_list_tail				{$$ = astCreate(AST_COMMAND_LIST, 0, $1, $2, 0, 0);}
	    ;
command_list_tail: ';' command command_list_tail 		{$$ = astCreate(AST_COMMAND_LIST_TAIL, 0, $2, $3, 0, 0);}
		 | 						{$$ = 0;} ;
command: assignment 						{$$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0);}
       | flux_control 						{$$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0);}
       | read 							{$$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0);}
       | print 							{$$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0);}
       | return 						{$$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0);}
       | block 							{$$ = astCreate(AST_COMMAND, 0, $1, 0, 0, 0);}
       | 							{$$ = 0;} ;


assignment: TK_IDENTIFIER ASSIGNMENT expression 			{$$ = astCreate(AST_ASSIGNMENT, $1, $3, 0, 0, 0);} // astPrint($3, 0);
	  | TK_IDENTIFIER '[' expression ']' ASSIGNMENT expression 	{$$ = astCreate(AST_ASSIGNMENT_ARRAY, $1, $3, $6, 0, 0);}
	  ;


read: KW_READ TK_IDENTIFIER 			{$$ = astCreate(AST_READ, $2, 0, 0, 0, 0);}
    | KW_READ TK_IDENTIFIER '[' expression ']' 	{$$ = astCreate(AST_READ_ARRAY, $2, $4, 0, 0, 0);}
    ;


print: KW_PRINT element_list 			{$$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0);}
     ;
element_list: print_argument element_list  	{$$ = astCreate(AST_ELEMENT_LIST, 0, $1, $2, 0, 0);}
	    | 					{$$ = 0;} ;
print_argument: LIT_STRING 			{$$ = astCreate(AST_PRINT_ARGUMENT_STR, $1, 0, 0, 0, 0);}
	      | expression			{$$ = astCreate(AST_PRINT_ARGUMENT_EXPR, 0, $1, 0, 0, 0);}
	      ;


return: KW_RETURN expression 			{$$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0);}
      ;


expression: expression '+' expression		{$$ = astCreate(AST_ADD, 0, $1, $3, 0, 0);}
	  | expression '-' expression 		{$$ = astCreate(AST_SUB, 0,$1,$3,0,0);}
	  | expression '.' expression 		{$$ = astCreate(AST_MUL, 0,$1,$3,0,0);}
	  | expression '/' expression 		{$$ = astCreate(AST_DIV, 0,$1,$3,0,0);}
	  | expression '<' expression		{$$ = astCreate(AST_LES, 0,$1,$3,0,0);}
	  | expression '>' expression 		{$$ = astCreate(AST_GRT, 0,$1,$3,0,0);}
	  | expression OPERATOR_LE expression 	{$$ = astCreate(AST_LEQ, 0,$1,$3,0,0);}
	  | expression OPERATOR_GE expression 	{$$ = astCreate(AST_GEQ, 0,$1,$3,0,0);}
	  | expression OPERATOR_EQ expression 	{$$ = astCreate(AST_EQ, 0,$1,$3,0,0);}
	  | expression OPERATOR_DIF expression 	{$$ = astCreate(AST_DIF, 0,$1,$3,0,0);}
	  | expression '&' expression 		{$$ = astCreate(AST_AND, 0,$1,$3,0,0);}
	  | expression '|' expression 		{$$ = astCreate(AST_OR, 0,$1,$3,0,0);}
	  | expression '~' expression 		{$$ = astCreate(AST_NOT, 0,$1,$3,0,0);}
	  | '(' expression ')'             	{$$ = astCreate(AST_NESTED_EXPR, 0,$2,0,0,0);}
	  | TK_IDENTIFIER '(' argument_list ')' {$$ = astCreate(AST_FUNCTION_CALL, $1, $3, 0, 0, 0);}
	  | TK_IDENTIFIER '[' expression ']' 	{$$ = astCreate(AST_ARRAY_ACC, $1, $3, 0, 0, 0);}
      	  | TK_IDENTIFIER 			{$$ = astCreate(AST_SYMBOL, $1,0,0,0,0); } //tacPrint(tacCreate(TAC_SYMBOL, $1,0,0));
	  | value				{$$ = $1;} //astCreate(AST_VALUE, 0,$1,0,0,0);
	  ;
argument_list: argument argument_list 	{$$ = astCreate(AST_ARGUMENT_LIST, 0, $1, $2, 0, 0);}
	     | 			{$$ = 0;} ;

argument: expression {$$ = astCreate(AST_ARGUMENT, 0, $1, 0, 0, 0);};
		

flux_control: KW_IF '(' expression ')' command %prec IFX 	{$$ = astCreate(AST_IF, 0, $3, $5, 0, 0);}
	    | KW_IF '(' expression ')' command KW_ELSE command 	{$$ = astCreate(AST_IF_ELSE, 0, $3, $5, $7, 0);}
	    | KW_WHILE '(' expression ')' command 		{$$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0);}
	    ;


value: LIT_INTEGER 	{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
     | LIT_FLOAT 	{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
     | LIT_CHAR 	{$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
     ;
type: KW_INT {$$ = astCreate(AST_KW_INT, 0, 0, 0, 0, 0);}
    | KW_FLOAT {$$ = astCreate(AST_KW_FLOAT, 0, 0, 0, 0, 0);}
    | KW_CHAR {$$ = astCreate(AST_KW_CHAR, 0, 0, 0, 0, 0);}
    ;

%%

int yyerror(){
    fprintf(stderr, "ERRO na linha %d!\n", getLineNumber());
    exit(3);
}
