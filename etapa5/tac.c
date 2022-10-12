#include "tac.h"


TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2)
{
	TAC* newtac;
	newtac = (TAC*)calloc(1,sizeof(TAC));
	newtac->type = type;
	newtac->res = res;
	newtac->op1 = op1;
	newtac->op2 = op2;
	newtac->prev = 0;
	newtac->next = 0;
	return newtac;
}
void tacPrint(TAC* tac)
{
	if(!tac || tac->type==TAC_SYMBOL) return;
	
	fprintf(stderr,"TAC(");
	switch(tac->type)
	{
		case TAC_SYMBOL: fprintf(stderr,"TAC_SYMBOL"); break;
		case TAC_ADD: fprintf(stderr,"TAC_ADD"); break;
		case TAC_SUB: fprintf(stderr,"TAC_SUB"); break;
		case TAC_MUL: fprintf(stderr,"TAC_MUL"); break;
		case TAC_DIV: fprintf(stderr,"TAC_DIV"); break;
		case TAC_LES: fprintf(stderr,"TAC_LES"); break;
		case TAC_GRT: fprintf(stderr,"TAC_GRT"); break;
		case TAC_LEQ: fprintf(stderr,"TAC_LEQ"); break;
		case TAC_GEQ: fprintf(stderr,"TAC_GEQ"); break;
		case TAC_EQ: fprintf(stderr,"TAC_EQ"); break;
		case TAC_DIF: fprintf(stderr,"TAC_DIF"); break;
		case TAC_AND: fprintf(stderr,"TAC_AND"); break;
		case TAC_OR: fprintf(stderr,"TAC_OR"); break;
		case TAC_NOT: fprintf(stderr,"TAC_NOT"); break;
		
		case TAC_COPY: fprintf(stderr,"TAC_COPY"); break;
		case TAC_JZ: fprintf(stderr,"TAC_JZ"); break;
		case TAC_JMP: fprintf(stderr,"TAC_JMP"); break;
		case TAC_LABEL: fprintf(stderr,"TAC_LABEL"); break;
		
		case TAC_BEGINFUN: fprintf(stderr,"TAC_BEGINFUN"); break;
		case TAC_ENDFUN: fprintf(stderr,"TAC_ENDFUN"); break;
		case TAC_PRINT: fprintf(stderr,"TAC_PRINT"); break;
		case TAC_ARRAY_ACC: fprintf(stderr,"TAC_ARRAY_ACC"); break;
		case TAC_READ: fprintf(stderr,"TAC_READ"); break;
		case TAC_READ_ARRAY: fprintf(stderr,"TAC_READ_ARRAY"); break;
		case TAC_RETURN: fprintf(stderr,"TAC_RETURN"); break;
		
		case TAC_ARG: fprintf(stderr,"TAC_ARG"); break;
		case TAC_FUNCALL: fprintf(stderr,"TAC_FUNCALL"); break;
		case TAC_SET_ARR: fprintf(stderr,"TAC_SET_ARR"); break;

		
		default:
			fprintf(stderr,"UNKNOWN");
			break;
		
	}
	fprintf(stderr,", %s",(tac->res)?tac->res->text:"0");
	fprintf(stderr,", %s",(tac->op1)?tac->op1->text:"0");
	fprintf(stderr,", %s",(tac->op2)?tac->op2->text:"0");
	fprintf(stderr,")\n");
}

void tacPrintBack(TAC *tac)
{
	if(!tac)
	{
		return;
	}
	else
	{
		tacPrintBack(tac->prev);
		tacPrint(tac);
	}
}

TAC* tacJoin(TAC* l1, TAC* l2)
{
	TAC* point;
	if(!l1) return l2;
	if(!l2) return l1;
	
	for(point = l2; point->prev != 0; point = point->prev) {;}
	point->prev = l1;
	return l2;
}

TAC* makebinop(int type, TAC* code0, TAC* code1)
{
	return tacJoin(tacJoin(code0,code1), 
			tacCreate(type,makeTemp(code0?code0->res->datatype:0),code0?code0->res:0,code1?code1->res:0) ); 
}




