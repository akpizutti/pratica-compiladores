#include "ast.h"
AST* astCreate(int type, HASH_NODE *symbol, AST* son0, AST* son1, AST* son2, AST* son3){
    AST* newnode;

    newnode = (AST*) calloc(1, sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son0;
    newnode->son[1] = son1;
    newnode->son[2] = son2;
    newnode->son[3] = son3;

    return newnode;
}

void astPrint(AST* node, int level){
    int i;

    if(!node){
	//fprintf(stderr, "Node NULL!\n");
	return;
    }

    for(i = 0; i < level; ++i){
        fprintf(stderr, " ");
    }

    fprintf(stderr, "AST  ");

    switch(node->type){
        case AST_SYMBOL:  		fprintf(stderr, "AST_SYMBOL"); break;
        case AST_ADD: 	  		fprintf(stderr, "AST_ADD"); break;
	case AST_SUB: 	  		fprintf(stderr, "AST_SUB"); break;
	case AST_MUL: 	  		fprintf(stderr, "AST_MUL"); break;
	case AST_DIV: 	  		fprintf(stderr, "AST_DIV"); break;
	case AST_LES: 	  		fprintf(stderr, "AST_LES"); break;
	case AST_GRT: 	  		fprintf(stderr, "AST_GRT"); break;
	case AST_LEQ: 	  		fprintf(stderr, "AST_LEQ"); break;
	case AST_GEQ: 	  		fprintf(stderr, "AST_GEQ"); break;
	case AST_EQ: 	  		fprintf(stderr, "AST_EQ"); break;
	case AST_DIF: 	  		fprintf(stderr, "AST_DIF"); break;
	case AST_AND: 	  		fprintf(stderr, "AST_AND"); break;
	case AST_OR: 	  		fprintf(stderr, "AST_OR"); break;
	case AST_NOT: 	  		fprintf(stderr, "AST_NOT"); break;
	case AST_IF: 	  		fprintf(stderr, "AST_IF"); break;
	case AST_IF_ELSE: 		fprintf(stderr, "AST_IF_ELSE"); break;
	case AST_WHILE:   		fprintf(stderr, "AST_WHILE"); break;
	case AST_PROGRAM: 	  	fprintf(stderr, "AST_PROGRAM"); break;
	case AST_LIST: 	  		fprintf(stderr, "AST_LIST"); break;
	case AST_DECLARATION: 	  	fprintf(stderr, "AST_DECLARATION"); break;
	case AST_VARIABLE: 	  	fprintf(stderr, "AST_VARIABLE"); break;
	case AST_ARRAY_VAL_LIST: 	fprintf(stderr, "AST_ARRAY_VAL_LIST"); break;
	case AST_FUNCTION: 	  	fprintf(stderr, "AST_FUNCTION"); break;
	case AST_PARAMETERS: 	  	fprintf(stderr, "AST_PARAMETERS"); break;
	case AST_BLOCK: 	  	fprintf(stderr, "AST_BLOCK"); break;
	case AST_COMMAND_LIST: 	  	fprintf(stderr, "AST_COMMAND_LIST"); break;
	case AST_COMMAND_LIST_TAIL: 	fprintf(stderr, "AST_COMMAND_LIST_TAIL"); break;
	case AST_COMMAND: 	  	fprintf(stderr, "AST_COMMAND"); break;
	case AST_ASSIGNMENT: 	  	fprintf(stderr, "AST_ASSIGNMENT"); break;
	case AST_READ: 	  		fprintf(stderr, "AST_READ"); break;
	case AST_PRINT: 	  	fprintf(stderr, "AST_PRINT"); break;
	case AST_ELEMENT_LIST: 	  	fprintf(stderr, "AST_ELEMENT_LIST"); break;
	case AST_PRINT_ARGUMENT_STR: 	fprintf(stderr, "AST_PRINT_ARGUMENT_STR"); break;
	case AST_PRINT_ARGUMENT_EXPR: 	fprintf(stderr, "AST_PRINT_ARGUMENT_EXPR"); break;
	case AST_RETURN: 	  	fprintf(stderr, "AST_RETURN"); break;
	case AST_FUNCTION_CALL: 	fprintf(stderr, "AST_FUNCTION_CALL"); break;
	case AST_ARRAY_ACC: 	  	fprintf(stderr, "AST_ARRAY_ACC"); break;
	case AST_ASSIGNMENT_ARRAY: 	  	fprintf(stderr, "AST_ASSIGNMENT_ARRAY"); break;
	case AST_VARIABLE_ARRAY: 	  	fprintf(stderr, "AST_VARIABLE_ARRAY"); break;
	case AST_VALUE: 	  	fprintf(stderr, "AST_VALUE"); break;
	case AST_ARGUMENT_LIST: 	fprintf(stderr, "AST_ARGUMENT_LIST"); break;
	case AST_KW_INT: fprintf(stderr, "AST_KW_INT"); break;
	case AST_KW_FLOAT: fprintf(stderr, "AST_KW_FLOAT"); break;
	case AST_KW_CHAR: fprintf(stderr, "AST_CHAR"); break;
	case AST_READ_ARRAY: 	  		fprintf(stderr, "AST_READ_ARRAY"); break;
	case AST_NESTED_EXPR: 	  		fprintf(stderr, "AST_NESTED_EXPR"); break;
        default: fprintf(stderr, "UNKNOWN = %d", node->type); break;
    }

    if(node->symbol){
        fprintf(stderr, ",%s\n", node->symbol->text);
    }
    else{
        fprintf(stderr, "\n");
    }

    for(i = 0; i < MAXSONS; ++i){
        astPrint(node->son[i], level+1);
    }
}

void descompila(FILE* saida, AST* node)
{
    int i;
    int recur = 1;

    if(!node){
	//fprintf(saida, "Node NULL!\n");
	return;
    }

    switch(node->type){
    case AST_SYMBOL:  		fprintf(saida, "%s", node->symbol->text); return;
    case AST_ADD: 	  	    descompila(saida, node->son[0]);
                            fprintf(saida, " + ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_SUB: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " - ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_MUL: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " . ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_DIV: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " / ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_LES: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " < ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_GRT: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " > ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_LEQ: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " <= ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_GEQ: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " >= ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_EQ: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " == ");
                            descompila(saida, node->son[1]);
                             break;

	case AST_DIF: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " != ");
                            descompila(saida, node->son[1]);
                            break;

	case AST_AND: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " & ");
                            descompila(saida, node->son[1]);
                            break;

	case AST_OR: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " | ");
                            descompila(saida, node->son[1]);
                            break;

	case AST_NOT: 	  		descompila(saida, node->son[0]);
                            fprintf(saida, " ~ ");
                            descompila(saida, node->son[1]);
                            break;

	case AST_IF: 	  		fprintf(saida, "if (");
                            descompila(saida,node->son[0]) ;
                            fprintf(saida, ") \n");
                            descompila(saida,node->son[1]) ;
                            break;

	case AST_IF_ELSE: 		fprintf(saida, "if (");
                            descompila(saida,node->son[0]) ;
                            fprintf(saida, ") \n");
                            descompila(saida,node->son[1]) ;
                            fprintf(saida, "\nelse \n");
                            descompila(saida,node->son[2]) ;
                            break;

	case AST_WHILE:   		fprintf(saida, "while (");
                            descompila(saida,node->son[0]) ;
                            fprintf(saida, ") \n");
                            descompila(saida,node->son[1]) ;
                            fprintf(saida, "\n");
                            break;

	case AST_PROGRAM: 	  	descompila(saida,node->son[0]) ;
                            break;

	case AST_LIST: 	  		descompila(saida,node->son[0]);
                            fprintf(saida, "\n");
                            descompila(saida,node->son[1]);
                            break;

	case AST_DECLARATION: 	descompila(saida,node->son[0]);

                            break;

	case AST_VARIABLE: 	  	descompila(saida,node->son[0]);
                            fprintf(saida, " %s ( ", node->symbol->text);
                            descompila(saida,node->son[1]);
                            fprintf(saida, ") ;\n");
                            break;
    case AST_VARIABLE_ARRAY:
                            descompila(saida,node->son[0]);
                            fprintf(saida, " %s [ ", node->symbol->text);
                            descompila(saida,node->son[1]);
                            fprintf(saida, "] ");
                            descompila(saida,node->son[2]);
                            fprintf(saida, "; ");
                            break;
	case AST_ARRAY_VAL_LIST:
                            descompila(saida,node->son[0]);
                            fprintf(saida, " ");
                            descompila(saida,node->son[1]);
                            break;

	case AST_FUNCTION: 	  	descompila(saida,node->son[0]);
                            fprintf(saida, " %s ( ", node->symbol->text);
                            descompila(saida,node->son[1]);
                            fprintf(saida, ") \n");
                            descompila(saida,node->son[2]);
                            break;

	case AST_PARAMETERS: 	descompila(saida,node->son[0]);
                            fprintf(saida, "%s ", node->symbol->text);
                            descompila(saida,node->son[1]);
                            break;

	case AST_BLOCK: 	  	fprintf(saida, "{ \n");
                            descompila(saida,node->son[0]);
                            fprintf(saida, "} \n");
                            break;

	case AST_COMMAND_LIST: 	descompila(saida,node->son[0]);
                            descompila(saida,node->son[1]);
                            break;
	case AST_COMMAND_LIST_TAIL:
                            fprintf(saida, "; \n");
                            descompila(saida,node->son[0]);
                            descompila(saida,node->son[1]);
                            break;

	case AST_COMMAND: 	  	descompila(saida,node->son[0]);
                            break;

	case AST_ASSIGNMENT:    fprintf(saida, "%s <- ", node->symbol->text);
                            descompila(saida,node->son[0]);
                            break;
    case AST_ASSIGNMENT_ARRAY:
                            fprintf(saida, "%s [ ", node->symbol->text);
                            descompila(saida,node->son[0]);
                            fprintf(saida, "] <- ");
                            descompila(saida,node->son[1]);
                            break;

	case AST_READ: 	  		fprintf(saida,"read %s", node->symbol->text);
                            break;

	case AST_READ_ARRAY: 	fprintf(saida,"read %s[", node->symbol->text);
                            descompila(saida,node->son[0]);
                            fprintf(saida,"]");
                            break;

	case AST_PRINT: 	  	fprintf(saida, "print ");
                            descompila(saida,node->son[0]);
                            break;
	case AST_ELEMENT_LIST:
                            descompila(saida,node->son[0]);
                            descompila(saida,node->son[1]);
                            break;
	case AST_PRINT_ARGUMENT_STR:
                            fprintf(saida,"%s", node->symbol->text);
                            break;
	case AST_PRINT_ARGUMENT_EXPR:
                            descompila(saida,node->son[0]);
                            break;

	case AST_RETURN: 	  	fprintf(saida, "return ");
                            descompila(saida,node->son[0]);
                            break;

    case AST_NESTED_EXPR:   fprintf(saida, "(");
                            descompila(saida,node->son[0]);
                            fprintf(saida, ")");
                            break;
	case AST_FUNCTION_CALL:
                            fprintf(saida,"%s(", node->symbol->text);
                            descompila(saida,node->son[0]);
                            fprintf(saida,")");
                            break;

	case AST_ARRAY_ACC:   	fprintf(saida,"%s[", node->symbol->text);
                            descompila(saida,node->son[0]);
                            fprintf(saida,"]");
                            break;

	case AST_VALUE: 	  	descompila(saida,node->son[0]);  break;
	case AST_ARGUMENT_LIST: descompila(saida,node->son[0]);
                            fprintf(saida," ");
                            descompila(saida,node->son[1]);
                            break;
	case AST_KW_INT: fprintf(saida, "int "); break;
	case AST_KW_FLOAT: fprintf(saida, "float "); break;
	case AST_KW_CHAR: fprintf(saida, "char "); break;
        default: fprintf(saida, "UNKNOWN\n"); break;
    }

//    if(recur==1){
//        for(i = 0; i < MAXSONS; ++i){
//            descompila(saida, node->son[i]);
//        }
//    }
}


void testabc(void)
{
	fprintf(stderr,"ola");
}
