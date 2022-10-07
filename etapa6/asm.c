#include "asm.h"

//int print_str_label = 0;

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
		    //case TAC_ADD: fprintf(stderr,"TAC_ADD"); break;
		    //case TAC_SUB: fprintf(stderr,"TAC_SUB"); break;
		    //case TAC_MUL: fprintf(stderr,"TAC_MUL"); break;
		    //case TAC_DIV: fprintf(stderr,"TAC_DIV"); break;
		    //case TAC_LES: fprintf(stderr,"TAC_LES"); break;
		    //case TAC_GRT: fprintf(stderr,"TAC_GRT"); break;
		    //case TAC_LEQ: fprintf(stderr,"TAC_LEQ"); break;
		    //case TAC_GEQ: fprintf(stderr,"TAC_GEQ"); break;
		    //case TAC_EQ: fprintf(stderr,"TAC_EQ"); break;
		    //case TAC_DIF: fprintf(stderr,"TAC_DIF"); break;
		    //case TAC_AND: fprintf(stderr,"TAC_AND"); break;
		    //case TAC_OR: fprintf(stderr,"TAC_OR"); break;
		    //case TAC_NOT: fprintf(stderr,"TAC_NOT"); break;
		    
		    //case TAC_COPY: fprintf(stderr,"TAC_COPY"); break;
		    //case TAC_JZ: fprintf(stderr,"TAC_JZ"); break;
		    //case TAC_JMP: fprintf(stderr,"TAC_JMP"); break;
		    //case TAC_LABEL: fprintf(stderr,"TAC_LABEL"); break;
		    
		    case TAC_BEGINFUN: makeBeginFun(fout, tac); break;
		    case TAC_ENDFUN: makeEndFun(fout, tac); break;
		    case TAC_PRINT: makePrint(fout, tac); break;
		    //case TAC_ARRAY_ACC: fprintf(stderr,"TAC_ARRAY_ACC"); break;
		    //case TAC_READ: fprintf(stderr,"TAC_READ"); break;
		    //case TAC_READ_ARRAY: fprintf(stderr,"TAC_READ_ARRAY"); break;
		    //case TAC_RETURN: fprintf(stderr,"TAC_RETURN"); break;
		    
		    //case TAC_ARG: fprintf(stderr,"TAC_ARG"); break;
		    //case TAC_FUNCALL: fprintf(stderr,"TAC_FUNCALL"); break;

		    
		    default: break;
	    }
	}
	
	// HASH TABLE
    printASM(fout);
    

    fclose(fout);
}

void makePrint(FILE* fout, TAC* tac){

    fprintf(fout,   "## TAC PRINT\n");
    switch(tac->res->datatype){
        case DATATYPE_STRING:   fprintf(fout, 	"\tmovl	_%s(%%rip), %%eax\n"
	                                            "\tmovl	%%eax, %%esi\n"
	                                            "\tleaq	printStr(%%rip), %%rdi\n"
	                                            "\tmovl	$0, %%eax\n"
	                                            "\tcall	printf@PLT\n"
                                                "\n", tac->res->text);
	                                            break;
                                                
        case DATATYPE_INT:      fprintf(fout,   "\tmovl	_%s(%%rip), %%eax\n"
	                                            "\tmovl	%%eax, %%esi\n"
	                                            "\tleaq	printInt(%%rip), %%rdi\n"
	                                            "\tmovl	$0, %%eax\n"
	                                            "\tcall	printf@PLT\n"
                                                "\n", tac->res->text);
	                                            break;
	                                            
        case DATATYPE_FLOAT:    fprintf(fout,   "\tmovl	_%s(%%rip), %%eax\n"
	                                            "\tmovl	%%eax, %%esi\n"
	                                            "\tleaq	printFlt(%%rip), %%rdi\n"
	                                            "\tmovl	$0, %%eax\n"
	                                            "\tcall	printf@PLT\n"
                                                "\n", tac->res->text);
	                                            break;
        default: fprintf(fout,"\tDATATYPE = %d\n\n", tac->res->datatype);break; //Consertar DATATYPE das temps
    }
    
}

void makeBeginFun(FILE* fout, TAC* tac){
    fprintf(fout,   "## TAC BEGIN FUN\n");
    fprintf(fout, 	    "\t.globl	%s\n"
                    "%s:\n"
	                    "\tpushq	%%rbp\n"
	                    "\tmovq	%%rsp, %%rbp\n"
	                "\n", tac->res->text, tac->res->text);
}

void makeEndFun(FILE* fout, TAC* tac){
    fprintf(fout,   "## TAC END FUN\n");
    fprintf(fout, 		"\tleave\n"
	                    "\tret\n"
	                "\n");
}
