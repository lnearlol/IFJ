#include "parser.h"
int token_counter = 1;  // delete later
int deep = -1;
int number_of_operands = 0;
int PROGRAMM_RUN = FIRST_RUN;
int error_flag = 0;






void get_and_set_token(){
        if(PROGRAMM_RUN == FIRST_RUN){
        token->next = malloc (sizeof(Token));
        token = token->next;
        token->next = NULL;
        if (get_token(token) == 1)
            error_flag = 1;
        token_counter++;  // delete later
    } else if (PROGRAMM_RUN == SECOND_RUN)
        token = token->next;


}

//  ------------------------------------ E L S E    S T A C K ------------------------------------

void add_to_else_stack(){
    printf("\n\nADD_TO_ELSE \n");
    if (elseStack == NULL){
        elseStack = malloc(sizeof(else_stack));
        elseStack->deep = deep;
        elseStack->next = NULL;
            printf("     deep[%d]    %d\n", deep, elseStack->deep);
    } else {
        else_stack *tmpElseStack = elseStack;
        elseStack = malloc(sizeof(else_stack));
        elseStack->deep = deep;
        elseStack->next = tmpElseStack;
        elseStack->next->next = NULL;
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
                    program_start = function_check();
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

bool function_check(){
    bool func = false;
    if(token->type == TOKEN_TYPE_FUNC){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_IDENTIFIER){
            // A D D    F U N C T I O N S    T O    S Y M T A B L E
            if(PROGRAMM_RUN){          
                insertFunction(token, &(SymTable->func));
                saved_func_name = token;
            }
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
                                if(PROGRAMM_RUN == FIRST_RUN)
                                    func = first_run_body();
                                else
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
    return input_parameters;
}

bool input_single_parameters(){
    bool input_single_parameter = false;
    if(token->type == TOKEN_TYPE_IDENTIFIER){
        if(PROGRAMM_RUN){
            if(strcmp(saved_func_name->data, "main") == 0)      // I F    M A I N   H A S   I N P U T   P A R A M S
                return false;  // error_flag = 4to-to

            saved_arg_name = token;
        }
        get_and_set_token();
        if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){

            if(PROGRAMM_RUN){        // A D D   F U N C   A R G S    T O    S Y M T A B L E
                saved_arg_type = token;
                printf("INPUTS PARAMS -%s -%s -%s  -%s\n", saved_func_name->data, saved_arg_name->data, saved_arg_type->data, SymTable->func->name);
                addInputArguments(saved_func_name, saved_arg_name, saved_arg_type, SymTable->func);
            }
            // input_single_parameter = true;
            get_and_set_token();
            if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
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
        get_and_set_token();
        output_parameters = output_single_parameters();
    }
    return output_parameters;
}

bool output_single_parameters(){
    bool output_single_parameter = false;
    if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
        if(PROGRAMM_RUN){
            saved_arg_type = token;
            printf("OUTPUTS PARAMS -%s -%s -%s  -%s\n", saved_func_name->data, saved_arg_name->data, saved_arg_type->data, SymTable->func->name);
            addOutputArguments(saved_func_name, saved_arg_type, SymTable->func);
        }
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

// //  ------------------------------------  B O D Y    1    R U N  ------------------------------------

bool first_run_body(){
    bool first_run_accept = true;


    while(1){
        if(token->type == TOKEN_TYPE_END_BLOCK)
            deep--;
        if(token->type == TOKEN_TYPE_START_BLOCK)
            deep++;
        if(deep == -1)
            break;
        get_and_set_token();
    }

    return first_run_accept;
}

//  ------------------------------------ F U N C T I O N    B O D Y ------------------------------------

bool function_body(){
    bool else_condition_flag = false;
    static bool empty_block = false;
    bool function_accept = false;
    if(token->type == TOKEN_TYPE_END_BLOCK){
        deep--;
        if(empty_block){
            printf("                                 ITS EMPTY BLOCK\n");
            return false;
        }
    }

    // I G N O R I N G    E M P T Y    B L O C K S   -> { /n  /n }, but not if{ /n }
    if(token->type != TOKEN_TYPE_END_BLOCK && token->type != TOKEN_TYPE_EOL){
        empty_block = false;
       
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

    // E N D    B L O C K   ( L A S T )
    if(token->type == TOKEN_TYPE_END_BLOCK && deep == -1)
        function_accept = true;
    
    // E O L
    else if(token->type == TOKEN_TYPE_EOL){
        get_and_set_token();
        function_accept = function_body();
    }

    // S T A R T    B L O C K
    else if(token->type == TOKEN_TYPE_START_BLOCK){
        empty_block = true;
        function_accept = start_block_new_line();
    }

    // E N D    B L O C K ( N O T    L A S T )
    else if(token->type == TOKEN_TYPE_END_BLOCK && deep != -1){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            // get_and_set_token();
            function_accept = function_body();
        } else if (token->type == TOKEN_TYPE_ELSE)
            function_accept = function_body();

    // F O R
    } else if(token->type == TOKEN_TYPE_FOR){
        get_and_set_token();
        function_accept = for_construction();

    // I F
    } else if(token->type == TOKEN_TYPE_IF){
        get_and_set_token();
        function_accept = if_construction();

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

    // U N D E R S C O R E  --  D E C L A R E,  E Q U A T I N G
    } else if (token->type == TOKEN_TYPE_UNDERSCORE){
        
        if(define_func(TOKEN_TYPE_EOL, 1, 1, false)){
            get_and_set_token();
            function_accept = function_body();   
        }

    // R E T U R N
    } else if (token->type == TOKEN_TYPE_RETURN){
        get_and_set_token();
        function_accept = return_construction();
    }
    

    return function_accept;
}

//  ------------------------------------ F O R    C O N S T R U C T I O N ------------------------------------

bool for_construction(){
    bool for_accept = false;
    printf("            HI FROM FOR_CONSTRUCTION %s\n", token->data);
    if(!define_func(TOKEN_TYPE_SEMICOLON, 1, 1, false)){
            printf("            here 1 token - %s\n", token->data);
        return false;
    }
    get_and_set_token();
    if(!logic_expression(TOKEN_TYPE_SEMICOLON)){
        printf("            here 2\n");
        return false;
    }
    get_and_set_token();
    if(!define_func(TOKEN_TYPE_START_BLOCK, 0, 1, false)){
        printf("            here 3\n");
        return false;
    }
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
        add_to_else_stack();
        deep++;
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            if_accept = function_body();
        }
    }

    return if_accept;
}

//  ------------------------------------ L O G I C    E X P R E S S I O N ------------------------------------

bool logic_expression(int end_condition){
    bool logic_expression = false;
    if(token->type == end_condition)
        logic_expression = true;
    else {
        logic_expression = expression(TOKEN_TYPE_LOGICAL_OPERATOR); //left side + operator
        if(logic_expression){
            get_and_set_token();
            logic_expression = expression(end_condition); //right side + semicolon (for)
        }
    }
    return logic_expression;
}




//  ------------------------------------ D E C L A R E   &   E Q U A T I N G    E X P R E S S I O N ------------------------------------
//  ------------------------------------ O R    F U N C T I O N ()    F R O M    B O D Y 

bool define_func(int end_condition, int declare, int equating, bool func){
    bool define_accept = false;
printf("                            IIIIIIIN DEEEFINE  FUNC %s\n", token->data);
    if(token->type == end_condition)
        define_accept = true;
    else {
        
        printf("                            DEFINE1 returns token [%d] %s\n", end_condition, token->data);
        define_accept = define_operands(func);
        printf("                            DEFINE2 returns token %s, %d\n", token->data, define_accept);
        if(declare && define_accept && token->type == TOKEN_TYPE_DECLARE){
            
            // token
            // mov token->name, EAX
            
            get_and_set_token();
            printf("                            DEFINE3 returns token %s\n", token->data);
            define_accept = count_operands(end_condition);
        } else if (equating && define_accept && token->type == TOKEN_TYPE_EQUATING){
                        printf("                            DEFINE4 (EQUATING) returns token %s\n", token->data);
            get_and_set_token();
            define_accept = count_operands(end_condition);
        } else if (func && define_accept && token->type == TOKEN_TYPE_LEFT_BRACKET){
            get_and_set_token();
            define_accept = expression_func_arguments();
            if (define_accept){
                number_of_operands--;
                printf("                            DEFINE5 (FUNCTION) returns token %s\n", token->data);
                get_and_set_token();
                define_accept = true;
            }
        } else
            define_accept = false;
        printf("                            NOT DEFINE5 (FUNCTION) returns token %d %s\n", define_accept, token->data);
    }

    return define_accept;
}

bool define_operands(int func){
   
    bool operands_accept = false;

    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_UNDERSCORE){
        number_of_operands++;
        // C H E C K   E X I S T I N G   F U N C T I O N   L O G I C
        saved_func_name = token;
        printf("define ops   token->data = %s\n", token->data);

        get_and_set_token();
        printf("define ops   token->next->data = %s\n", token->data);
        if(func && number_of_operands == 1 && token->type == TOKEN_TYPE_LEFT_BRACKET && saved_func_name == TOKEN_TYPE_IDENTIFIER){
            // S Y M T A B L E    L O G I C
            if(findFunction(saved_func_name, SymTable->func))       
                operands_accept = true;
            else {
                operands_accept = false;
                error_flag = 3;   //  ERROR 3
            }
        } else {
            if(token->type == TOKEN_TYPE_COMMA){
                get_and_set_token();
                operands_accept = define_operands(func);
            } else
                operands_accept = true;
        }
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
    
    count_operands_accept = expression_including_string(current_end_condition);
    printf("          count operands      ZASEL #%d   token [%s], counter [%d], accept [%d] \n", token_counter, token->data, number_of_operands, count_operands_accept);

    if(count_operands_accept && number_of_operands > 0){
        get_and_set_token();
        count_operands_accept = count_operands(end_condition);
    } else if(count_operands_accept && number_of_operands == 0)
        count_operands_accept = true;
    else if (token->type == TOKEN_TYPE_COMMA && number_of_operands == 0){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_IDENTIFIER){
            printf("WRONG COUNT OF OPPS <\n");
            count_operands_accept = false;
        }
    } else if(token->type == end_condition && number_of_operands > 0){
        printf("WRONG COUNT OF OPPS >\n");
        count_operands_accept = false;
    }
  

    return count_operands_accept;
}


//  ------------------------------------ E X P R E S S I O N ------------------------------------

bool expression_including_string(int end_condition){   // MAYBE WORKS
    bool including_string_accept = false;
    if(token->type == TOKEN_TYPE_LITERAL_STRING){   
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
        get_and_set_token();
        expression_accept = expression(end_condition);
         
    } else if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_IDENTIFIER){

        // C H E C K   E X I S T I N G   F U N C T I O N   L O G I C
        saved_func_name = token;

        get_and_set_token();
    // CHECK CLOSED BRACKETS
        closed_bracket_counter = is_closed_bracket();
        if(closed_bracket_counter){
            bracket -= closed_bracket_counter;
        }
    // --

        if(token->type == end_condition){
            expression_accept = true;
            is_function = 1;
        } else if (token->type == TOKEN_TYPE_MATH_OPERATOR){
            get_and_set_token();
            is_function = 0;
            expression_accept = expression(end_condition);
        } else if (token->type == TOKEN_TYPE_LEFT_BRACKET && is_function){
            // S Y M T A B L E    L O G I C
            if(!findFunction(saved_func_name, SymTable->func)){
                error_flag = 3;   //  ERROR 3
                return false;
            }
            get_and_set_token();
            expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА  
            if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
                get_and_set_token();
                if(token->type == end_condition){
                    expression_accept = true;
                    is_function = 1;
                }
            }
        }

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
        expression_accept = false;
    }
    return expression_accept;
}

