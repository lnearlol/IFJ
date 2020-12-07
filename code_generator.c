/**
 * @file code_generator.c
 * 
 * @brief Code generator implementation
 * 
 * IFJ Projekt 2020, Tým 55
 * 
 * @author <xstepa64> Stepaniuk Roman, Bc.
 * @author <xpastu04> Pastushenko Vladislav
 * @author <xbahda01> Bahdanovich Viktoryia 
 * @author <xtomas34> Tomason Viktoryia 
 */
#include "parser.h"

// char framePrint[3];
/**
 * Function insert header for ifjcode20 and creates a global variable for working with "main"
 */
 void GEN_START_MAIN(){
	printf(".IFJcode20\n");
    printf("DEFVAR GF@_\n");

    // Global variables for concatenation processing
    printf("DEFVAR GF@str1\n");
    printf("DEFVAR GF@str2\n");
    printf("DEFVAR GF@strRes\n");

    // If divide by zero
    printf("DEFVAR GF@tmpDividingByZero\n");
    printf("JUMP enderror9\n");
    printf("LABEL error9\n");
    printf("EXIT int@9\n");
    printf("LABEL enderror9\n");


	printf("JUMP $$main\n");

 }
/**
 * Function generates function with the creation of a local frame and creates temporary frame for working with "main".
 * @param token Pointer to the token that indicates the function name
 */
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
/**
 * Function generates pop instruction with return and jump to the end of function for "main".
 * @param token Pointer to the token that indicates the function name
 */
  void GEN_END_OF_FUNCTION(Token *token){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    if(!strcmp(token->data, "main")){
	    printf("POPFRAME\n\n");
        printf("JUMP $END_OF_PROGRAM\n");
    } else {
        printf("POPFRAME \n");
        printf("RETURN \n\n");
    }
 }

/**
 * Function creates temporary frame and initialize temporary variables
 * @param InParams Signifies input parameters of function 
 */
void GEN_CREATE_FRAME_AND_SET_PARAMS(inputParams InParams){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    printf("CREATEFRAME\n");
    while(InParams != NULL){
        printf("DEFVAR TF@%s$0\n", InParams->name);
        InParams = InParams->next;
    }
}
/**
 * Function moves parameters value to the temporary 
 * @param InParam Signifies input parameters of function 
 * @param value Pointer to the token that indicates the name of variable
 * @param deep Signifies the level of the variable
 */
void MOVE_INTO_INPUT_PARAMETER(inputParams InParam, Token *value, int deep){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    printf("MOVE TF@%s$0 ", InParam->name);
    GEN_WRITE_VAR_LITERAL(value, deep);
    printf("\n");
}

/**
 * Function calls needed function
 * @param token Pointer to the token that indicates the function name
 */
void GEN_CALL(Token *token){
    if(!GET_REPEAT_FUNC_RUN())
        return;
   printf("CALL $%s\n", token->data);
}

// //Function defines the location frame
// void GEN_SET_FRAME_TYPE(){
//     genFrameType currentFrame = GET_GEN_FRAME();
    
//     switch (currentFrame)
//     {
//         case GLOBAL:
//             strcpy(framePrint, "GF");
//             break;
//         case LOCAL:
//             strcpy(framePrint, "LF");
//             break;
//         case TEMPORARY:
//             strcpy(framePrint, "TF");    
//             break;
//     }
// }
/**
 * Function writes a variable according to its type. If it's identifier find the deepest one by its name
 * @param token Pointer to the token that indicates the name of variable
 * @param deep Signifies the level of the variable
 */
void GEN_WRITE_VAR_LITERAL(Token *token, int deep){
    // GEN_SET_FRAME_TYPE();
    char *ASM_string = NULL;
    switch (token->type)
	{
		case TOKEN_TYPE_LITERAL_INT:
			printf("int@%s", token->data);
			break;

		case TOKEN_TYPE_LITERAL_FLOAT:
			printf("float@%a", atof(token->data));
			break;

		case TOKEN_TYPE_LITERAL_STRING:
			ASM_string = GEN_ASM_STRING(token, ASM_string);
			printf("string@%s", ASM_string);
			break;

        case TOKEN_TYPE_IDENTIFIER:
            printf("LF@%s$%d", token->data, findVariableWithType(token, deep, SymTable->var)->deep);
            break;
        
        case TOKEN_TYPE_UNDERSCORE:
            printf("GF@_");
            break;
	}
    free(ASM_string);
}


