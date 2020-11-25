#include "parser.h"



 void START_OF_FUNCTION(Token *token){
    if(!strcmp(token->data, "main")){
        printf("LABEL $");
        printf("%s\n", token->data);
        printf("PUSHFRAME \n");
    } else {
        printf("LABEL $main\n");
	    printf("CREATEFRAME\n");
    }

 }

  void END_OF_FUNCTION(Token *token){
    printf("POPFRAME \n");
    printf("RETURN \n");
    printf("LABEL $endof");
    printf("%s\n", token->data);
 }

 void START_MAIN(){
	printf(".IFJcode20\n");
	printf("JUMP $main\n");

 }

  void END_OF_MAIN(){
	printf("POPFRAME\n");
    printf("LABEL $endofmain\n");
 }

 void CALL(Token *token){
   printf("CALL $");
   printf("%s\n", token->data);
 }

void GENERATION_LEN(){
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
