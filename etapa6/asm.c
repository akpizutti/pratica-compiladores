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

void cleanArray(char* array, int size){
    int i;
    for(i = 0; i < size; i++){
        array[i] = 0;
    }
}

void printASM(FILE* fout, TAC* first){
    int i;
    char array_name[256];
	HASH_NODE *node;
	TAC* tac = first;

    fprintf(fout,   "## DATA SECTION\n"
                        "\t.data\n"
                        "\t.align\t4\n");
    for(; tac; tac = tac->next){
        if(tac->type == TAC_DECLARATION){
            switch(tac->res->datatype){
                case DATATYPE_INT:  fprintf(fout, "_%s:\t.int\t%s\n", tac->res->text, tac->op1->text); break;
                case DATATYPE_CHAR:  fprintf(fout, "_%s:\t.byte\t%d\n", tac->res->text, (int)tac->op1->text[1]); break;
                case DATATYPE_FLOAT: dotFloat(tac->op1->text);
                                     fprintf(fout, "_%s:\t.float\t%s\n", tac->res->text, tac->op1->text); break;
                default: break;
            }
            
        }
        else if(tac->type == TAC_SET_ARR){
            if(strcmp(tac->res->text, array_name) != 0){
                strncpy(array_name, tac->res->text, strlen(tac->res->text));
                fprintf(fout, "_%s:\t.long\t%s\n", tac->res->text, tac->op1->text);
            }
            else{
                fprintf(fout, "\t.long\t%s\n", tac->op1->text);
            }
        }
        else if(tac->type == TAC_ARG){
            fprintf(fout, "_%s:\t.zero\t4\n", tac->res->text);
        }
        
        if(tac->type != TAC_SET_ARR){
            cleanArray(array_name, 256);
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
		    case TAC_ARRAY_ACC: makeArrayAcc(fout, tac); break;
		    //case TAC_READ: fprintf(stderr,"TAC_READ"); break;
		    //case TAC_READ_ARRAY: fprintf(stderr,"TAC_READ_ARRAY"); break;
		    case TAC_RETURN: makeReturn(fout, tac); break;
		    
		    case TAC_ARG: makeArg(fout, tac); break;
		    case TAC_FUNCALL: makeFunctionCall(fout, tac); break;
		    
		    //Usadas no data section
    		//case TAC_SET_ARR: fprintf(stderr,"TAC_SET_ARR"); break;
		    //case TAC_DECLARATION: fprintf(stderr,"TAC_DECLARATION"); break;
		    //case TAC_DECLARATION_ARR: fprintf(stderr,"TAC_DECLARATION"); break;

		    
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
	                                            
	    case DATATYPE_CHAR:      fprintf(fout,  "\tmovzbl	_%s(%%rip), %%eax\n"
	                                            "\tmovsbl	%%al, %%eax\n"
	                                            "\tmovl	%%eax, %%edi\n"
	                                            "\tcall	putchar@PLT\n"
                                                "\n", tac->res->text);
	                                            break;
        default: fprintf(fout,"\t##TYPE = %d; DATATYPE = %d; TEXT = %s\n\n", tac->res->type, tac->res->datatype, tac->res->text);break; //Consertar DATATYPE das temps
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
    if(tac->op2){
        fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                        "\tmovl	_%s(%%rip), %%edx\n"
	                        "\tcltq\n"
	                        "\tleaq	0(,%%rax,4), %%rcx\n"
	                        "\tleaq	_%s(%%rip), %%rax\n"
	                        "\tmovl	%%edx, (%%rcx,%%rax)\n"
	                        "\n", tac->op2->text, tac->op1->text, tac->res->text);
    }
	else{
	    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                        "\tmovl	%%eax, _%s(%%rip)\n"
	                        "\n", tac->op1->text, tac->res->text);
	}
}

void makeArg(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC ARG\n");
    if(tac->op2){
        fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                        "\tmovl	_%s(%%rip), %%edx\n"
	                        "\tcltq\n"
	                        "\tleaq	0(,%%rax,4), %%rcx\n"
	                        "\tleaq	_%s(%%rip), %%rax\n"
	                        "\tmovl	%%edx, (%%rcx,%%rax)\n"
	                        "\n", tac->op2->text, tac->op1->text, tac->res->text);
    }
	else{
	    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                        "\tmovl	%%eax, _%s(%%rip)\n"
	                        "\n", tac->op1->text, tac->res->text);
	}
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
	                    "\tcmpl	%%edx, %%eax\n"
	                    "\tsete	%%al\n"
	                    "\tmovzbl	%%al, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeDif(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC DIF\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%edx, %%eax\n"
	                    "\tsetne	%%al\n"
	                    "\tmovzbl	%%al, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeGrt(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC GRT\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%edx, %%eax\n"
	                    "\tsetg	%%al\n"
	                    "\tmovzbl	%%al, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeGeq(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC GEQ\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%edx, %%eax\n"
	                    "\tsetge	%%al\n"
	                    "\tmovzbl	%%al, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeLes(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC LES\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%edx, %%eax\n"
	                    "\tsetl	%%al\n"
	                    "\tmovzbl	%%al, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->op2->text, tac->res->text);
}

void makeLeq(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC LEQ\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%edx\n"
	                    "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcmpl	%%edx, %%eax\n"
	                    "\tsetle	%%al\n"
	                    "\tmovzbl	%%al, %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
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
    fprintf(fout,       "\tmovl    $0, %%edx\n"
	                    "\tmovl    _%s(%%rip), %%eax\n"
	                    "\tcmpl    %%eax, %%edx\n"
	                    "\tjz   .%s\n"
	                    "\n", tac->op1->text, tac->res->text);
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
    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                    "\n", tac->res->text);
}

void makeArrayAcc(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC ARRAY ACC\n");
    fprintf(fout,       "\tmovl	_%s(%%rip), %%eax\n"
	                    "\tcltq\n"
	                    "\tleaq	0(,%%rax,4), %%rdx\n"
	                    "\tleaq	_%s(%%rip), %%rax\n"
	                    "\tmovl	(%%rdx,%%rax), %%eax\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op2->text, tac->op1->text, tac->res->text);
}

void makeFunctionCall(FILE* fout, TAC* tac){
    fprintf(fout,   "\t## TAC FUN CALL\n");
    fprintf(fout,       "\tcall\t%s\n"
                    	"\taddq	$80, %%rsp\n"
	                    "\tmovl	%%eax, _%s(%%rip)\n"
	                    "\n", tac->op1->text, tac->res->text);
}


