#include "parser.h"
int deep = -1;
int number_of_operands = 0;
bool function_body();
void allocating_error(){
     printf ("*ERROR* Memory was not alocated.\n");
    errflg = true;                      /* globální proměnná -- příznak chyby */
}

void get_and_set_token(){
    token->next = malloc (sizeof(Token));
    token = token->next;
    token->next = NULL;
    get_token(token);
}
//  ------------------------------------ P R O G R A M    S T A R T ------------------------------------
// package main \n func() EOF
bool program_start(){
    bool program_start = false;
    if(token->type == TOKEN_TYPE_PACKAGE){
        printf("PROGRAM_START FLAG 1: TOKEN->DATA: %s\n", token->data);
        get_and_set_token();
        if(token->type == TOKEN_TYPE_IDENTIFIER && !strcmp(token->data, "main")){
            printf("PROGRAM_START FLAG 2: TOKEN->DATA: %s\n", token->data);
            get_and_set_token();
            if(token->type == TOKEN_TYPE_EOL){
                printf("PROGRAM_START FLAG 3: TOKEN->DATA: %s\n", token->data);
                get_and_set_token();
                while(token->type != TOKEN_TYPE_EOFILE){
                    printf("PROGRAM_START FLAG 4: TOKEN->DATA: %s\n", token->data);
                    program_start = function();
                    if(program_start == false){
                        printf("ERROR IN FUNCTION!\n");
                        return program_start;
                    }
                    get_and_set_token();  // when we return this function as valid we should get next token to compare with EOF
                }
            }
        }
    }
    if(program_start == false)
        printf("ERROR IN FIRST LINE OR THERE ARE NO (VALID) FUNCTIONS IN YOUR PROGRAM!\n");
    return program_start;
}

//  ------------------------------------ F U N C T I O N ------------------------------------

bool function(){
    bool func = false;
    if(token->type == TOKEN_TYPE_FUNC){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_IDENTIFIER){
            get_and_set_token();
            if(token->type == TOKEN_TYPE_LEFT_BRACKET){
                get_and_set_token();
                func = input_parameters();
                if(func){
                    get_and_set_token();
                    func = output_parameters();
                    if(func){
                        if(token->type == TOKEN_TYPE_START_BLOCK){
                            get_and_set_token();
                            if(token->type == TOKEN_TYPE_EOL){
                                get_and_set_token();
                                deep++;    // when first { is opened in function
                                func = function_body();
                                if(func && token->type == TOKEN_TYPE_END_BLOCK){
                                    get_and_set_token();
                                    if(token->type == TOKEN_TYPE_EOL)
                                    func = true;
                                }      
                            }
                        }
                    }
                }
            }
        }
    }
    printf("HI FROM FUNC %s\n", token->data);
    return func;
}

//  ------------------------------------ I N P U T    P A R A M E T E R S ------------------------------------

bool input_parameters(){
    bool input_parameters = false;
    
    if(token->type == TOKEN_TYPE_RIGHT_BRACKET)
        input_parameters = true;
    else 
        input_parameters = input_single_parameters();
    
    printf("HI FROM INPUT_PARAMS %s\n", token->data);
    return input_parameters;
}

bool input_single_parameters(){
    bool input_single_parameter = false;

                printf("\nTOKEN: %s %d\n\n", token->data, token->type);
    if(token->type == TOKEN_TYPE_IDENTIFIER){
        printf("INPUT_SINGLE_PARAMETER 1 %s\n", token->data);
        get_and_set_token();
                    printf("\nTOKEN: %s %d\n\n", token->data, token->type);
        if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
            printf("INPUT_SINGLE_PARAMETER 2 %s\n", token->data);
            
            // input_single_parameter = true;
            get_and_set_token();
            printf("\nTOKEN: %s %d\n\n", token->data, token->type);
            if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
                printf("INPUT_SINGLE_PARAM RETURNS TRUE\n");
                return true;
            }
            else if (token->type == TOKEN_TYPE_COMMA){
                get_and_set_token();
                input_single_parameter = input_single_parameters();
            } else {
                printf("INPUT_SINGLE_PARAM RETURNS FALSE\n");
                input_single_parameter = false;
            }
        }
    } 

    printf("HI FROM INPUT_SINGLE_PARAMETER %s\n", token->data);
    return input_single_parameter;
}

