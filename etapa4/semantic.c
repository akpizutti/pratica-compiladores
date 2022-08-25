#include "semantic.h"

int SemanticErrors = 0;

void check_and_set_declarations(AST* node)
{
    int i;
    if(!node){
	//fprintf(stderr, "Node NULL!\n");
	return;
    }

    switch (node->type)
	{
	case AST_VARIABLE:
	if(node->symbol)
	{
		if(node->symbol->type != SYMBOL_IDENTIFIER)
		{
		fprintf(stderr,"Semantic error: variable %s already declared\n",node->symbol->text);
		++SemanticErrors;
		}
		node->symbol->type = SYMBOL_VARIABLE;
		node->symbol->datatype = convert_ast_type(node);
	}
		break;
	case AST_FUNCTION:
	if(node->symbol)
	{
		if(node->symbol->type != SYMBOL_IDENTIFIER)
		{
		fprintf(stderr,"Semantic error: function %s already declared\n",node->symbol->text);
		++SemanticErrors;
		}
		node->symbol->type = SYMBOL_FUNCTION;
		node->symbol->datatype = convert_ast_type(node);
	}
		break;
	case AST_VARIABLE_ARRAY:
	if(node->symbol)
	{
		if(node->symbol->type != SYMBOL_IDENTIFIER)
		{
		fprintf(stderr,"Semantic error: array %s already declared\n",node->symbol->text);
		++SemanticErrors;
		}
		node->symbol->type = SYMBOL_ARRAY;
		node->symbol->datatype = convert_ast_type(node);
	}
		break;
	case AST_PARAMETERS:
	if(node->symbol)
	{
		if(node->symbol->type != SYMBOL_IDENTIFIER)
		{
		fprintf(stderr,"Semantic error: parameter %s already declared\n",node->symbol->text);
		++SemanticErrors;
		}
		node->symbol->type = SYMBOL_PARAMETER;
		node->symbol->datatype = convert_ast_type(node);
	}
		break;
	}

    for(i = 0; i < MAXSONS; ++i){
        check_and_set_declarations(node->son[i]);
    }
}
void check_undeclared()
{
    SemanticErrors += hash_check_undeclared();
}

int is_number(AST* son)
{
       return
        (	son->type == AST_ADD ||
        	son->type == AST_SUB ||
        	son->type == AST_DIV ||
        	son->type == AST_MUL ||
        	is_integer(son)		 ||
			is_float(son)
        );
}

int is_integer(AST* son)
{
       return
           (son->type == AST_SYMBOL && 
		   ((son->symbol->type == SYMBOL_LIT_INT) || (son->symbol->type == SYMBOL_VARIABLE && son->symbol->datatype == DATATYPE_INT) )) ||
           (son->type == AST_FUNCTION_CALL && son->symbol->datatype == DATATYPE_INT);
}

int is_float(AST* son)
{
       return
           (son->type == AST_SYMBOL && 
		   ((son->symbol->type == SYMBOL_LIT_FLOAT) || (son->symbol->type == SYMBOL_VARIABLE && son->symbol->datatype == DATATYPE_FLOAT) )) ||
           (son->type == AST_FUNCTION_CALL && son->symbol->datatype == DATATYPE_FLOAT);
}

int is_leaf(AST* node)
{
	return (!node->son[0] && !node->son[1] && !node->son[2] && !node->son[3] && !node->son[4] ); 
}

void check_operands(AST* node)
{

	int i;
    if(!node || is_leaf(node))
	{
	//fprintf(stderr, "Node NULL!\n");
	return;
    }

    switch (node->type){
		case AST_ADD:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for ADD on expression %s + %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for ADD on expression %s + %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for ADD! On expression %s + %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for ADD! On expression %s + %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_SUB:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for SUB on expression %s - %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for SUB on expression %s - %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for SUB! On expression %s - %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for SUB! On expression %s - %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_MUL:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for MUL on expression %s . %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for MUL on expression %s . %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for MUL! On expression %s . %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for MUL! On expression %s . %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_DIV:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for DIV on expression %s / %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for DIV on expression %s + %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for DIV! On expression %s / %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for DIV! On expression %s / %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_LES:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for LES on expression %s < %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for LES on expression %s < %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for LES! On expression %s < %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for LES! On expression %s < %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_GRT:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for GRT on expression %s > %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for GRT on expression %s > %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for GRT! On expression %s > %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for GRT! On expression %s > %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_LEQ:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for LEQ on expression %s <= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for LEQ on expression %s <= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for LEQ! On expression %s <= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for LEQ! On expression %s <= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_GEQ:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for GEQ on expression %s >= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for GEQ on expression %s >= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for GEQ! On expression %s >= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for GEQ! On expression %s >= %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_EQ:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for EQ on expression %s == %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for EQ on expression %s == %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for EQ! On expression %s == %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for EQ! On expression %s == %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_DIF:
		    if(!(is_number(node->son[0]) && is_number(node->son[1]))){
				if(!(is_number(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for DIF on expression %s != %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_number(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for DIF on expression %s != %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			else{
				if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for DIF! On expression %s != %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_float(node->son[0]) && is_float(node->son[1]))){
					fprintf(stderr,"Semantic error: The type of the operands doesn't match for DIF! On expression %s != %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				++SemanticErrors;
			}
			break;

		case AST_AND:
		    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
				if(!(is_integer(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for AND on expression %s & %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for AND on expression %s & %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			break;

		case AST_OR:
		    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
				if(!(is_integer(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for OR on expression %s | %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for OR on expression %s | %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			break;

		case AST_NOT:
		    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
				if(!(is_integer(node->son[0]))){
					fprintf(stderr,"Semantic error: Invalid left operand for NOT on expression %s ~ %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
				if(!(is_integer(node->son[1]))){
					fprintf(stderr,"Semantic error: Invalid right operand for NOT on expression %s ~ %s \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
				}
		        ++SemanticErrors;
			}
			break;
	}


    for(i = 0; i < MAXSONS; ++i){
        check_operands(node->son[i]);
    }
}

int get_semantic_errors()
{
    return SemanticErrors;
}

int convert_ast_type(AST* node)
{
    switch (node->son[0]->type)
    {
        case AST_KW_INT:
            return DATATYPE_INT;
        case AST_KW_FLOAT:
            return DATATYPE_FLOAT;
        case AST_KW_CHAR:
            return DATATYPE_CHAR;
        default:
            return -1;
    }
}

void verifySemantic(AST* node)
{
    check_and_set_declarations(node);
    check_undeclared();
    check_operands(node);
    //outras verificações

    return ;
}


/*
node->son[0]->type == AST_ADD ||
node->son[0]->type == AST_SUB ||
node->son[0]->type == AST_DIV ||
node->son[0]->type == AST_MUL ||
(node->son[0]->type == AST_SYMBOL && node->son[0]->symbol == LIT_INTEGER) ||
(node->son[0]->type == AST_FUNCTION_CALL && node->son[0]->symbol == LIT_INTEGER)
*/
