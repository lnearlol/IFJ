#include "parser.h"
<<<<<<< Updated upstream
=======
int token_counter = 1;  // delete later
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
}
=======
    token_counter++;  // delete later
}

//  ------------------------------------ E L S E    S T U C K ------------------------------------

void add_to_else_stack(){
    printf("\n\nADD_TO_ELSE \n");
    if (elseStack == NULL){
        elseStack = malloc(sizeof(else_stack));
        elseStack->deep = deep;
            printf("     deep[%d]    %d\n", deep, elseStack->deep);
    } else {
        else_stack *tmpElseStack = elseStack;
        elseStack = malloc(sizeof(else_stack));
        elseStack->deep = deep;
        elseStack->next = tmpElseStack;
    }
    printf("\n");
}

void delete_from_else_stack(){
     printf("\nDELETE_FROM_ELSE \n");
     printf("     deep[%d]     %d\n", deep, elseStack->deep);
    if(elseStack != NULL){
        else_stack *tmpElseStack = elseStack->next;
        free(elseStack);
        elseStack = tmpElseStack;
    }
        printf("\n");
}


>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    
    printf("HI FROM INPUT_PARAMS %s\n", token->data);
=======
>>>>>>> Stashed changes
    return input_parameters;
}

bool input_single_parameters(){
    bool input_single_parameter = false;
<<<<<<< Updated upstream

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
=======
    if(token->type == TOKEN_TYPE_IDENTIFIER){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
            
            // input_single_parameter = true;
            get_and_set_token();
            if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        printf("              HI FROM OUTPUT_PARAMETERS %s\n", token->data);
        get_and_set_token();
        output_parameters = output_single_parameters();
    }
    
    printf("HI FROM OUTPUT_PARAMS %s\n", token->data);
=======
        get_and_set_token();
        output_parameters = output_single_parameters();
    }
>>>>>>> Stashed changes
    return output_parameters;
}

bool output_single_parameters(){
    bool output_single_parameter = false;
<<<<<<< Updated upstream
    printf("              HI FROM OUTPUT_SINGLE_PARAMETER %s\n", token->data);
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
    bool else_condition_flag = false;
>>>>>>> Stashed changes
    static bool empty_block = false;
    bool function_accept = false;
    if(token->type == TOKEN_TYPE_END_BLOCK){
        deep--;
<<<<<<< Updated upstream
                   printf("                                 ITS EMPTY BLOCK  %d\n", empty_block);
=======
>>>>>>> Stashed changes
        if(empty_block){
            printf("                                 ITS EMPTY BLOCK\n");
            return false;
        }
    } 

    // I G N O R I N G    E M P T Y    B L O C K S   -> { /n  /n }, but not if{ /n }
    if(token->type != TOKEN_TYPE_END_BLOCK && token->type != TOKEN_TYPE_EOL){
        empty_block = false;
<<<<<<< Updated upstream
        printf("                                 ITS EMPTY BLOCK_FALSE WORKS  %s\n", token->data);
    }
        


    printf("DEEP = %d\n\n", deep);
=======
       
        if(elseStack != NULL)
            printf("%d\n", elseStack->deep);
        else 
            printf("ELSE-STACK IS NULL \n");
    }
     printf("                         %d        ITS EMPTY BLOCK_FALSE WORKS  %s\n", token->type, token->data);

    // W O R K I N G    W I T H    E L S E - S T A C K
    if(token->type == TOKEN_TYPE_ELSE && elseStack != NULL){
        if(elseStack->deep == deep)
            else_condition_flag = true;
            delete_from_else_stack();
    } else if (elseStack != NULL) {
        if(elseStack->deep == deep && token->type != TOKEN_TYPE_END_BLOCK)
            delete_from_else_stack();
        else if (elseStack->deep > deep)
            delete_from_else_stack();
    }

>>>>>>> Stashed changes
    // E N D    B L O C K   ( L A S T )
    if(token->type == TOKEN_TYPE_END_BLOCK && deep == -1)
        function_accept = true;
    
    // E O L
    else if(token->type == TOKEN_TYPE_EOL){
        get_and_set_token();
<<<<<<< Updated upstream
        printf("HI FROM FUNCTION_BODY %s\n", token->data);
=======
>>>>>>> Stashed changes
        function_accept = function_body();
    }

    // S T A R T    B L O C K
    else if(token->type == TOKEN_TYPE_START_BLOCK){
<<<<<<< Updated upstream
        get_and_set_token();
        deep++;
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            printf("HI FROM FUNCTION_BODY %s\n", token->data);
            empty_block = true;
            function_accept = function_body();
        }
=======
        empty_block = true;
        function_accept = start_block_new_line();
