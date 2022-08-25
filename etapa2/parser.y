/*Alunos:
Arthur Casagrande Paixão - 00288561
Akim Lee Pizutti - 00302937
*/
%{
    int yyerror();
%}



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

%token TK_IDENTIFIER

%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_CHAR
%token LIT_STRING

%token END_OF_FILE
%token TOKEN_ERROR



%left '+' '-'
%left '.' '/'
%left '<' '>'
%left OPERATOR_LE OPERATOR_GE
%left OPERATOR_EQ OPERATOR_DIF
%left '&' '|' '~'
%nonassoc IFX
%nonassoc KW_ELSE


%%

program: list;
list: declaration list | ;
declaration: variable | function;


variable: type TK_IDENTIFIER '(' value ')' ';'
	| type TK_IDENTIFIER '[' LIT_INTEGER ']' array_val_list ';' ;
array_val_list: value array_val_list | ;


function: type TK_IDENTIFIER '(' parameters ')' block ;
parameters: type TK_IDENTIFIER parameters | ;
block: '{' command_list '}' ;
command_list: command command_list_tail;
command_list_tail: ';' command command_list_tail | ;
command: assignment | flux_control | read | print | return | block | ;


assignment: TK_IDENTIFIER ASSIGNMENT expression
	  | TK_IDENTIFIER '[' expression ']' ASSIGNMENT expression ;


read: KW_READ TK_IDENTIFIER | KW_READ TK_IDENTIFIER '[' expression ']' ;


print: KW_PRINT element_list ;
element_list: print_argument element_list | ;
print_argument: LIT_STRING | expression;


return: KW_RETURN expression ;


expression: expression '+' expression
	  | expression '-' expression
	  | expression '.' expression
	  | expression '/' expression
	  | expression '<' expression
	  | expression '>' expression
	  | expression OPERATOR_LE expression
	  | expression OPERATOR_GE expression
	  | expression OPERATOR_EQ expression
	  | expression OPERATOR_DIF expression
	  | expression '&' expression
	  | expression '|' expression
	  | expression '~' expression
	  | '(' expression ')'
	  | TK_IDENTIFIER '(' argument_list ')'
	  | TK_IDENTIFIER '[' expression ']'
	  | TK_IDENTIFIER
	  | value;
argument_list: expression argument_list | ;


flux_control: KW_IF '(' expression ')' command %prec IFX
	    | KW_IF '(' expression ')' command KW_ELSE command
	    | KW_WHILE '(' expression ')' command ;


value: LIT_INTEGER | LIT_FLOAT | LIT_CHAR ;
type: KW_INT | KW_FLOAT | KW_CHAR;

%%

int yyerror(){
    fprintf(stderr, "ERRO na linha %d!\n", lineNumber);
    exit(3);
}
