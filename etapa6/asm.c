#include "asm.h"
extern HASH_NODE *Table[HASH_SIZE];

void dotFloat(char* float_num){
    int i;
    int len = strlen(float_num);
    for (i = 0; i < len; i++) {
        if (float_num[i] == ',') {
            float_num[i] = '.';
            break;
        }
    }
}

void printASM(FILE* fout, TAC* first){
    int i;
	HASH_NODE *node;
	TAC* tac = first;

    fprintf(fout,   "## DATA SECTION\n"
                        "\t.data\n"
                        "\t.align\t4\n");
    for(; tac; tac = tac->next){
        if(tac->type == TAC_DECLARATION){
            fprintf(fout, "_%s:\t.long\t%s\n", tac->res->text, tac->op1->text); //Dá pra fazer um IF e declarar certinho como .int, .float, .string ou .byte
        }
    }
    
	for(i = 0; i<HASH_SIZE; ++i){
		for(node = Table[i]; node; node = node->next){
		    if(node->type == SYMBOL_VARIABLE && strstr(node->text, "temp")){
		        fprintf(fout, "_%s:\t.zero\t4\n", node->text);
			}
			else if(node->type == SYMBOL_LIT_INT){
			    fprintf(fout, "_%s:\t.int\t%s\n", node->text, node->text);
			}
			else if(node->type == SYMBOL_LIT_FLOAT){
			    dotFloat(node->text);
			    fprintf(fout, "_%s:\t.float\t%s\n", node->text, node->text);
			}
			else if(node->type == SYMBOL_LIT_CHAR){
			    fprintf(fout, "_%s:\t.byte\t%d\n", node->text, node->text[1]);
			}
			else if(node->type == SYMBOL_LIT_STRING){
			    fprintf(fout, "_%s%d:\t.string\t%s\n", "str", node->id, node->text);
			}
		}
	}
	fprintf(fout,"\n");
}

void generateASM(TAC *first){

    TAC* tac;
    FILE* fout;
    fout = fopen("out.s", "w");
    
    // INIT
    fprintf(fout,   "## INIT\n");
    fprintf(fout,   "printInt:\n"
	                    "\t.string	\"%%d\"\n"
                    "printFlt:\n"
	                    "\t.string	\"%%f\"\n"
                    "printStr:\n"
	                    "\t.string	\"%%s\"\n"
	                "\n");    
    
    //EACH TAC
    for(tac = first; tac; tac = tac->next){
        switch(tac->type){
		    //case TAC_SYMBOL: fprintf(stderr,"TAC_SYMBOL"); break;
		    case TAC_ADD: makeAdd(fout, tac); break;
		    case TAC_SUB: makeSub(fout, tac); break;
		    case TAC_MUL: makeMul(fout, tac); break;
		    case TAC_DIV: makeDiv(fout, tac); break;
		    case TAC_LES: makeGrt(fout, tac); break;
		    case TAC_GRT: makeLes(fout, tac); break;
		    case TAC_LEQ: makeLeq(fout, tac); break;
		    case TAC_GEQ: makeGeq(fout, tac); break;
		    case TAC_EQ: makeEq(fout, tac); break;
		    case TAC_DIF: makeDif(fout, tac); break;
		    case TAC_AND: makeAnd(fout, tac); break;
		    case TAC_OR: makeOr(fout, tac); break;
		    //case TAC_NOT: fprintf(stderr,"TAC_NOT"); break;
		    
		    case TAC_COPY: makeCopy(fout, tac); break;
		    case TAC_JZ: makeJz(fout, tac); break;
		    case TAC_JMP: makeJmp(fout, tac); break;
		    case TAC_LABEL: makeLab(fout, tac); break;
		    
		    case TAC_BEGINFUN: makeBeginFun(fout, tac); break;
		    case TAC_ENDFUN: makeEndFun(fout, tac); break;
		    case TAC_PRINT: makePrint(fout, tac); break;
		    //case TAC_ARRAY_ACC: fprintf(stderr,"TAC_ARRAY_ACC"); break;
		    //case TAC_READ: fprintf(stderr,"TAC_READ"); break;
		    //case TAC_READ_ARRAY: fprintf(stderr,"TAC_READ_ARRAY"); break;
		    case TAC_RETURN: makeReturn(fout, tac); break;
		    
		    //case TAC_ARG: fprintf(stderr,"TAC_ARG"); break;
		    //case TAC_FUNCALL: fprintf(stderr,"TAC_FUNCALL"); break;

		    
		    default: break;
	    }
	}
	
	// HASH TABLE
    printASM(fout, first);
    

    fclose(fout);
}

