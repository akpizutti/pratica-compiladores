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

int is_aritmetic_operation(AST* node)
{
	return ((node->type == AST_ADD) ||
		    (node->type == AST_SUB) ||
		    (node->type == AST_MUL) ||
		    (node->type == AST_DIV)) ;

}

int is_boolean_operation(AST* node)
{
	return ((node->type == AST_LES) ||
		    (node->type == AST_GRT) ||
		    (node->type == AST_LEQ) ||
		    (node->type == AST_GEQ) ||
		    (node->type == AST_LEQ) ||
		    (node->type == AST_EQ)  ||
		    (node->type == AST_DIF) ||
		    (node->type == AST_AND) ||
		    (node->type == AST_OR)  ||
		    (node->type == AST_NOT)) ;

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
        return ((node->type == AST_SYMBOL && node->symbol->type == SYMBOL_LIT_INT) ||
                (node->type == AST_SYMBOL && (node->symbol->type == SYMBOL_VARIABLE && node->symbol->datatype == DATATYPE_INT)) ||
                (node->type == AST_SYMBOL && (node->symbol->type == SYMBOL_PARAMETER && node->symbol->datatype == DATATYPE_INT)) ||
                (node->type == AST_FUNCTION_CALL && node->symbol->datatype == DATATYPE_INT) ||               
                is_char(node));
}

int types_match_aritmetic(AST* node)
{
    int type_son0;
    int type_son1;
    
    if(is_integer(node)){
        return DATATYPE_INT;
    }
    else if(is_float(node)){
        return DATATYPE_FLOAT;
    }
    else if(is_aritmetic_operation(node)){
        type_son0 = types_match_aritmetic(node->son[0]);
        type_son1 = types_match_aritmetic(node->son[1]);
        
        if(type_son0 == type_son1){
            return type_son0;
        }
        else{
            return 0;
        }
    }
}

int types_match_boolean(AST* node)
{
    int type_son0;
    int type_son1;
    
    if(is_integer(node) || is_bool(node)){
        return DATATYPE_INT;
    }
    else if(is_boolean_operation(node)){
        type_son0 = types_match_boolean(node->son[0]);
        type_son1 = types_match_boolean(node->son[1]);
        
        if(type_son0 == type_son1){
            return type_son0;
        }
        else{
            return 0;
        }
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

int is_leaf(AST* node)
{
	return (!node->son[0] && !node->son[1] && !node->son[2] && !node->son[3] && !node->son[4] ); 
}

void check_operands(AST* node)
{

	int i;
	int son0, son1 = -1;
    if(!node || is_leaf(node))
	{
	//fprintf(stderr, "Node NULL!\n");
	return;
    }

    switch (node->type){
		case AST_ADD:
		    son0 = types_match_aritmetic(node->son[0]);
		    son1 = types_match_aritmetic(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in ADD! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_SUB:
		    son0 = types_match_aritmetic(node->son[0]);
		    son1 = types_match_aritmetic(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in SUB! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_MUL:
		    son0 = types_match_aritmetic(node->son[0]);
		    son1 = types_match_aritmetic(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in MUL! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_DIV:
		    son0 = types_match_aritmetic(node->son[0]);
		    son1 = types_match_aritmetic(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in DIV! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_LES:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in LES! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_GRT:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in GRT! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_LEQ:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in LEQ! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_GEQ:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in GEQ! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_EQ:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in EQ! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_DIF:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in DIF! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_AND:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in AND! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_OR:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in OR! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;

		case AST_NOT:
		    son0 = types_match_boolean(node->son[0]);
		    son1 = types_match_boolean(node->son[1]);
			if(son0 != son1){
				fprintf(stderr,"Semantic error in NOT! Trying to operate a datatype '%d' with a datatype '%d'\n", son0, son1);
				
			    ++SemanticErrors;
			}
			break;
	}


    for(i = 0; i < MAXSONS; ++i){
        check_operands(node->son[i]);
    }
}

void check_array(AST* node)
{
    int i;
    if(!node || is_leaf(node) )
	{
	
	return;
    }
    
    if(node->type == AST_ARRAY_ACC || node->type == AST_READ_ARRAY){
		if(!(is_integer(node->son[0]))){
		    fprintf(stderr,"Semantic error: The type of the index doesn't match! On expression '%s[%s]' \n", node->symbol->text, node->son[0]->symbol->text);
		    ++SemanticErrors;
		}
    }
    
    for(i = 0; i < MAXSONS; ++i){
        check_array(node->son[i]);
    }
}

void check_correct_use(AST* node)
{
    int i;
    if(!node || is_leaf(node) )
	{
	
	return;
    }
    
    switch(node->type){
    
        case AST_ASSIGNMENT_ARRAY:
            if(node->symbol->type != SYMBOL_ARRAY){
                fprintf(stderr,"Semantic error: '%s' is not an array! \n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
            
        case AST_ASSIGNMENT:
            if(node->symbol->type != SYMBOL_VARIABLE){
                fprintf(stderr,"Semantic error: '%s' is not a variable! \n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
        
        case AST_FUNCTION_CALL:
            if(node->symbol->type != SYMBOL_FUNCTION){
                fprintf(stderr,"Semantic error: '%s' is not a function call! \n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
        
        case AST_ARRAY_ACC:
            if(node->symbol->type != SYMBOL_ARRAY){
                fprintf(stderr,"Semantic error: '%s' is not an array! \n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
            
        case AST_SYMBOL:
            if(node->symbol->type != SYMBOL_VARIABLE &&
               node->symbol->type != SYMBOL_LIT_INT  &&
               node->symbol->type != SYMBOL_LIT_CHAR &&
               node->symbol->type != SYMBOL_LIT_FLOAT &&
               node->symbol->type != SYMBOL_IDENTIFIER &&
               node->symbol->type != SYMBOL_PARAMETER ){
                fprintf(stderr,"Semantic error: '%s' is not a variable! \n", node->symbol->text);
                ++SemanticErrors;
            }
            break;
        
    }
    
    for(i = 0; i < MAXSONS; ++i){
        check_correct_use(node->son[i]);
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
    //hashPrint();
    check_undeclared();
    check_operands(node);
    //fprintf(stderr, "Começando checagem de array!\n");
    check_array(node);
    check_correct_use(node);
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