//  ------------------------------------ O U T P U T    P A R A M E T E R S ------------------------------------

bool output_parameters(){
    bool output_parameters = false;
    if(token->type == TOKEN_TYPE_START_BLOCK)
        output_parameters = true;
    else if (token->type == TOKEN_TYPE_LEFT_BRACKET){
        printf("              HI FROM OUTPUT_PARAMETERS %s\n", token->data);
        get_and_set_token();
        output_parameters = output_single_parameters();
    }
    
    printf("HI FROM OUTPUT_PARAMS %s\n", token->data);
    return output_parameters;
}

bool output_single_parameters(){
    bool output_single_parameter = false;
    printf("              HI FROM OUTPUT_SINGLE_PARAMETER %s\n", token->data);
    if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_COMMA){
            get_and_set_token();
            output_single_parameter = output_single_parameters();
        } else if (token->type == TOKEN_TYPE_RIGHT_BRACKET){
            get_and_set_token();
            if(token->type == TOKEN_TYPE_START_BLOCK){
                output_single_parameter = true;
            }
        }
    } 
    return output_single_parameter;
}

//  ------------------------------------ F U N C T I O N    B O D Y ------------------------------------

bool function_body(){
    static bool empty_block = false;
    bool function_accept = false;
    if(token->type == TOKEN_TYPE_END_BLOCK){
        deep--;
                   printf("                                 ITS EMPTY BLOCK  %d\n", empty_block);
        if(empty_block){
            printf("                                 ITS EMPTY BLOCK\n");
            return false;
        }
    } 

    // I G N O R I N G    E M P T Y    B L O C K S   -> { /n  /n }, but not if{ /n }
    if(token->type != TOKEN_TYPE_END_BLOCK && token->type != TOKEN_TYPE_EOL){
        empty_block = false;
        printf("                                 ITS EMPTY BLOCK_FALSE WORKS  %s\n", token->data);
    }
        


    printf("DEEP = %d\n\n", deep);
    // E N D    B L O C K   ( L A S T )
    if(token->type == TOKEN_TYPE_END_BLOCK && deep == -1)
        function_accept = true;
    
    // E O L
    else if(token->type == TOKEN_TYPE_EOL){
        get_and_set_token();
        printf("HI FROM FUNCTION_BODY %s\n", token->data);
        function_accept = function_body();
    }

    // S T A R T    B L O C K
    else if(token->type == TOKEN_TYPE_START_BLOCK){
        get_and_set_token();
        deep++;
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            printf("HI FROM FUNCTION_BODY %s\n", token->data);
            empty_block = true;
            function_accept = function_body();
        }
    }

    // E N D    B L O C K ( N O T    L A S T )
    else if(token->type == TOKEN_TYPE_END_BLOCK && deep != -1){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
                printf("HI FROM FUNCTION_BODY %s\n", token->data);
            function_accept = function_body();
        }

    // F O R
    } else if(token->type == TOKEN_TYPE_FOR){
        get_and_set_token();
        function_accept = for_construction();

    // I F
    } else if(token->type == TOKEN_TYPE_IF){
        get_and_set_token();
        function_accept = if_construction();
    } 
    

    return function_accept;
}

//  ------------------------------------ F O R    C O N S T R U C T I O N ------------------------------------

bool for_construction(){
    bool for_accept = false;
    printf("            HI FROM FOR_CONSTRUCTION %s\n", token->data);
    if(!define(TOKEN_TYPE_SEMICOLON, 1, 1)){
            printf("            here 1\n");
        return false;
    }
    get_and_set_token();
    if(!logic_expression(TOKEN_TYPE_SEMICOLON)){
        printf("            here 2\n");
        return false;
    }
    get_and_set_token();
    if(!define(TOKEN_TYPE_START_BLOCK, 0, 1)){
        printf("            here 3\n");
        return false;
    }
    printf("            HI FROM FOR_CONSTRUCTION after here3 %s\n", token->data);
    if(token->type == TOKEN_TYPE_START_BLOCK){
        get_and_set_token();
        deep++;                                 
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            for_accept = function_body();
            printf("            here 4\n");
        }
    }

    return for_accept;
}