>>>>>>> Stashed changes
    }

    // E N D    B L O C K ( N O T    L A S T )
    else if(token->type == TOKEN_TYPE_END_BLOCK && deep != -1){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
<<<<<<< Updated upstream
                printf("HI FROM FUNCTION_BODY %s\n", token->data);
            function_accept = function_body();
        }
=======
            function_accept = function_body();
        } else if (token->type == TOKEN_TYPE_ELSE)
            function_accept = function_body();
>>>>>>> Stashed changes

    // F O R
    } else if(token->type == TOKEN_TYPE_FOR){
        get_and_set_token();
        function_accept = for_construction();

    // I F
    } else if(token->type == TOKEN_TYPE_IF){
        get_and_set_token();
        function_accept = if_construction();
<<<<<<< Updated upstream
    } 
=======

    // E L S E
    } else if(token->type == TOKEN_TYPE_ELSE && else_condition_flag){
        get_and_set_token();
        function_accept = start_block_new_line();

    // I D E N T I F I E R    --   D E C L A R E,  E Q U A T I N G,  F U N C T I O N
    }  else if (token->type == TOKEN_TYPE_IDENTIFIER){
        
        if(define_func(TOKEN_TYPE_EOL, 1, 1, true)){
            get_and_set_token();
            function_accept = function_body();   
        }

    // R E T U R N
    } else if (token->type == TOKEN_TYPE_RETURN){
        get_and_set_token();
        function_accept = return_construction();
    }
>>>>>>> Stashed changes
    

    return function_accept;
}

//  ------------------------------------ F O R    C O N S T R U C T I O N ------------------------------------

bool for_construction(){
    bool for_accept = false;
    printf("            HI FROM FOR_CONSTRUCTION %s\n", token->data);
<<<<<<< Updated upstream
    if(!define(TOKEN_TYPE_SEMICOLON, 1, 1)){
            printf("            here 1\n");
=======
    if(!define_func(TOKEN_TYPE_SEMICOLON, 1, 1, false)){
            printf("            here 1 token - %s\n", token->data);
>>>>>>> Stashed changes
        return false;
    }
    get_and_set_token();
    if(!logic_expression(TOKEN_TYPE_SEMICOLON)){
        printf("            here 2\n");
        return false;
    }
    get_and_set_token();
<<<<<<< Updated upstream
    if(!define(TOKEN_TYPE_START_BLOCK, 0, 1)){
        printf("            here 3\n");
        return false;
    }
    printf("            HI FROM FOR_CONSTRUCTION after here3 %s\n", token->data);
=======
    if(!define_func(TOKEN_TYPE_START_BLOCK, 0, 1, false)){
        printf("            here 3\n");
        return false;
    }
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        deep++;
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
                printf("                                                               IF_ACCEPTED      %d   %s\n", if_accept, token->data);
=======
        add_to_else_stack();
        deep++;     
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
>>>>>>> Stashed changes
            if_accept = function_body();

        }
    }

<<<<<<< Updated upstream
    printf("                                IF_ACCEPTED FINAL       %d   %s\n", if_accept, token->data);
    return if_accept;
}
=======
    return if_accept;
}

>>>>>>> Stashed changes
//  ------------------------------------ L O G I C    E X P R E S S I O N ------------------------------------