void makePrint(FILE* fout, TAC* tac){

    fprintf(fout,   "\t## TAC PRINT\n");
    switch(tac->res->datatype){
        case DATATYPE_STRING:   fprintf(fout, 	"\tleaq	_%s%d(%%rip), %%rsi\n"
	                                            "\tleaq	printStr(%%rip), %%rdi\n"
	                                            "\tmovl	$0, %%eax\n"
	                                            "\tcall	printf@PLT\n"
                                                "\n", "str", tac->res->id);
	                                            break;
                                                
        case DATATYPE_INT:      fprintf(fout,   "\tmovl	_%s(%%rip), %%eax\n"
	                                            "\tmovl	%%eax, %%esi\n"
	                                            "\tleaq	printInt(%%rip), %%rdi\n"
	                                            "\tmovl	$0, %%eax\n"
	                                            "\tcall	printf@PLT\n"
                                                "\n", tac->res->text);
	                                            break;
	                                            
        case DATATYPE_FLOAT:    dotFloat(tac->res->text);
                                fprintf(fout,   "\tmovss	_%s(%%rip), %%xmm0\n"
	                                            "\tcvtss2sd	%%xmm0, %%xmm0\n"
	                                            "\tleaq	printFlt(%%rip), %%rdi\n"
	                                            "\tmovl	$1, %%eax\n"
	                                            "\tcall	printf@PLT\n"
                                                "\n", tac->res->text);
	                                            break;
        default: fprintf(fout,"\tTYPE = %d; DATATYPE = %d; TEXT = %s\n\n", tac->res->type, tac->res->datatype, tac->res->text);break; //Consertar DATATYPE das temps
    }
    
}

void makeBeginFun(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC BEGIN FUN\n");
    fprintf(fout, 	    "\t.globl	%s\n"
                    "%s:\n"
	                    "\tpushq	%%rbp\n"
	                    "\tmovq	%%rsp, %%rbp\n"
	                "\n", tac->res->text, tac->res->text);
}

void makeEndFun(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC END FUN\n");
    fprintf(fout, 		"\tleave\n"
	                    "\tret\n"
	                "\n");
}

void makeCopy(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC COPY\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->res->text);
}

void makeAdd(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC ADD\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\taddl	%%edx, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeSub(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC SUB\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tsubl	%%eax, %%edx\n"
	                    "\tmovl	%%edx, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeMul(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC MUL\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\timull	%%edx, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeDiv(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC DIV\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tmovl	_%s(%%rip), %%ecx\n"
	                    "\tcltd\n"
	                    "\tidivl	%%ecx\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeEq(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC EQ\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%eax, %%edx\n"
	                    "\tsete	%%al\n"
	                    "\tmovzbl	%%al, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeDif(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC DIF\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%eax, %%edx\n"
	                    "\tsetne	%%al\n"
	                    "\tmovzbl	%%al, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeGrt(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC GRT\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%eax, %%edx\n"
	                    "\tsetg	%%al\n"
	                    "\tmovzbl	%%al, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeGeq(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC GEQ\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%eax, %%edx\n"
	                    "\tsetge	%%al\n"
	                    "\tmovzbl	%%al, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeLes(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC LES\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%eax, %%edx\n"
	                    "\tsetl	%%al\n"
	                    "\tmovzbl	%%al, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeLeq(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC LEQ\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%eax, %%edx\n"
	                    "\tsetle	%%al\n"
	                    "\tmovzbl	%%al, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeAnd(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC AND\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tandl	%%eax, %%edx\n"
	                    "\tmovl	%%edx, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeOr(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC OR\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tmovl	_%s(%%rip), %%edx\n"
	                    "\torl	%%eax, %%edx\n"
	                    "\tmovl	%%edx, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeJz(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC JZ\n");
    fprintf(fout,       "\tjz   .%s\n"
	                    "\n", tac->res->text);
}

void makeJmp(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC JMP\n");
    fprintf(fout,       "\tjmp   .%s\n"
	                    "\n", tac->res->text);
}

void makeLab(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC LABEL\n");
    fprintf(fout,       ".%s:\n"
	                    "\n", tac->res->text);
}

void makeReturn(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC RETURN\n");
    fprintf(fout,       "\tmovl	$0, %%eax\n"
	                    "\n");
}