//  ------------------------------------ I F    C O N S T R U C T I O N ------------------------------------

bool if_construction()
{
    bool if_accept = false;
    if_accept = logic_expression(TOKEN_TYPE_START_BLOCK);
    if (if_accept){
        if_accept = 0;
        deep++;
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
                printf("                                                               IF_ACCEPTED      %d   %s\n", if_accept, token->data);
            if_accept = function_body();

        }
    }

    printf("                                IF_ACCEPTED FINAL       %d   %s\n", if_accept, token->data);
    return if_accept;
}
//  ------------------------------------ L O G I C    E X P R E S S I O N ------------------------------------

bool logic_expression(int end_condition){
    bool logic_expression = false;
    if(token->type == end_condition)
        logic_expression = true;
    else {
        logic_expression = expression(TOKEN_TYPE_LOGICAL_OPERATOR); //left side + operator
        printf("                            CONDITION1 returns token %s\n", token->data);
        if(logic_expression){
            get_and_set_token();
            printf("                            CONDITION2 returns token %s\n", token->data);
            logic_expression = expression(end_condition); //right side + semicolon (for)
        }
    }
    return logic_expression;
}




//  ------------------------------------ D E C L A R E   &   E Q U A T I N G    E X P R E S S I O N ------------------------------------


bool define(int end_condition, int declare, int equating){
    bool define_accept = 0;

    if(token->type == end_condition)
        define_accept = true;
    else {

        printf("                            DEFINE1 returns token [%d] %s\n", end_condition, token->data);
        define_accept = define_operands();
        printf("                            DEFINE2 returns token %s\n", token->data);
        if(declare && define_accept && token->type == TOKEN_TYPE_DECLARE){
            get_and_set_token();
            printf("                            DEFINE3 returns token %s\n", token->data);
            define_accept = count_operands(end_condition);
        } else if (equating && define_accept && token->type == TOKEN_TYPE_EQUATING){
                        printf("                            DEFINE4 (EQUATING) returns token %s\n", token->data);
            get_and_set_token();
            define_accept = count_operands(end_condition);
        }
    }

    return define_accept;
}

bool define_operands(){
    bool operands_accept = false;

    if(token->type == TOKEN_TYPE_IDENTIFIER){
        number_of_operands++;
                        printf("                                      DEFINE2 returns token %d %s\n", number_of_operands, token->data);

        get_and_set_token();
        if(token->type == TOKEN_TYPE_COMMA){
            get_and_set_token();
            operands_accept = define_operands();
        } else
            operands_accept = true;
    }
    return operands_accept;
}

// FUNCTION FOR RECURSIVE CALLING EXPRESSIONS + COUNT OF OPERANDS
bool count_operands(int end_condition){
    int current_end_condition = 0;   // for a, b = 2+1, 3+1   ',' -> end_condition
    bool count_operands_accept = false;
    number_of_operands--;

    if(number_of_operands > 0)
        current_end_condition = TOKEN_TYPE_COMMA;
    else
        current_end_condition = end_condition;
    
                            printf("           COUNT_OPERANDS returns token; end [%d] current end [%d] [%d]\n", end_condition, current_end_condition, number_of_operands);


    count_operands_accept = expression_including_string(current_end_condition);



    if(count_operands_accept && number_of_operands > 0){
        get_and_set_token();
        count_operands_accept = count_operands(end_condition);

    } else if(count_operands_accept && number_of_operands == 0)
        count_operands_accept = true;

    else if (token->type == TOKEN_TYPE_COMMA && number_of_operands == 0){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_IDENTIFIER){
            printf("WRONG COUNT OF OPPS <\n");
            exit(1);
        }
    } else if(token->type == end_condition && number_of_operands > 0){
        printf("WRONG COUNT OF OPPS >\n");
        exit(1);
    }

    return count_operands_accept;
}


//  ------------------------------------ E X P R E S S I O N ------------------------------------

bool expression_including_string(int end_condition){   // MAYBE WORKS
    bool including_string_accept = false;
    if(token->type == TOKEN_TYPE_STRING){
        get_and_set_token();
        if(token->type == end_condition)
            including_string_accept = true;
    } else
        including_string_accept = expression(end_condition);
    

    return including_string_accept;
}

