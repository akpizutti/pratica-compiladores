%x COMMENT

%{
#include "hash.h"
#include "y.tab.h"

int lineNumber = 1;
int token = 0;
int end_of_file = 0;
%}



%%

"char"				{return KW_CHAR;}
"int"				{return KW_INT;}
"float"				{return KW_FLOAT;}
"if"				{return KW_IF;}
"else"				{return KW_ELSE;}
"while"				{return KW_WHILE;}
"read"				{return KW_READ;}
"print"				{return KW_PRINT;}
"return"			{return KW_RETURN;}
"<-"				{return ASSIGNMENT;}
"<="				{return OPERATOR_LE;}
">="				{return OPERATOR_GE;}
"=="				{return OPERATOR_EQ;}
"!="				{return OPERATOR_DIF;}
[-+;\(\)\[\]{}\./<>&\|~]	{return yytext[0];}
[a-zA-Z_]+			{ hashInsert(yytext, SYMBOL_IDENTIFIER); return TK_IDENTIFIER;}
[0-9]+				{ hashInsert(yytext, SYMBOL_LIT_INT); return LIT_INTEGER;}
[0-9]+\,[0-9]+		{ hashInsert(yytext, SYMBOL_LIT_FLOAT); return LIT_FLOAT;}
\'.\'				{ hashInsert(yytext, SYMBOL_LIT_CHAR); return LIT_CHAR;}
\"([^\"\n]|\\.)*\"	{ hashInsert(yytext, SYMBOL_LIT_STRING); return LIT_STRING;}
"##".*				{;}
"\\#"				{ BEGIN(COMMENT); } // backup: "\\#"(.|\n)*"#\\"
[ \t]+				{;}
"\n" 				{ ++lineNumber; }

.				{return TOKEN_ERROR;}


<COMMENT>"#\\" { BEGIN(INITIAL); }
<COMMENT>.	{}
<COMMENT>\n 	{++lineNumber; }




%%

#include "hash.c"
int isRunning(void){
	if(end_of_file==1)
        return 0;
    else return 1;
}

void initMe()
{
    hashInit();
    return;
}

int getLineNumber()
{
    return lineNumber;
}

#include "y.tab.c"
#include "main.c"
//#include "main-minha.c"
//#include "main-prof.c"



int yywrap(void) {
	return 1;


}