bool logic_expression(int end_condition){
    bool logic_expression = false;
    if(token->type == end_condition)
        logic_expression = true;
    else {
        logic_expression = expression(TOKEN_TYPE_LOGICAL_OPERATOR); //left side + operator
<<<<<<< Updated upstream
        printf("                            CONDITION1 returns token %s\n", token->data);
        if(logic_expression){
            get_and_set_token();
            printf("                            CONDITION2 returns token %s\n", token->data);
=======
        if(logic_expression){
            get_and_set_token();
>>>>>>> Stashed changes
            logic_expression = expression(end_condition); //right side + semicolon (for)
        }
    }
    return logic_expression;
}




//  ------------------------------------ D E C L A R E   &   E Q U A T I N G    E X P R E S S I O N ------------------------------------
<<<<<<< Updated upstream


bool define(int end_condition, int declare, int equating){
    bool define_accept = 0;
=======
//  ------------------------------------ O R    F U N C T I O N ()    F R O M    B O D Y 

bool define_func(int end_condition, int declare, int equating, bool func){
    bool define_accept = false;
>>>>>>> Stashed changes

    if(token->type == end_condition)
        define_accept = true;
    else {

        printf("                            DEFINE1 returns token [%d] %s\n", end_condition, token->data);
<<<<<<< Updated upstream
        define_accept = define_operands();
        printf("                            DEFINE2 returns token %s\n", token->data);
        if(declare && define_accept && token->type == TOKEN_TYPE_DECLARE){
=======
        define_accept = define_operands(func);
        printf("                            DEFINE2 returns token %s, %d\n", token->data, define_accept);
        if(declare && define_accept && token->type == TOKEN_TYPE_DECLARE){
            
            // token
            // mov token->name, EAX
            
>>>>>>> Stashed changes
            get_and_set_token();
            printf("                            DEFINE3 returns token %s\n", token->data);
            define_accept = count_operands(end_condition);
        } else if (equating && define_accept && token->type == TOKEN_TYPE_EQUATING){
                        printf("                            DEFINE4 (EQUATING) returns token %s\n", token->data);
            get_and_set_token();
            define_accept = count_operands(end_condition);
<<<<<<< Updated upstream
        }
=======
        } else if (func && define_accept && token->type == TOKEN_TYPE_LEFT_BRACKET){
            get_and_set_token();
            define_accept = expression_func_arguments();
            if (define_accept){
                number_of_operands--;
                printf("                            DEFINE5 (FUNCTION) returns token %s\n", token->data);
                get_and_set_token();
                define_accept = true;
            }
        }
        printf("                            NOT DEFINE5 (FUNCTION) returns token %d %s\n", define_accept, token->data);
>>>>>>> Stashed changes
    }

    return define_accept;
}

<<<<<<< Updated upstream
bool define_operands(){
=======
bool define_operands(int func){
   
>>>>>>> Stashed changes
    bool operands_accept = false;

    if(token->type == TOKEN_TYPE_IDENTIFIER){
        number_of_operands++;
<<<<<<< Updated upstream
                        printf("                                      DEFINE2 returns token %d %s\n", number_of_operands, token->data);

        get_and_set_token();
        if(token->type == TOKEN_TYPE_COMMA){
            get_and_set_token();
            operands_accept = define_operands();
        } else
            operands_accept = true;
=======
        if(func){
            get_and_set_token();
            operands_accept = true;
        } else {
            get_and_set_token();
            if(token->type == TOKEN_TYPE_COMMA){
                get_and_set_token();
                operands_accept = define_operands(func);
            } else
                operands_accept = true;
        }
>>>>>>> Stashed changes
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
    
<<<<<<< Updated upstream
                            printf("           COUNT_OPERANDS returns token; end [%d] current end [%d] [%d]\n", end_condition, current_end_condition, number_of_operands);


    count_operands_accept = expression_including_string(current_end_condition);


=======
    count_operands_accept = expression_including_string(current_end_condition);
    printf("          count operands      ZASEL #%d   token [%s], counter [%d], accept [%d] \n", token_counter, token->data, number_of_operands, count_operands_accept);
>>>>>>> Stashed changes

    if(count_operands_accept && number_of_operands > 0){
        get_and_set_token();
        count_operands_accept = count_operands(end_condition);
<<<<<<< Updated upstream

    } else if(count_operands_accept && number_of_operands == 0)
        count_operands_accept = true;

=======
    } else if(count_operands_accept && number_of_operands == 0)
        count_operands_accept = true;
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
  
>>>>>>> Stashed changes

    return count_operands_accept;
}


//  ------------------------------------ E X P R E S S I O N ------------------------------------

bool expression_including_string(int end_condition){   // MAYBE WORKS
    bool including_string_accept = false;
<<<<<<< Updated upstream
    if(token->type == TOKEN_TYPE_STRING){
=======
    if(token->type == TOKEN_TYPE_LITERAL_STRING){   
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
                    printf("                            AFTER LEFT BRACKET TOKEN IS returns token %s\n", token->data);
        get_and_set_token();
        expression_accept = expression(end_condition);
    } else if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_IDENTIFIER){

        get_and_set_token();
    
 printf("IM HEEEERE\n");
=======
        get_and_set_token();
        expression_accept = expression(end_condition);
         
    } else if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_IDENTIFIER){
        get_and_set_token();
>>>>>>> Stashed changes
    // CHECK CLOSED BRACKETS
        closed_bracket_counter = is_closed_bracket();
        if(closed_bracket_counter){
            bracket -= closed_bracket_counter;
        }
    // --
<<<<<<< Updated upstream
                        printf("                                      EXPR returns token [%d] [%d] [%s]\n", end_condition, token->type, token->data);
=======
>>>>>>> Stashed changes

        if(token->type == end_condition){
            expression_accept = true;
            is_function = 1;
        } else if (token->type == TOKEN_TYPE_MATH_OPERATOR){
            get_and_set_token();
            is_function = 0;
            expression_accept = expression(end_condition);
        } else if (token->type == TOKEN_TYPE_LEFT_BRACKET && is_function){
            get_and_set_token();
<<<<<<< Updated upstream
                        printf("                            FUNCTION IN EXPRESSION %s\n", token->data);
            expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА  
            if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
                printf("                                                  HERE %s\n", token->data);
                get_and_set_token();
                                        printf("                            FUNCTION2 IN EXPRESSION %s\n", token->data);
=======
            expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА  
            if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
                get_and_set_token();
>>>>>>> Stashed changes
                if(token->type == end_condition){
                    expression_accept = true;
                    is_function = 1;
                }
            }
        }
<<<<<<< Updated upstream
    }

                        printf("                                      EXPR returns %d [%d] [%d] [%d] [%s]\n", number_of_operands, expression_accept, end_condition, token->type, token->data);

    if(bracket != 0){
            printf("BRACKETS ERROR [%d] !\n", bracket);
=======

        // C O M M A N D    F U N C T I O N S
    } else if (token->type == TOKEN_TYPE_COMMAND_FUNCTION){
        get_and_set_token();
        if (token->type == TOKEN_TYPE_LEFT_BRACKET){
            get_and_set_token();
            expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА  
            if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
                get_and_set_token();
                if(token->type == end_condition){
                    expression_accept = true;
                }
            }
        }
    }


    if(bracket != 0){
>>>>>>> Stashed changes
            exit(1);
    }
    return expression_accept;
}