bool expression(int end_condition){
    bool expression_accept = false;
    static int is_function = 1;
    static int bracket = 0;
    static int closed_bracket_counter = 0;
    if(token->type == TOKEN_TYPE_LEFT_BRACKET){
        bracket++;
        is_function = 0;
                    printf("                            AFTER LEFT BRACKET TOKEN IS returns token %s\n", token->data);
        get_and_set_token();
        expression_accept = expression(end_condition);
    } else if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_IDENTIFIER){

        get_and_set_token();
    
 printf("IM HEEEERE\n");
    // CHECK CLOSED BRACKETS
        closed_bracket_counter = is_closed_bracket();
        if(closed_bracket_counter){
            bracket -= closed_bracket_counter;
        }
    // --
                        printf("                                      EXPR returns token [%d] [%d] [%s]\n", end_condition, token->type, token->data);

        if(token->type == end_condition){
            expression_accept = true;
            is_function = 1;
        } else if (token->type == TOKEN_TYPE_MATH_OPERATOR){
            get_and_set_token();
            is_function = 0;
            expression_accept = expression(end_condition);
        } else if (token->type == TOKEN_TYPE_LEFT_BRACKET && is_function){
            get_and_set_token();
                        printf("                            FUNCTION IN EXPRESSION %s\n", token->data);
            expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА  
            if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
                printf("                                                  HERE %s\n", token->data);
                get_and_set_token();
                                        printf("                            FUNCTION2 IN EXPRESSION %s\n", token->data);
                if(token->type == end_condition){
                    expression_accept = true;
                    is_function = 1;
                }
            }
        }
    }

                        printf("                                      EXPR returns %d [%d] [%d] [%d] [%s]\n", number_of_operands, expression_accept, end_condition, token->type, token->data);

    if(bracket != 0){
            printf("BRACKETS ERROR [%d] !\n", bracket);
            exit(1);
    }
    return expression_accept;
}

// CLOSED_BRACKET_COUNTER
int is_closed_bracket(){
    int closed_bracket_counter = 0;
    printf(" %d %d\n", closed_bracket_counter, token->type);
    while(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        get_and_set_token();
        closed_bracket_counter++;
                printf("CLOSED_BRACKET_COUNTER_plus = %d\n", closed_bracket_counter);
    }
       
    printf("CLOSED_BRACKET_COUNTER = %d\n", closed_bracket_counter);
    return closed_bracket_counter;
    
}


//  ------------------------------------ E X P R E S S I O N    A R G U M E N T S ------------------------------------


// bool function_equating_declare(){
//     bool func_eq_dec_accept = false;

//     if(token->type == TOKEN_TYPE_IDENTIFIER){

//     }

//     return func_eq_dec_accept;
// }

bool expression_func_arguments(){
    bool func_arguments_accept = false;
    
    if(token->type == TOKEN_TYPE_RIGHT_BRACKET)
        func_arguments_accept = true;
    else 
        func_arguments_accept = expression_func_single_argument();
            printf("                            FUNCTION IN EXPRESSION INSIDE %s, %d\n", token->data, func_arguments_accept);
    return func_arguments_accept;
}

bool expression_func_single_argument(){
    bool func_single_argument = false;

    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_LITERAL_FLOAT 
    || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_LITERAL_STRING){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
            func_single_argument = true;
        } else if(token->type == TOKEN_TYPE_COMMA){
            get_and_set_token();
            func_single_argument = expression_func_single_argument();
        }
    }
    return func_single_argument;
}





//  ------------------------------------ M A I N    F U N C T I O N ------------------------------------

int main(){
    program_code = fopen ("file.ifj20", "r");

    token = malloc (sizeof(Token));
    token->next = NULL;
    Token *first = token;
    get_token(token);

    bool result = program_start();
    printf("PROGRAM RESULT:  %d\n", result);


    // while(first != NULL){
    //     printf("data: %s   number: %d \n", first->data, first->type);
    //     first = first->next;
    // }
    dtor(first);
    fclose(program_code);
    return 0;
}