// CLOSED_BRACKET_COUNTER
int is_closed_bracket(){
    int closed_bracket_counter = 0;
    while(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        get_and_set_token();
        closed_bracket_counter++;
    }
    return closed_bracket_counter;
}


//  ------------------------------------ E X P R E S S I O N    A R G U M E N T S ------------------------------------

bool expression_func_arguments(){
    bool func_arguments_accept = false;
    
    // F U N C T I O N   A R G U M E N T S   L O G I C
    function arg_find = findFunction(saved_func_name, SymTable->func);
    if(!arg_find) {
        error_flag = 3;
        return false;
    }
    inputParams args_check = arg_find->input_params;

    if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        // LOGIC
        if(args_check == NULL)
            func_arguments_accept = true;
        else 
            error_flag = 3;
    } else {
        func_arguments_accept = expression_func_single_argument(args_check);
    }
    return func_arguments_accept;
}

bool expression_func_single_argument(inputParams args_check){
    bool func_single_argument = false;

    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_LITERAL_FLOAT 
    || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_LITERAL_STRING){
        if(args_check != NULL){
            // F U N C T I O N   A R G U M E N T S   L O G I C  --   C H E C K    L I T E R A L    T Y P E
            // S Y M T A B L E   F U N C T I O N  (returns  bool/int)
            if(token->type == TOKEN_TYPE_LITERAL_INT && args_check->type != TOKEN_TYPE_INT){
                printf("             arg name - %s\n", args_check->name);
                error_flag = 3;
                return false;
            } else if (token->type == TOKEN_TYPE_LITERAL_FLOAT && args_check->type != TOKEN_TYPE_FLOAT){
                printf("             arg name - %s\n", args_check->name);
                error_flag = 3;
                return false;            
            } else if (token->type == TOKEN_TYPE_LITERAL_STRING && args_check->type != TOKEN_TYPE_STRING){
                printf("             arg name - %s\n", args_check->name);
                error_flag = 3;
                return false;            
            }
        } else {
            error_flag = 3;
                return false;     
        }
        // ---------
        get_and_set_token();
        if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
            // L O G I C   C H E C K    [ args_check->next is NULL, it was the last parameter in SymTable->Func->InputParams ]
            if(args_check->next == NULL)
                func_single_argument = true;
            else {
                error_flag = 3;
                func_single_argument = false;
            }
        } else if(token->type == TOKEN_TYPE_COMMA){
            get_and_set_token();
            func_single_argument = expression_func_single_argument(args_check->next);
        }
    }
    return func_single_argument;
}

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

