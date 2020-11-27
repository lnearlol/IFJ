#include "parser.h"

char framePrint[3];

 void GEN_START_MAIN(){
	printf(".IFJcode20\n");
    printf("GF@_\n");

    // Global variables for concatenation processing
    printf("DEFVAR GF@str1\n");
    printf("DEFVAR GF@str2\n");
    printf("DEFVAR GF@strRes\n");

    // If divide by zero
    printf("DEFVAR GF@tmpDividingByZero\n");
    printf("JUMP enderror9\n");
    printf("LABEL error9\n");
    printf("EXIT 9\n");
    printf("LABEL enderror9\n");


	printf("JUMP $$main\n");

 }

 void GEN_START_OF_FUNCTION(Token *token){
    if(strcmp(token->data, "main")){
        printf("LABEL $%s\n", token->data);
        printf("PUSHFRAME \n\n");
    } else {
        printf("LABEL $$main\n");
	    printf("CREATEFRAME\n");
        printf("PUSHFRAME\n\n");
    }
 }

  void GEN_END_OF_FUNCTION(Token *token){
    if(!strcmp(token->data, "main")){
	    printf("POPFRAME\n\n");
    } else {
        printf("POPFRAME \n");
        printf("RETURN \n\n");
    }
 }

void GEN_CREATE_FRAME_AND_SET_PARAMS(inputParams InParams){
    
    printf("CREATEFRAME\n");
    while(InParams != NULL){
        printf("DEFVAR TF@%s$0\n", InParams->name);
        InParams = InParams->next;
    }
}

void MOVE_INTO_INPUT_PARAMETER(inputParams InParam, Token *value, int deep){
    printf("MOVE TF@%s$0 ", InParam->name);
    GEN_WRITE_VAR_LITERAL(value, deep);
    printf("\n");
}

void GEN_CALL(Token *token){
   printf("CALL $%s\n", token->data);
}


void GEN_SET_FRAME_TYPE(){
    genFrameType currentFrame = GET_GEN_FRAME();
    
    switch (currentFrame)
    {
        case GLOBAL:
            strcpy(framePrint, "GF");
            break;
        case LOCAL:
            strcpy(framePrint, "LF");
            break;
        case TEMPORARY:
            strcpy(framePrint, "TF");    
            break;
    }
}

void GEN_WRITE_VAR_LITERAL(Token *token, int deep){
    GEN_SET_FRAME_TYPE();
    switch (token->type)////if is a digit
	{
		case TOKEN_TYPE_LITERAL_INT:
			printf("int@%s", token->data);
			break;

		case TOKEN_TYPE_LITERAL_FLOAT:
			printf("float@%a", atof(token->data));
			break;

		case TOKEN_TYPE_LITERAL_STRING:
			printf("string@%s", token->data);
			break;

        case TOKEN_TYPE_IDENTIFIER:
            printf("%s@%s$%d", framePrint, token->data, findVariableWithType(token, deep, SymTable->var)->deep);
            break;
        
        case TOKEN_TYPE_UNDERSCORE:
            printf("GF@_");
	}
}

// void GEN_RETVAL_CREATER(outputParams outPut){
//     int retval_number = 0;
//     while(outPut != NULL){
//         printf("DEFVAR LF@retval%d\n", retval_number);
//         retval_number++;
//         outPut = outPut->next;
//     }
// }

// void GEN_RETVAL_RETURN(outputParams outPut){
//     int retval_number = -1;
//     while(outPut != NULL){
//         retval_number++;
//         outPut = outPut->next;
//     }
//     while(retval_number >= 0){
//         printf("POPS LF@retval%d\n", retval_number);
//         retval_number--;
//     }
// }


void GEN_CREATE_LEFT_SIDE(int deep){
    while(varStack != NULL){
        if(varStack->token_stack->type == TOKEN_TYPE_UNDERSCORE)
            changeErrorCode(3);
        else {
            printf("DEFVAR ");
            GEN_WRITE_VAR_LITERAL(varStack->token_stack, deep);
            printf("\n");
            printf("POPS ");
            GEN_WRITE_VAR_LITERAL(varStack->token_stack, deep);
            printf("\n");
        }
        GEN_DELETE_VAR_FROM_ASSEMBLY_STACK();
    }
}

void GEN_EQ_LEFT_SIDE(int deep){
    while(varStack != NULL){
        printf("POPS ");
        GEN_WRITE_VAR_LITERAL(varStack->token_stack, deep);
        printf("\n"); 
        GEN_DELETE_VAR_FROM_ASSEMBLY_STACK();
    }
}





void GEN_ADD_VAR_TO_ASSEMBLY_STACK(Token *stack){
    if(varStack == NULL){
        varStack = malloc(sizeof(var_assembly_stack));
        varStack->token_stack = stack;
        varStack->next = NULL;
    } else {
        var_assembly_stack *tmpStack = malloc(sizeof(var_assembly_stack));
        tmpStack->token_stack = stack;
        tmpStack->next = varStack;
        varStack = tmpStack;
    }
}


void GEN_DELETE_VAR_FROM_ASSEMBLY_STACK(){
    if(varStack != NULL){
        var_assembly_stack *tmpStack = varStack->next;
        free(varStack);
        varStack = tmpStack;
    }
}

