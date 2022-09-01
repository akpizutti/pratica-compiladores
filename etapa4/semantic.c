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
	case AST_SYMBOL:
	if(node->symbol)
	{
		switch(node->symbol->type){
			case SYMBOL_LIT_INT: node->symbol->datatype = DATATYPE_INT; break;
			case SYMBOL_LIT_FLOAT: node->symbol->datatype = DATATYPE_FLOAT; break;
			case SYMBOL_LIT_CHAR: node->symbol->datatype = DATATYPE_CHAR; break;
			case SYMBOL_LIT_STRING: node->symbol->datatype = DATATYPE_STRING; break;
		}
		
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

int is_operation(AST* node)
{
	return ((node->type == AST_ADD) ||
		(node->type == AST_SUB) ||
		(node->type == AST_MUL) ||
		(node->type == AST_DIV)) ;

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

int is_integer(AST* node)
{
       
       if (is_operation(node))
	{
		return(is_integer(node->son[0]) && is_integer(node->son[1]));
	}
	else 
	{
       	return ((node->type == AST_SYMBOL && node->symbol->type == SYMBOL_LIT_INT) ||
		       (node->type == AST_SYMBOL && (node->symbol->type == SYMBOL_VARIABLE && node->symbol->datatype == DATATYPE_INT)) ||
		       (node->type == AST_SYMBOL && (node->symbol->type == SYMBOL_PARAMETER && node->symbol->datatype == DATATYPE_INT)) ||
               (node->type == AST_FUNCTION_CALL && node->symbol->datatype == DATATYPE_INT)) ||               
               is_char(node);
       }
}

int is_char(AST* son)
{
       return ((son->type == AST_SYMBOL && son->symbol->type == SYMBOL_LIT_CHAR) ||
		       (son->type == AST_SYMBOL && (son->symbol->type == SYMBOL_VARIABLE && son->symbol->datatype == DATATYPE_CHAR)) ||
               (son->type == AST_FUNCTION_CALL && son->symbol->datatype == DATATYPE_CHAR));
}

int is_float(AST* son)
{
       return ((son->type == AST_SYMBOL && son->symbol->type == SYMBOL_LIT_FLOAT) ||
			   (son->type == AST_SYMBOL && (son->symbol->type == SYMBOL_VARIABLE && son->symbol->datatype == DATATYPE_FLOAT)) ||
               (son->type == AST_SYMBOL && (son->symbol->type == SYMBOL_PARAMETER && son->symbol->datatype == DATATYPE_FLOAT)) ||
               (son->type == AST_FUNCTION_CALL && son->symbol->datatype == DATATYPE_FLOAT));
}

int is_bool(AST* son)
{
        return ((son->type == AST_LES) ||
                (son->type == AST_GRT) ||
                (son->type == AST_LEQ) ||
                (son->type == AST_GEQ) ||
                (son->type == AST_EQ)  ||
                (son->type == AST_DIF) ||
                (son->type == AST_AND) ||
                (son->type == AST_OR)  ||
                (son->type == AST_NOT));
}



/*
int check_subtree_type(AST* node)
{
	if (is_operation(node))
	{
		if(check_subtree_type(node->son[0]) == check_subtree_type(node->son[1])) && (check_subtree_type(node->son[0]) != -1) && (check_subtree_type(node->son[1]) != -1)
		{
			if(is_integer(node->son[0]))
				return INT;
			if(is_float(node->son[0]))
				return FLOAT;
		}
		else
			return -1;
			
	}
	else 
	{
		if(is_integer(node))
			return INT;
		if(is_float(node))
			return FLOAT;
	}

}
*/
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
			if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for ADD! On expression '%s + %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_SUB:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for SUB! On expression '%s - %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_MUL:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for MUL! On expression '%s . %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_DIV:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for DIV! On expression '%s / %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_LES:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for LES! On expression '%s < %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_GRT:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for GRT! On expression '%s > %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_LEQ:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for LEQ! On expression '%s <= %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_GEQ:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for GEQ! On expression '%s >= %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_EQ:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for EQ! On expression '%s == %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_DIF:
		    if(!((is_integer(node->son[0]) && is_integer(node->son[1])) || (is_float(node->son[0]) && is_float(node->son[1])))){
				fprintf(stderr,"Semantic error: The type of the operands doesn't match for DIF! On expression '%s != %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
			    if(!(is_integer(node->son[0]) && is_integer(node->son[1]))){
			        
			        if(!(is_integer(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an integer!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_integer(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an integer!\n", node->son[1]->symbol->text);
				    }
			    }
			    
			    if(!(is_float(node->son[0]) && is_float(node->son[1]))){
				    
				    if(!(is_float(node->son[0]))){
					    fprintf(stderr,"  - The left operand '%s' is not an float!\n", node->son[0]->symbol->text);
				    }
				    if(!(is_float(node->son[1]))){
					    fprintf(stderr,"  - The right operand '%s' is not an float!\n", node->son[1]->symbol->text);
				    }
			    }
			}
			break;

		case AST_AND:
		    if(!((is_integer(node->son[0]) || is_bool(node->son[0])) && (is_integer(node->son[1]) || is_bool(node->son[1])))){
		        fprintf(stderr,"Semantic error: The type of the operands doesn't match for AND! On expression '%s & %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
				if(!(is_integer(node->son[0]) || is_bool(node->son[0]))){
					fprintf(stderr,"  - The left operand '%s' is not an integer or an boolean!\n", node->son[0]->symbol->text);
				}
				if(!(is_integer(node->son[1]) || is_bool(node->son[1]))){
					fprintf(stderr,"  - The right operand '%s' is not an integer or an boolean!\n", node->son[1]->symbol->text);
				}
			}
			break;

		case AST_OR:
		    if(!((is_integer(node->son[0]) || is_bool(node->son[0])) && (is_integer(node->son[1]) || is_bool(node->son[1])))){
		        fprintf(stderr,"Semantic error: The type of the operands doesn't match for OR! On expression '%s | %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
				if(!(is_integer(node->son[0]) || is_bool(node->son[0]))){
					fprintf(stderr,"  - The left operand '%s' is not an integer or an boolean!\n", node->son[0]->symbol->text);
				}
				if(!(is_integer(node->son[1]) || is_bool(node->son[1]))){
					fprintf(stderr,"  - The right operand '%s' is not an integer or an boolean!\n", node->son[1]->symbol->text);
				}
			}
			break;

		case AST_NOT:
		    if(!((is_integer(node->son[0]) || is_bool(node->son[0])) && (is_integer(node->son[1]) || is_bool(node->son[1])))){
		        fprintf(stderr,"Semantic error: The type of the operands doesn't match for NOT! On expression '%s ~ %s' \n", node->son[0]->symbol->text, node->son[1]->symbol->text);
			    ++SemanticErrors;
			    
				if(!(is_integer(node->son[0]) || is_bool(node->son[0]))){
					fprintf(stderr,"  - The left operand '%s' is not an integer or an boolean!\n", node->son[0]->symbol->text);
				}
				if(!(is_integer(node->son[1]) || is_bool(node->son[1]))){
					fprintf(stderr,"  - The right operand '%s' is not an integer or an boolean!\n", node->son[1]->symbol->text);
				}
			}
			break;
	}


    for(i = 0; i < MAXSONS; ++i){
        check_operands(node->son[i]);
    }
}

void check_array(AST* node) //checa se o nodo é do tipo "acesso ao array" e se o seu índice é um integer. CONSERTAR!!!
{
    int i;
    if(!node || is_leaf(node) )
	{
	//fprintf(stderr, "Node NULL!\n");
	return;
    }
    
    //fprintf(stderr,"Semantic error: The type of the index doesn't match! On expression '%s[%s]' \n", node->symbol->text, node->son[0]->symbol->text);
    //fprintf(stderr,"Son: Type = %d, Symbol Type = %d, Datatype = %d, Text = %s\n", node->son[0]->type, node->son[0]->symbol->type, node->son[0]->symbol->datatype, node->son[0]->symbol->text);
    if(node->type == AST_ARRAY_ACC || node->type == AST_READ_ARRAY){
		if(!(is_integer(node->son[0]))){
	//    	fprintf(stderr, "Deu erro!\n");
			//fprintf(stderr, "Deu erro no %d \n", node->type);
		    fprintf(stderr,"Semantic error: The type of the index doesn't match! On expression '%s[%s]' \n", node->symbol->text, node->son[0]->symbol->text);
		    //fprintf(stderr,"Son: Type = %d, Symbol Type = %d, Datatype = %d, Text = %s\n", node->type, node->symbol->type, node->symbol->datatype, node->symbol->text);
		    //fprintf(stderr,"Son: Type = %d, Symbol Type = %d, Datatype = %d, Text = %s\n", node->son[0]->type, node->son[0]->symbol->type, node->son[0]->symbol->datatype, node->son[0]->symbol->text);
		    ++SemanticErrors;
		}
    }
    
    for(i = 0; i < MAXSONS; ++i){
        check_array(node->son[i]);
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
    fprintf(stderr, "Começando checagem de array!\n");
    check_array(node);
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