TAC* makeIf(TAC* code0, TAC* code1)
{
	TAC* jumptac = 0;
	TAC* labeltac = 0;
	HASH_NODE* newlabel = 0;
	
	newlabel = makeLabel();
	
	jumptac = tacCreate(TAC_JZ,newlabel,code0?code0->res:0,0);
	jumptac->prev = code0;
	labeltac = tacCreate(TAC_LABEL, newlabel,0,0);
	labeltac->prev = code1;
	return tacJoin(jumptac,labeltac);
}

TAC* makeIfElse(TAC* code0, TAC* code1, TAC* code2)
{
	/*
	código:
			code0
			jz label1
			code1
			jmp label2
		label1:
			code2
		label2:
	*/
	TAC* jztac = 0;
	TAC* jumptac = 0;
	TAC* labeltac1 = 0;
	TAC* labeltac2 = 0;
	HASH_NODE* label1 = makeLabel();
	HASH_NODE* label2 = makeLabel();
	
	jztac = tacCreate(TAC_JZ,label1,code0?code0->res:0,0);
	jztac->prev = code0;
	jumptac = tacCreate(TAC_JMP,label2,0,0);
	jumptac->prev = code1;
	
	jumptac = tacJoin(jztac,jumptac);
	
	labeltac1 = tacCreate(TAC_LABEL, label1,0,0);
	labeltac1->prev = jumptac;
	labeltac2 = tacCreate(TAC_LABEL, label2,0,0);
	labeltac2->prev = code2;
	
	return tacJoin(labeltac1,labeltac2);
}

TAC* makeWhile(TAC* code0, TAC* code1)
{
	/*
	código:
		label2:
			code0
			jz label1
			code1
			jmp label2
		label1:

	*/
	TAC* jztac = 0;
	TAC* jumptac = 0;
	TAC* labeltac1 = 0;
	TAC* labeltac2 = 0;
	HASH_NODE* label1 = makeLabel();
	HASH_NODE* label2 = makeLabel();
	
	jztac = tacCreate(TAC_JZ,label1,code0?code0->res:0,0);
	jztac->prev = code0;
	jztac = tacJoin(jztac,code1);
	
	jumptac = tacCreate(TAC_JMP,label2,0,0);
	jumptac->prev = jztac;
	
	labeltac1 = tacCreate(TAC_LABEL, label1,0,0);
	labeltac1->prev = jumptac;
	
	labeltac2 = tacCreate(TAC_LABEL, label2,0,0);
	
	return tacJoin(labeltac2,labeltac1);
	
}

TAC* makeArrayDec(AST* node)
{
	TAC* out = 0;
	AST* point = node->son[2];
	int i = 0;
	char index[256] = "";
	
		
	while(point)
	{	
		sprintf(index,"%d",i);
		//out = tacJoin(out,tacCreate(TAC_SET_ARR,node->symbol,point->son[0]->symbol,0));
		out = tacJoin(out,tacCreate(TAC_SET_ARR,node->symbol,point->son[0]->symbol,hashInsert(index,SYMBOL_LIT_INT)));
		point = point->son[1];
		i++;
	}
	return out;
}


//função muito louca, tirou vários pontos da minha sanidade
TAC* makeFunCall(TAC* code0, AST* node)
{
	TAC* out = 0;
	AST* astFundec = astFind(astRoot2, node->symbol->text, AST_FUNCTION);
	
	if(astFundec == 0){
		fprintf(stderr,"Declaracao da funcao nao encontrada\n");
		return 0;
		}
	AST* astParameters = astFundec->son[1];
	
	
	int i=0;
	AST* point = node->son[0];
	while(point)
	{
		
		out = tacJoin(out, tacCreate(TAC_ARG, astParameters->symbol, code0->res,0));
		astParameters = astParameters->son[1];
		point = point->son[1];
		i++;

	}
	
	out = tacJoin(out,tacCreate(TAC_FUNCALL,makeTemp(astFundec->symbol->datatype),node->symbol,0));
	return out;
	
	

}

