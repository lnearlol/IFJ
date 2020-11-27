//#include "scanner.h"
#include "generation.h"
int set_next_token(){
    token->next = malloc (sizeof(Token));
    token = token->next;
    token->next = NULL;
    if(get_token(token))
        return 1;
    return 0;
}
void START_OF_FUNCTION(char *id_of_function){
    printf("LABEL $$");
    printf("%s\n",id_of_function);
    printf("PUSHFRAME \n");
    printf("DEFVAR LF@retval\n");
}

void END_OF_FUNCTION(){
    printf("POPFRAME \n");
    printf("RETURN \n");
}

void START_MAIN(){
	printf(".IFJcode20\n");
	printf("JUMP $main\n");
	printf("LABEL $main\n");
	printf("CREATEFRAME\n");
}

void END_OF_MAIN(){
    printf("POPFRAME\n");
    printf("LABEL $endofmain\n");
}

void CALL(char *id_of_function){
   printf("CALL $");
   printf("%s\n",id_of_function);
 }

void GENERATION_INPUTI(){
    printf("LABEL $INPUTI\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("READ LF@$retval int\n");
    printf("DEFVAR LF@retval_check\n");

    printf("TYPE LF@$retval_check LF@retval\n");
    printf("JUMPIFEQ $INPUTI_END LF@$retval_check nil@nil\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END\n");

    printf("LABEL $INPUTI_END\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $END\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}
void GENERATION_INPUTS(){
    printf("LABEL $INPUTS\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("READ LF@$retval string\n");
    printf("DEFVAR LF@retval_check\n");

    printf("TYPE LF@$retval_check LF@retval\n");
    printf("JUMPIFEQ $INPUTS_END LF@$retval_check  nil@nil\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END\n");

    printf("LABEL $INPUTS_END\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $END\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}
void GENERATION_INPUTF(){
    printf("LABEL $INPUTF\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("DEFVAR LF@retval_check\n");
    printf("READ LF@$retval float\n");

    printf("TYPE LF@$retval_check LF@retval\n");
    printf("JUMPIFEQ $INPUTF_END LF@$retval_check nil@nil\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END\n");

    printf("LABEL $INPUTF_END\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $END\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void GENERATION_LEN(){
   printf("LABEL $LENGTH\n");
   printf("PUSHFRAME\n");
   printf("DEFVAR LF@retval\n");
   printf("DEFVAR LF@param1\n");
   printf("MOVE LF@param1 LF@s$0\n");
   printf("STRLEN LF@retval LF@param1\n");
   printf("POPFRAME\n");
   printf("RETURN\n");
}	

void GENERATION_FLOAT2INT(){
   printf("LABEL $FLOAT2INT\n");
   printf("PUSHFRAME\n");
   printf("DEFVAR LF@retval\n");
   printf("DEFVAR LF@param$1\n");
   printf("MOVE LF@param$1 LF@f$0\n");//f
   printf("FLOAT2INT LF@retval LF@param$1\n");
   printf("PUSHS LF@retval\n");
   printf("POPFRAME\n");
   printf("RETURN\n");
}	

void GENERATION_INT2FLOAT(){
   printf("LABEL $INT2FLOAT\n");
   printf("PUSHFRAME\n");
   printf("DEFVAR LF@retval\n");
   printf("DEFVAR LF@param$1\n");
   printf("MOVE LF@param$1 LF@i$0\n");
   printf("INT2FLOAT LF@retval LF@param$1\n");
   printf("PUSHS LF@retval\n");
   printf("POPFRAME\n");
   printf("RETURN\n");
}	

void GENERATION_SUBSTR(){
    printf("LABEL $SUBSTR\n");
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

    printf("MOVE LF@string LF@s$0\n");
    printf("MOVE LF@start LF@i$0\n");
    printf("MOVE LF@length_of_str LF@n$0\n");

    printf("MOVE LF@lenght_helper int@0\n");

    printf("STRLEN LF@length LF@strihg\n");//length = STRLEN(STRING)
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

    printf("ADD LF@length_helper LF@from\n");
    printf("ADD LF@length_helper LF@length_of_str\n");//i + n

    printf("GT LF@result LF@length_helper LF@length\n");//i + n > n ? n : i+n
    printf("JUMPIFEQ $SUBSTR_LEN LF@result bool@true\n");
    printf("JUMP $for_loop\n");
    printf("LABEL $SUBSTR_LEN\n");
    printf("MOV LF@length_helper LF@length\n");

    printf("LABEL $for_loop\n");
    printf("JUMPIFEQ $SUBSTR_RET_0 LF@length_helper LF@from\n");
    printf("GETCHAR LF@char LF@string LF@from\n");
    printf("CONCAT LF@$retval LF@$retval LF@char\n");
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
    printf("RETURN\n");

    printf("LABEL $SUBSTR_EMPTY\n");
    printf("MOVE LF@retval string@\000\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $END\n");
}

void GENERATION_ORD(){
    printf("LABEL $ORD\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@string\n");
    printf("DEFVAR LF@int\n");
    printf("DEFVAR LF@length\n");
    printf("DEFVAR LF@right_int\n");
    printf("DEFVAR LF@retval\n");//string
    printf("DEFVAR LF@retval$1\n");//int

    printf("MOVE LF@string LF@s$0\n");
    printf("MOVE LF@int LF@i$0\n\n");

    printf("STRLEN LF@length LF@string\n");
    printf("SUB LF@length LF@length int@1\n");

    printf("GT LF@right_int LF@int LF@length\n");
    printf("JUMPIFEQ $ORD_END LF@right_int bool@true\n");//i > len(n)-1

    printf("LT LF@right_int LF@int int@0\n");
    printf("JUMPIFEQ $ORD_END LF@right_int bool@true\n");//i < 0

    printf("STRI2INT LF@$retval LF@string LF@int\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $ORD_RET\n");

    printf("LABEL $ORD_END\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $ORD_RET\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}
void GENERATION_CHR(){
    printf("LABEL $CHR\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@int\n");
    printf("DEFVAR LF@right_int\n");
    printf("DEFVAR LF@retval\n");//string
    printf("DEFVAR LF@retval$1\n");//int

    printf("MOVE LF@int LF@i$0\n\n");

    printf("LT LF@right_int LF@int int@0\n");
    printf("JUMPIFEQ $CHR_END LF@right_int bool@true\n");//i < 0

    printf("GT LF@right_int LF@int int@255\n");
    printf("JUMPIFEQ $CHR_END LF@right_int bool@true\n");//i > len(n)-1

    printf("INT2CHAR LF@$retval LF@int\n");

    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@0\n");
    printf("PUSHS LF@retval$1\n");
    printf("JUMP $CHR_RET\n");

    printf("LABEL $CHR_END\n");
    printf("PUSHS LF@retval\n");
    printf("MOVE LF@retval$1 int@1\n");
    printf("PUSHS LF@retval$1\n");

    printf("LABEL $CHR_RET\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void GENERATION_PRINT(){
    printf("LABEL $PRINT\n");
    printf("PUSHFRAME\n");
    printf("WRITE LF@ ");///??????????
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void GENERATION_PRINT(Token *token){
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

int main(){
    token = malloc(sizeof(Token));
    token->next = NULL;
    Token *first = token;

    if(get_token(token)){
        fprintf(stderr, "\n     ERROR!!\n\n");
    }


    for(int i = 0; ; i++) {
        printf("#%d %-10s       <-%d\n", i, token->data, token->type);
        if(token->type == TOKEN_TYPE_EOFILE)
            break;
        if(!strcmp(token->data, "inputs"))
            GENERATION_INPUTS();
        if(set_next_token()){
            fprintf(stderr, "\n     ERROR!!\n\n");
        }
    }

    dtor(first);
    //fclose(program _code);
    return 0;
}