// CLOSED_BRACKET_COUNTER
int is_closed_bracket(){
    int closed_bracket_counter = 0;
<<<<<<< Updated upstream
    printf(" %d %d\n", closed_bracket_counter, token->type);
    while(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        get_and_set_token();
        closed_bracket_counter++;
                printf("CLOSED_BRACKET_COUNTER_plus = %d\n", closed_bracket_counter);
    }
       
    printf("CLOSED_BRACKET_COUNTER = %d\n", closed_bracket_counter);
    return closed_bracket_counter;
    
=======
    while(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        get_and_set_token();
        closed_bracket_counter++;
    }
    return closed_bracket_counter;
>>>>>>> Stashed changes
}


//  ------------------------------------ E X P R E S S I O N    A R G U M E N T S ------------------------------------

<<<<<<< Updated upstream

// bool function_equating_declare(){
//     bool func_eq_dec_accept = false;

//     if(token->type == TOKEN_TYPE_IDENTIFIER){

//     }

//     return func_eq_dec_accept;
// }

=======
>>>>>>> Stashed changes
bool expression_func_arguments(){
    bool func_arguments_accept = false;
    
    if(token->type == TOKEN_TYPE_RIGHT_BRACKET)
        func_arguments_accept = true;
    else 
        func_arguments_accept = expression_func_single_argument();
<<<<<<< Updated upstream
            printf("                            FUNCTION IN EXPRESSION INSIDE %s, %d\n", token->data, func_arguments_accept);
=======
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream



=======
//  ------------------------------------ R E T U R N    C O N S T R U C T I O N ------------------------------------

bool return_construction(){
    bool return_construction_accept = false;

    return_construction_accept = expression_including_string(TOKEN_TYPE_EOL);
    printf("RETURN %d\n", return_construction_accept);
    if(return_construction_accept){
        get_and_set_token();
        return_construction_accept = function_body();
    } else if(token->type == TOKEN_TYPE_EOL){
        get_and_set_token();
        return_construction_accept = function_body();
    }
    return return_construction_accept;
}

//  ------------------------------------ S T A R T    B L O C K    &    N E W    L I N E ------------------------------------


bool start_block_new_line(){
    bool start_block_accept = false;
    if(token->type == TOKEN_TYPE_START_BLOCK){
        get_and_set_token();
        deep++;
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            start_block_accept = function_body();
        } 
    }
    return start_block_accept;
}
>>>>>>> Stashed changes

//  ------------------------------------ M A I N    F U N C T I O N ------------------------------------

int main(){
    program_code = fopen ("file.ifj20", "r");

    token = malloc (sizeof(Token));
    token->next = NULL;
    Token *first = token;
    get_token(token);

    bool result = program_start();
    printf("PROGRAM RESULT:  %d\n", result);

<<<<<<< Updated upstream

    // while(first != NULL){
    //     printf("data: %s   number: %d \n", first->data, first->type);
=======
    // int i = 0;
    // while(first != NULL){
    //     printf("[%d] data: %s   number: %d \n", ++i, first->data, first->type);
>>>>>>> Stashed changes
    //     first = first->next;
    // }
    dtor(first);
    fclose(program_code);
    return 0;
}