TAC* generateCode(AST* node)
{
	if(!node) return 0;
	
	TAC* result = 0;
	TAC* code[MAXSONS];
	
	for(int i=0; i<MAXSONS; ++i)
	{
		code[i] = generateCode(node->son[i]);
	}
	
	switch(node->type)
	{
		case AST_SYMBOL:
			result = tacCreate(TAC_SYMBOL,node->symbol,0,0); 
			break;
			
		case AST_ADD :
			result = makebinop(TAC_ADD,code[0],code[1]);
			break;
		case AST_SUB:
			result = makebinop(TAC_SUB,code[0],code[1]);
			break;
		case AST_MUL:
			result = makebinop(TAC_MUL,code[0],code[1]);
			break;
		case AST_DIV:
			result = makebinop(TAC_DIV,code[0],code[1]);
			break;
		case AST_LES:
			result = makebinop(TAC_LES,code[0],code[1]);
			break;
		case AST_GRT:
			result = makebinop(TAC_GRT,code[0],code[1]);
			break;
		case AST_LEQ:
			result = makebinop(TAC_LEQ,code[0],code[1]);
			break;
		case AST_GEQ:
			result = makebinop(TAC_GEQ,code[0],code[1]);
			break;
		case AST_EQ:
			result = makebinop(TAC_EQ,code[0],code[1]);
			break;
		case AST_DIF:
			result = makebinop(TAC_DIF,code[0],code[1]);
			break;
		case AST_OR:
			result = makebinop(TAC_OR,code[0],code[1]);
			break;
		case AST_AND:
			result = makebinop(TAC_AND,code[0],code[1]);
			break;
		case AST_NOT:
			result = makebinop(TAC_NOT,code[0],code[1]);
			break;	
		
		case AST_VARIABLE:
			result = tacJoin(code[1],tacCreate(TAC_COPY,node->symbol,code[1]?code[1]->res:0,0)); 
			break;
		case AST_VARIABLE_ARRAY:
			result = makeArrayDec(node);
			
			break;
			
		case AST_ASSIGNMENT:
			result = tacJoin(code[0],tacCreate(TAC_COPY,node->symbol,code[0]?code[0]->res:0,0)); 
			break;
		case AST_ASSIGNMENT_ARRAY:
			result = tacJoin(code[0],tacJoin(code[1],tacCreate(TAC_COPY,node->symbol,code[1]?code[1]->res:0,code[0]?code[0]->res:0))); 
			break;
			
		case AST_READ:
			result = tacCreate(TAC_READ,node->symbol,0,0);
			break;
		case AST_READ_ARRAY:
			result = tacJoin(code[0],tacCreate(TAC_READ_ARRAY,node->symbol,code[0]?code[0]->res:0,0));
			break;
		
		case AST_IF:
			result = makeIf(code[0], code[1]);
			break;
		case AST_IF_ELSE:
			result = makeIfElse(code[0],code[1],code[2]);
			break;
		/*	só descomenta depois de fazer a makeWhile, senão dá segfault */
		case AST_WHILE:
			result = makeWhile(code[0],code[1]);
			break;
			
		case AST_FUNCTION: 
			result = tacJoin(tacJoin(tacCreate(TAC_BEGINFUN,node->symbol,0,0),code[2]),tacCreate(TAC_ENDFUN,node->symbol,0,0));
			break;
		case AST_PRINT_ARGUMENT_EXPR: 
			result = tacJoin(code[0], tacCreate(TAC_PRINT,code[0]?code[0]->res:0,0,0));
			break;
		case AST_PRINT_ARGUMENT_STR: 
			result = tacCreate(TAC_PRINT,node->symbol,0,0);
			break;
		
		case AST_ARRAY_ACC:
			result = tacJoin(code[0],tacCreate(TAC_ARRAY_ACC,makeTemp(node->symbol->datatype),node->symbol,code[0]->res));
			break;
		/*
		case AST_ARGUMENT:
			result = tacJoin(code[0],tacCreate(TAC_ARG,code[0]->res,0,0));
			break;
		*/
		case AST_FUNCTION_CALL:
			//result = tacJoin(code[0],tacCreate(TAC_FUNCALL,makeTemp(),0,0));
			result = makeFunCall(code[0],node);
			break; 
			
		case AST_RETURN: 
			result = tacJoin(code[0], tacCreate(TAC_RETURN,code[0]?code[0]->res:0,0,0));
			break;
			

		default:
			result = tacJoin(code[0], tacJoin(code[1],tacJoin(code[2],code[3]))); 
			break;
	}
}