char* ASM_DATA_APPEND(char* ASM_string, char state, int length){
    ASM_string = realloc(ASM_string, length);
    strncat(ASM_string, &state, 1);
    return ASM_string;
}

char *GEN_ASM_STRING(Token *token, char* ASM_string){
    char state = token->data[0];
    int length = 1;
    ASM_string = malloc(length);
    strcpy(ASM_string, "");

    for(int i = 1; state != '\0'; i++){
        if((state >= 0 && state <= 32) || state == 35 || state == 92){
            char *ASCII_code=(char*)malloc(ASCII_CODE_ARRAY_SIZE);
            sprintf(ASCII_code, "%03d", state);
            state = Backslash;
            ASM_string = ASM_DATA_APPEND(ASM_string, state, ++length);
            state = ASCII_code[0];
            ASM_string = ASM_DATA_APPEND(ASM_string, state, ++length);
            state = ASCII_code[1];
            ASM_string = ASM_DATA_APPEND(ASM_string, state, ++length);
            state = ASCII_code[2];
            free(ASCII_code);
        }
        ASM_string = ASM_DATA_APPEND(ASM_string, state, ++length);
        state = token->data[i];
    }
    return ASM_string;
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

/**
 * Functions doing something
 * @param deep Signifies the level of the variable
 */
void GEN_CREATE_LEFT_SIDE(int deep){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    while(varStack != NULL){
        if(varStack->token_stack->type == TOKEN_TYPE_UNDERSCORE)
            changeErrorCode(3);
        else {
            // printf("DEFVAR ");
            // GEN_WRITE_VAR_LITERAL(varStack->token_stack, deep);
            // printf("\n");
            printf("POPS ");
            GEN_WRITE_VAR_LITERAL(varStack->token_stack, deep);
            printf("\n");
        }
        GEN_DELETE_VAR_FROM_ASSEMBLY_STACK();
    }
}

void GEN_EQ_LEFT_SIDE(int deep){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    while(varStack != NULL){
        printf("POPS ");
        GEN_WRITE_VAR_LITERAL(varStack->token_stack, deep);
        printf("\n"); 
        GEN_DELETE_VAR_FROM_ASSEMBLY_STACK();
    }
}

/**
 * Functions doing something
 * @param stack Pointer signifies 
 */
void GEN_ADD_VAR_TO_ASSEMBLY_STACK(Token *stack){
    if(!GET_REPEAT_FUNC_RUN()){
                // printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ended\n");

        return;
    }
    // printf("--------------------------------------------------------------------\n");
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
    if(!GET_REPEAT_FUNC_RUN())
        return;
    while(varStack != NULL)
        GEN_DELETE_VAR_FROM_ASSEMBLY_STACK();
}
/**
* Function print 
* @param token Pointer to the token that indicates the name of variable
* @param deep Signifies the level of the variable
*/
void GEN_PRINT_WRITE(Token *token, int deep){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    printf("WRITE ");
    GEN_WRITE_VAR_LITERAL(token, deep);
    printf("\n");
}

// ---------------------------------   I F   C O N S T R U C T I O N   ---------------------------------

/**
 * Functions generates if operation in code
 * @param token Pointer to the token that indicates the name of functions
 * @param count Number of condition
 * @param condition Signifies the current condition
 */
void GEN_JUMP(Token *token, int count, bool if_condition, int for_condition){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    
    if(for_condition == FOR_JUMP_EQ){
        printf("PUSHS bool@true\n");
        printf("JUMPIFEQS $%s$for$%d\n", token->data, count);
    } else if (for_condition == FOR_JUMP){
        printf("JUMP $%s$for$%d\n", token->data, count);
    } else if (if_condition){
        printf("PUSHS bool@true\n");
        printf("JUMPIFNEQS $%s$if$%d$else\n", token->data, count);
    } else 
        printf("JUMP $%s$if$%d$end\n", token->data, count);
}

void GEN_SCOPE(Token *token, int count, bool if_condition, bool for_condition){
    if(!GET_REPEAT_FUNC_RUN())
        return;
    if(for_condition){
        printf("LABEL $%s$for$%d\n", token->data, count);
    } else if (if_condition){
        printf("LABEL $%s$if$%d$else\n", token->data, count);
    } else 
        printf("LABEL $%s$if$%d$end\n", token->data, count);
}


// ---------------------------------   I N B U I L D   F U N C T I O N S   ---------------------------------

//Defines inbuilt functions
void GEN_CALL_INBUILDS(){
    GENERATION_INPUTS();
    GENERATION_INPUTI();
    GENERATION_INPUTF();
    GENERATION_LEN();
    GENERATION_FLOAT2INT();
    GENERATION_INT2FLOAT();
    GENERATION_SUBSTR();
    GENERATION_ORD();
    GENERATION_CHR();
}

/**
 * Function generates inputi instruction in code.
 * inputi() (int,int)
 */
void GENERATION_INPUTI(){
    printf("LABEL $inputi\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("DEFVAR LF@retval$1\n");
    printf("READ LF@retval int\n");
    printf("DEFVAR LF@retval_check\n");

    printf("TYPE LF@retval_check LF@retval\n");
    printf("JUMPIFNEQ $INPUTI_END LF@retval_check string@int\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END_INPUTI\n");

	printf("LABEL $INPUTI_END\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $END_INPUTI\n");
	printf("POPFRAME\n");
	printf("RETURN\n\n");
}
/**
 * Function generates inputs instruction in code.
 * inputs() (string,int)
 */
void GENERATION_INPUTS(){
    printf("LABEL $inputs\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("DEFVAR LF@retval$1\n");
    printf("READ LF@retval string\n");
    printf("DEFVAR LF@retval_check\n");

    printf("TYPE LF@retval_check LF@retval\n");
    printf("JUMPIFNEQ $INPUTS_END LF@retval_check string@string\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END_INPUTS\n");

	printf("LABEL $INPUTS_END\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $END_INPUTS\n");
	printf("POPFRAME\n");
	printf("RETURN\n\n");
}
/**
 * Function generates inputf instruction in code.
 * inputf() (float,int)
 */
void GENERATION_INPUTF(){
    printf("LABEL $inputf\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("DEFVAR LF@retval$1\n");
    printf("DEFVAR LF@retval_check\n");
    printf("READ LF@retval float\n");

    printf("TYPE LF@retval_check LF@retval\n");
    printf("JUMPIFNEQ $INPUTF_END LF@retval_check string@float\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END_INPUTF\n");

	printf("LABEL $INPUTF_END\n");
    printf("MOVE LF@retval nil@nil\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $END_INPUTF\n");
	printf("POPFRAME\n");
	printf("RETURN\n\n");
}
/**
 * Function generates len instruction in code.
 * len (s string) (int)
 */
void GENERATION_LEN(){
   printf("LABEL $len\n");
   printf("PUSHFRAME\n");
   printf("DEFVAR LF@retval\n");
   printf("DEFVAR LF@param1\n");
   printf("MOVE LF@param1 LF@s$0\n");
   printf("STRLEN LF@retval LF@param1\n");
   printf("PUSHS LF@retval\n");

   printf("POPFRAME\n");
   printf("RETURN\n\n");
}	
/**
 * Function generates float2int instruction in code.
 * float2int (f float64) (int)
 */
void GENERATION_FLOAT2INT(){
   printf("LABEL $float2int\n");
   printf("PUSHFRAME\n");
   printf("DEFVAR LF@retval\n");
   printf("DEFVAR LF@param$1\n");
   printf("MOVE LF@param$1 LF@f$0\n");//f
   printf("FLOAT2INT LF@retval LF@param$1\n");
   printf("PUSHS LF@retval\n");
   printf("POPFRAME\n");
   printf("RETURN\n\n");
}	
/**
 * Function generates int2float instruction in code.
 * int2float (i int)(float64)
 */
void GENERATION_INT2FLOAT(){
   printf("LABEL $int2float\n");
   printf("PUSHFRAME\n");
   printf("DEFVAR LF@retval\n");
   printf("DEFVAR LF@param$1\n");
   printf("MOVE LF@param$1 LF@i$0\n");

   printf("INT2FLOAT LF@retval LF@param$1\n");
   printf("PUSHS LF@retval\n");
   printf("POPFRAME\n");
   printf("RETURN\n\n");
}	
/**
 * Function generates substr instruction in code.
 * substr(s string,i int,n int) (string, int)
 */
void GENERATION_SUBSTR(){
    printf("LABEL $substr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");//string
    printf("DEFVAR LF@retval$1\n");//int

    printf("DEFVAR LF@string\n");
    printf("DEFVAR LF@from\n");
    printf("DEFVAR LF@length_of_str\n");
    printf("DEFVAR LF@length\n");

    printf("DEFVAR LF@length_helper\n");
    printf("DEFVAR LF@char\n");
    printf("DEFVAR LF@new_strlen\n");

    printf("MOVE LF@retval string@\n");
    printf("MOVE LF@string LF@s$0\n");
    printf("MOVE LF@from LF@i$0\n");
    printf("MOVE LF@length_of_str LF@n$0\n");

    printf("MOVE LF@length_helper int@0\n");

    printf("STRLEN LF@length LF@string\n");//length = STRLEN(STRING)
    printf("SUB LF@new_strlen LF@length int@1\n");//new_strlen = length - 1

    printf("DEFVAR LF@result\n");			
    printf("LT LF@result LF@length_of_str int@0\n"); //n < O
	printf("JUMPIFEQ $SUBSTR_END LF@result bool@true\n");

    printf("EQ LF@result LF@length_of_str int@0\n"); //n == O
	printf("JUMPIFEQ $SUBSTR_EMPTY LF@result bool@true\n");

    printf("LT LF@result LF@from int@0\n"); //i < O
	printf("JUMPIFEQ $SUBSTR_END LF@result bool@true\n");

    printf("GT LF@result LF@from LF@new_strlen\n");//i >= length - 1
    printf("JUMPIFEQ $SUBSTR_END LF@result bool@true\n");

    printf("ADD LF@length_helper LF@length_helper LF@from\n");
    printf("ADD LF@length_helper LF@length_helper LF@length_of_str\n");//i + n

    printf("GT LF@result LF@length_helper LF@length\n");//i + n > n ? n : i+n
	printf("JUMPIFEQ $SUBSTR_LEN LF@result bool@true\n");
    printf("JUMP $for_loop\n");
	printf("LABEL $SUBSTR_LEN\n");
	printf("MOVE LF@length_helper LF@length\n");

	printf("LABEL $for_loop\n");
    printf("JUMPIFEQ $SUBSTR_RET_0 LF@length_helper LF@from\n");
    printf("GETCHAR LF@char LF@string LF@from\n");
    printf("CONCAT LF@retval LF@retval LF@char\n");
    printf("ADD LF@from LF@from int@1\n");
    printf("JUMP $for_loop\n");

    printf("LABEL $SUBSTR_RET_0\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END\n");

	printf("LABEL $SUBSTR_END\n");
    printf("MOVE LF@retval nil@nil\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $END\n");
	printf("POPFRAME\n");
	printf("RETURN\n\n");

    printf("LABEL $SUBSTR_EMPTY\n");
    printf("MOVE LF@retval string@\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END\n\n");
}
/**
 * Function generates ord instruction in code.
 * ord(s string,i int) (int, int)
 */
void GENERATION_ORD(){
    printf("LABEL $ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@string\n");
    printf("DEFVAR LF@int\n");
    printf("DEFVAR LF@length\n");
    printf("DEFVAR LF@right_int\n");
    printf("DEFVAR LF@retval\n");//string
    printf("DEFVAR LF@retval$1\n");//int

    printf("MOVE LF@string LF@s$0\n");
    printf("MOVE LF@int LF@i$0\n\n");

    printf("STRLEN LF@length LF@string\n");//5
    printf("SUB LF@length LF@length int@1\n");//4

    printf("GT LF@right_int LF@int LF@length\n");
    printf("JUMPIFEQ $ORD_END LF@right_int bool@true\n");//i > len(n)-1

    printf("LT LF@right_int LF@int int@0\n");
    printf("JUMPIFEQ $ORD_END LF@right_int bool@true\n");//i < 0

    printf("STRI2INT LF@retval LF@string LF@int\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $ORD_RET\n");

    printf("LABEL $ORD_END\n");
    printf("MOVE LF@retval nil@nil\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $ORD_RET\n");
	printf("POPFRAME\n");
	printf("RETURN\n\n");
}
/**
 * Function generates chr instruction in code.
 * chr(i int) (string, int)
 */
void GENERATION_CHR(){
    printf("LABEL $chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@int\n");
    printf("DEFVAR LF@right_int\n");
    printf("DEFVAR LF@retval\n");//string
    printf("DEFVAR LF@retval$1\n");//int

    printf("MOVE LF@int LF@i$0\n");

    printf("LT LF@right_int LF@int int@0\n");
    printf("JUMPIFEQ $CHR_END LF@right_int bool@true\n");//i < 0

    printf("GT LF@right_int LF@int int@255\n");
    printf("JUMPIFEQ $CHR_END LF@right_int bool@true\n");//i > len(n)-1

    printf("INT2CHAR LF@retval LF@int\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $CHR_RET\n");

    printf("LABEL $CHR_END\n");

    printf("MOVE LF@retval nil@nil\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $CHR_RET\n");
	printf("POPFRAME\n");
	printf("RETURN\n\n");
}