void GEN_DELETE_FULL_VAR_ASSEMBLY_STACK(){
    while(varStack != NULL)
        GEN_DELETE_VAR_FROM_ASSEMBLY_STACK();
}

















void GEN_LEN(){
   printf("LABEL $length\n");
   printf("PUSHFRAME\n");
   printf("DEFVAR LF@%%retval\n");

   printf("DEFVAR LF@%%param1\n");
   printf("MOVE LF@param1 LF@%%1\n");
   printf("DEFVAR LF@param1_type\n");
   printf("STRLEN LF@%%retval LF@%%param1\n");
   printf("POPFRAME\n");
   printf("RETURN\n");
}	

void generation_substr(){
    printf("LABEL $substr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%retval\n");
   //printf("MOVE LF@$retval string@\n");
    printf("DEFVAR LF@length_of_str\n");
    // printf("CREATEFRAME\n");
    // printf("DEFVAR TF@%%1\n");
    // printf("MOVE TF@%%1 LF@%%1\n");
    // printf("CALL $length\n");	
    // printf("MOVE LF@length_of_str TF@%%retval\n");
    printf("DEFVAR LF@%%param1\n");
    printf("MOVE LF@param1 LF@%%1\n");
    printf("STRLEN LF@%%length_of_str LF@%%param1\n");

    printf("DEFVAR LF@result\n");			

    printf(" EQ LF@result LF@length_of_str int@0\n");
	printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");
	printf(" LT LF@result LF@length_of_str int@0\n");
	printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");

	printf(" LT LF@result LF@%%2 int@0\n");
	printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");
	printf(" EQ LF@result LF@%%2 int@0\n");////////////////&??????
	printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");

	printf(" GT LF@result LF@%%2 LF@length_of_str\n");
    printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");

	printf(" EQ LF@result LF@%%3 int@0\n");
	printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");
    printf(" LT LF@result LF@%%3 int@0\n");
	printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");

	printf(" GT LF@result LF@%%3 LF@length_of_str\n");
    printf(" JUMPIFEQ $pop_ret LF@result bool@true\n");

	printf(" DEFVAR LF@new_length\n");
	printf(" MOVE LF@new_length LF@length_of_str\n");
	printf(" SUB LF@new_length LF@new_length LF@%%2\n");
	printf(" ADD LF@new_length LF@new_length int@1\n");

	// printf(" DEFVAR LF@edit_n_cond\n");
	// printf(" GT LF@edit_n_cond LF@%2 LF@new_length\n");
	// printf(" JUMPIFEQ $substr$edit_n LF@edit_n_cond bool@true\n");

	// printf(" JUMP $substr$process\n");
	// printf(" LABEL $substr$edit_n\n");
	// printf(" MOVE LF@%2 LF@new_length\n");
	// printf(" LABEL $substr$process\n");
	// printf(" DEFVAR LF@index\n");
	// printf(" MOVE LF@index LF@%%2\n");
	// printf(" SUB LF@index LF@index int@1\n");
	// printf(" DEFVAR LF@char\n");
	// printf(" DEFVAR LF@process_loop_cond\n");
	// printf(" LABEL $substr$process_loop\n");
	// printf(" GETCHAR LF@char LF@%0 LF@index\n");
	// printf(" CONCAT LF@%retval LF@%retval LF@char\n");
	// printf(" ADD LF@index LF@index int@1\n");
	// printf(" SUB LF@%2 LF@%2 int@1\n");
	// printf(" GT LF@process_loop_cond LF@%2 int@0\n");
	// printf(" JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true\n");

	printf(" LABEL $pop_ret\n");
	printf(" POPFRAME\n");
	printf(" RETURN\n");
	

}
void print(){
    printf("WRITE GF@");
}

// void float_2_int(){
//    printf("LABEL $float_2_int\n");
//    printf("PUSHFRAME\n");
//    printf("DEFVAR LF@%retval\n");
//    printf("FLOAT2INT LF@%retval LF@%1\n");
//    printf("POPFRAME\n");
//    printf("RETURN\n");
// }
// void int_2_float(){
//    printf("LABEL $float_2_int\n");
//    printf("PUSHFRAME\n");
//    printf("DEFVAR LF@%retval\n");
//    printf("FLOAT2INT LF@%retval LF@%1\n");
//    printf("POPFRAME\n");
//    printf("RETURN\n");
// }

// void int2float{}
// 	if (l_type == TYPE_INT && ret_type == TYPE_DOUBLE)
// 	{
// 		ADD_INST("FLOAT2R2EINT TF@%retval TF@%retval");
// 	}
// 	else if (l_type == TYPE_DOUBLE && ret_type == TYPE_INT)
// 	{
// 		ADD_INST("INT2FLOAT TF@%retval TF@%retval");
// 	}				
// void var_declare(char *var_id){
// 	ptintf("DEFVAR LF@");
//     printf("%s\n",var_id);
// }f
void generation_print(){
        switch(token->type)
        {
            case 11:
                printf( "WRITE int@%s\n",token->data);
                break;
            case 12:
                printf("WRITE float@%s\n",token->data);
                break;
            case 13:
                printf( "WRITE string@%s\n",token->data);
                break;
            default:
                printf( "WRITE  LF@%s\n",token->data);
                break;
        }
}