//  ------------------------------------ M A I N    F U N C T I O N ------------------------------------

int main(){
   // program_code = fopen ("file.ifj20", "r");
   SymTable = declaration(SymTable);

    saved_func_name = saved_arg_name = saved_arg_type = NULL;

    token = malloc (sizeof(Token));
    token->next = NULL;
    Token *second_run = token;

    if(get_token(token) == 1)
        error_flag = 1;

    if(error_flag == 0){
        bool result = program_start();

        if(!result && error_flag == 0){
            printf("\n\n                                                 PROGRAM FINISHED ERROR 2\n");
            error_flag = 2;
        } 
    }
     printf("\n\n                                                 PROGRAM FINISHED 1: [%d]\n", error_flag);
    PROGRAMM_RUN = SECOND_RUN;
    token = second_run;
    if(error_flag == 0){
        bool result = program_start();

        if(!result && error_flag == 0){
            printf("\n\n                                                 PROGRAM FINISHED ERROR 2\n");
            error_flag = 2;
        } 
    printf("\n\n                                            PROGRAM FINISHED 2: [ERROR CODE %d]\n", error_flag);
    }
    //printf("------- %d\n", SymTable->func->LPtr->LPtr->output_params->next->type);


    Print_func(SymTable->func);
    
    // int i = 0;
    // while(second_run != NULL){
    //     printf("[%d] data: %s   number: %d \n", ++i, second_run->data, second_run->type);
    //     second_run = second_run->next;
    // }
    freeFunctions(&(SymTable->func));
    free(SymTable);
    dtor(second_run);
   // fclose(program_code);
    return error_flag;
}