#include "parser.h"

bool delete_expr_stack = false;
int token_counter = 1;  // delete later
int deep = -1;
int number_of_operands = 0;
int PROGRAMM_RUN = FIRST_RUN;
int error_flag = 0;
int return_in_function = true;
bool wasMainInProgram = false;
bool WAS_CONDITION = false;


genFrameType GEN_FRAME = LOCAL;

int GET_GEN_FRAME(){
    return GEN_FRAME;
}

void CHANGE_GEN_FRAME(int Frame){
    GEN_FRAME = Frame;
}



void get_and_set_token(){
        if(PROGRAMM_RUN == FIRST_RUN){
        token->next = malloc (sizeof(Token));
        token = token->next;
        token->next = NULL;
        if (get_token(token) == 1)
            changeErrorCode(1);
        token_counter++;  // delete later
    } else if (PROGRAMM_RUN == SECOND_RUN)
        token = token->next;
}

//  ------------------------------------ E R R O R    C O D E ------------------------------------

void changeErrorCode(int code){
    if(error_flag == 0)
        error_flag = code;
}


//  ------------------------------------ F O R   S T A C K ------------------------------------

void add_to_for_stack(){
    if (forStack == NULL){
        forStack = malloc(sizeof(else_stack));
        forStack->deep = deep;
        forStack->next = NULL;
    } else {
        else_stack *tmpforStack = forStack;
        forStack = malloc(sizeof(else_stack));
        forStack->deep = deep;
        forStack->next = tmpforStack;
        forStack->next->next = NULL;
    }
}

void delete_from_for_stack(){
    if(forStack != NULL){
        else_stack *tmpforStack = forStack->next;
        free(forStack);
        forStack = tmpforStack;
    }
}


//  ------------------------------------ E L S E    S T A C K ------------------------------------

void add_to_else_stack(){
    if (elseStack == NULL){
        elseStack = malloc(sizeof(else_stack));
        elseStack->deep = deep;
        elseStack->next = NULL;
    } else {
        else_stack *tmpElseStack = elseStack;
        elseStack = malloc(sizeof(else_stack));
        elseStack->deep = deep;
        elseStack->next = tmpElseStack;
        elseStack->next->next = NULL;
    }
}

void delete_from_else_stack(){
    if(elseStack != NULL){
        else_stack *tmpElseStack = elseStack->next;
        free(elseStack);
        elseStack = tmpElseStack;
    }
}

//  ------------------------------------ C O M P A R E    L I S T ------------------------------------

void add_var_to_compare_list(Token *var){
    if(varCompareList == NULL){
        varCompareList = malloc(sizeof(variables_compare_list));
        varCompareList->var = var;
        varCompareList->next = NULL;
    } else {
        variables_compare_list *tmpList = malloc(sizeof(variables_compare_list));
        tmpList->var = var;
        tmpList->next = NULL;
        variables_compare_list *cycleList = varCompareList;
        while(cycleList->next != NULL){
            cycleList = cycleList->next;
        }
        cycleList->next = tmpList;
    }
    variables_compare_list *tmp = varCompareList;
    while(tmp != NULL){
        tmp = tmp->next;
    }
}

void delete_var_from_compare_list(){
    if(varCompareList != NULL){
        variables_compare_list *tmpList = varCompareList->next;
        free(varCompareList);
        varCompareList = tmpList;
    }
}

void add_type_to_compare_list(int type){
    if(typeCompareList == NULL){
        typeCompareList = malloc(sizeof(type_compare_list));
        typeCompareList->type = type;
        typeCompareList->next = NULL;
    } else {
        type_compare_list *tmpTypeList = malloc(sizeof(type_compare_list));
        tmpTypeList->type = type;
        tmpTypeList->next = NULL;
        type_compare_list *cycleTypeList = typeCompareList;
        while(cycleTypeList->next != NULL)
            cycleTypeList = cycleTypeList->next;
        cycleTypeList->next = tmpTypeList;
    }

    type_compare_list *tmp = typeCompareList;
    while(tmp != NULL){
        tmp = tmp->next;
    }
}

void delete_type_from_compare_list(){

    if(typeCompareList != NULL){
        type_compare_list *tmpList = typeCompareList->next;
        free(typeCompareList);
        typeCompareList = tmpList;
    }
}

bool checkCompareLists(){
    if (typeCompareList != NULL  || varCompareList != NULL){
        changeErrorCode(3);
        return false;
    } else 
        return true;
}

int compareCompareLists(){
    variables_compare_list *tmpVar = varCompareList;
    type_compare_list *tmpType = typeCompareList;
    while(tmpVar != NULL && tmpType != NULL){
        tmpVar = tmpVar->next;
        tmpType = tmpType->next;
    }
    if(tmpVar == NULL && tmpType == NULL){
        return 0;
    } else if (tmpVar != NULL){
        return 1;
    } else {
        return -1;
    }
}


void freeBothCompareLists(){
    while(varCompareList != NULL || typeCompareList != NULL){
        if(typeCompareList != NULL)
        delete_type_from_compare_list();
        delete_var_from_compare_list();
    }
}

//  ------------------------------------ P R O G R A M    S T A R T ------------------------------------
// package main \n func() EOF
bool program_start(){
    bool program_start = false;
    allowed_eol();
    if(token->type == TOKEN_TYPE_PACKAGE){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_IDENTIFIER && !strcmp(token->data, "main")){
            get_and_set_token();
            if(token->type == TOKEN_TYPE_EOL){
                get_and_set_token();
                allowed_eol();
                while(token->type != TOKEN_TYPE_EOFILE){
                    program_start = function_check();
                    if(program_start == false){
                        return program_start;
                    }
                    get_and_set_token();  // when we return this function as valid we should get next token to compare with EOF
                }
            }
        } else if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_COMMAND_FUNCTION){
            changeErrorCode(7);
        }
    } 

    return program_start;
}

//  ------------------------------------ F U N C T I O N ------------------------------------

bool function_check(){
    bool func = false;
    if(token->type == TOKEN_TYPE_FUNC){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_IDENTIFIER){
            if(!strcmp(token->data, "main"))
                wasMainInProgram = true;
            // A D D    F U N C T I O N S    T O    S Y M T A B L E
            if(PROGRAMM_RUN){          
                insertFunction(token, &(SymTable->func));
            } else 
                GEN_START_OF_FUNCTION(token);
            current_function_name = token; // saved name of current function
            saved_func_name = token; // save function name for working with arguments
            get_and_set_token();
            if(token->type == TOKEN_TYPE_LEFT_BRACKET){
                get_and_set_token();
                func = input_parameters();
                if(func){
                    get_and_set_token();

                    func = output_parameters();
                    if(func){
                        func = false;

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
            if(strcmp(saved_func_name->data, "main") == 0){      // I F    M A I N   H A S   I N P U T   P A R A M S
                changeErrorCode(6);
                return false;  // error_flag = 4to-to
            }   
        }
        saved_arg_name = token; // foo(a int) -> saved_arg_name = a

        get_and_set_token();
        if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
            saved_arg_type = token; // // foo(a int) -> saved_arg_type = int
            if(PROGRAMM_RUN){        // A D D   F U N C   A R G S    T O    S Y M T A B L E
               addInputArguments(saved_func_name, saved_arg_name, saved_arg_type, SymTable->func);
            } else { // R U N 2,   C R E A T I N G    A R G U M E N T S   A S    Z E R O    L E V E L     V A R S
                insertVariable(saved_arg_name, deep+1, &(SymTable->var));  // var
                putTypeVariable(saved_arg_name, deep+1, saved_arg_type->type, SymTable->var);  // var_type
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
                input_single_parameter = false;
            }
        }
    } 

    return input_single_parameter;
}

//  ------------------------------------ O U T P U T    P A R A M E T E R S ------------------------------------

bool output_parameters(){
    bool output_parameters = false;
    if(token->type == TOKEN_TYPE_START_BLOCK)
        output_parameters = true;
    else if (token->type == TOKEN_TYPE_LEFT_BRACKET){
        get_and_set_token();
        if (token->type == TOKEN_TYPE_RIGHT_BRACKET){
            get_and_set_token();
            output_parameters = true;
        } else {
            if(PROGRAMM_RUN == SECOND_RUN){
                outputParams out = findFunction(saved_func_name, SymTable->func)->output_params;
                // GEN_RETVAL_CREATER(out);
            }
            output_parameters = output_single_parameters();
        }
    }
    return output_parameters;
}

bool output_single_parameters(){
    bool output_single_parameter = false;
    if(PROGRAMM_RUN){
            if(strcmp(saved_func_name->data, "main") == 0){      // I F    M A I N   H A S   I N P U T   P A R A M S
                changeErrorCode(6);
                return false;  // error_flag = 4to-to
            }   
        }
    if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
        
        if(PROGRAMM_RUN){
            saved_arg_type = token;
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

//  ------------------------------------  B O D Y    1    R U N  ------------------------------------

bool first_run_body(){
    bool first_run_accept = true;


    while(1){
        if(token->type == TOKEN_TYPE_END_BLOCK)
            deep--;
        if(token->type == TOKEN_TYPE_START_BLOCK)
            deep++;
        if(deep == -1 || error_flag == 1)
            break;
        if(token->type == TOKEN_TYPE_EOFILE){
            changeErrorCode(2);
            return false;
        }
        get_and_set_token();
    }

    return first_run_accept;
}

//  ------------------------------------ F U N C T I O N    B O D Y ------------------------------------

bool function_body(){
    bool else_condition_flag = false;
    bool for_condition_flag = false;
    static bool empty_block = false;
    bool function_accept = false;


    if(token->type == TOKEN_TYPE_END_BLOCK){
        if(SymTable->var != NULL && deep == SymTable->var->deep)
            freeVariablesLastLabel(&(SymTable->var));
        
        deep--;
        if(empty_block){
            return false;
        }
    }

    // I G N O R I N G    E M P T Y    B L O C K S   -> { /n  /n }, but not if{ /n }
    if(token->type != TOKEN_TYPE_END_BLOCK && token->type != TOKEN_TYPE_EOL){
        empty_block = false;
    }

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

    // W O R K I N G    W I T H    F O R - S T A C K
    if(token->type == TOKEN_TYPE_END_BLOCK && forStack != NULL){
        if(forStack->deep == deep){
            delete_from_for_stack();
            if(SymTable->var != NULL && deep == SymTable->var->deep)
                freeVariablesLastLabel(&(SymTable->var));
            deep--;
        }
    } 


    // E N D    B L O C K   ( L A S T )
    if(token->type == TOKEN_TYPE_END_BLOCK && deep == -1) {
        function_accept = true;
        if(findFunction(current_function_name, SymTable->func)->output_params != NULL && return_in_function == true){ // was not return command
                changeErrorCode(6); // number of return args and function output args not the same
                return false;
        }
        GEN_END_OF_FUNCTION(current_function_name);
    // E O L
    } else if(token->type == TOKEN_TYPE_EOL){
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
    }  else if (token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_COMMAND_FUNCTION){
        
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
        return_in_function = false;
        get_and_set_token();
        function F = findFunction(current_function_name, SymTable->func);
        outputParams out_params = F->output_params;      // C O M P A R E   R E T U R N   A N D   O U T   P A R A M S

        // GEN_RETVAL_RETURN(out_params); // A S S E M B L Y

        if(token->type == TOKEN_TYPE_EOL && out_params == NULL)
            function_accept = function_body();
        else if(out_params != NULL && token->type != TOKEN_TYPE_EOL)
            function_accept = return_construction(out_params);
        else {
            changeErrorCode(6); // number of return args and function output args not the same
            return false;
        }
        
    }
    

    return function_accept;
}

//  ------------------------------------ F O R    C O N S T R U C T I O N ------------------------------------

bool for_construction(){
    bool for_accept = false;
    deep++;
    if(!define_func(TOKEN_TYPE_SEMICOLON, 1, 1, false)){
        return false;
    }
    get_and_set_token();
    expr = createStack();
    if(token->type != TOKEN_TYPE_SEMICOLON && !expression(TOKEN_TYPE_SEMICOLON)){
        return false;
    }
    get_and_set_token();
    if(!WAS_CONDITION){
        changeErrorCode(5);
        return false;
    }
    WAS_CONDITION = false;
    freeBothCompareLists();

    if(!define_func(TOKEN_TYPE_START_BLOCK, 0, 1, false)){
        return false;
    }
    if(token->type == TOKEN_TYPE_START_BLOCK){
        get_and_set_token();
        add_to_for_stack();
        deep++;                                 
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            for_accept = function_body();
        }
    }


    return for_accept;
}

//  ------------------------------------ I F    C O N S T R U C T I O N ------------------------------------

bool if_construction()
{
    expr = createStack();
    bool if_accept = false;
    if_accept = expression(TOKEN_TYPE_START_BLOCK);
    if(if_accept){
        add_to_else_stack();
        deep++;
    }
    if (if_accept && WAS_CONDITION){
        WAS_CONDITION = false;
        get_and_set_token();
        if_accept = 0;
        freeBothCompareLists();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            if_accept = function_body();
        }
    }


    return if_accept;
}

//  ------------------------------------ L O G I C    E X P R E S S I O N ------------------------------------

// bool logic_expression(int end_condition){
//     bool logic_expression = false;
    
//         delete_expr_stack = true;
//         expr = createStack();
        
//         logic_expression = expression(TOKEN_TYPE_LOGICAL_OPERATOR); //left side + operator
        
//         if(logic_expression && WAS_CONDITION){
//             get_and_set_token();
//             delete_expr_stack = true;
//             expr = createStack();
            
//             logic_expression = expression(end_condition); //right side + semicolon (for)

//             if(typeCompareList != NULL && typeCompareList->next != NULL){
//                 if(typeCompareList->type != typeCompareList->next->type){
//                     changeErrorCode(5);
//                     logic_expression = false;
//                 }
//             }
//         }
//         freeBothCompareLists(); // free the logic list
    
//     return logic_expression;
// }




//  ------------------------------------ D E C L A R E   &   E Q U A T I N G    E X P R E S S I O N ------------------------------------
//  ------------------------------------ O R    F U N C T I O N ()    F R O M    B O D Y 

bool define_func(int end_condition, int declare, int equating, bool func){
    bool define_accept = false;
    number_of_operands = 0;

    if(token->type == end_condition)
        define_accept = true;
    else {
        define_accept = define_operands(func);
        if(declare && define_accept && token->type == TOKEN_TYPE_DECLARE){
            
            // token
            // mov token->name, EAX

            get_and_set_token();

            allowed_eol(); // [ a := \n b] situation
            define_accept = count_operands(end_condition);
            // ЗАПИСЬ
            if(define_accept && !check_declare_logic(deep)){
                changeErrorCode(7);
                return false;
            } else if(define_accept){
                GEN_CREATE_LEFT_SIDE(deep);
        }

            // CHECKING TYPES FOR SYMTABLE
        } else if (equating && define_accept && token->type == TOKEN_TYPE_EQUATING){
            get_and_set_token();
            allowed_eol(); //[ a = \n b] situation
            define_accept = count_operands(end_condition);

            // C O M P A R E   T W O   L I S T S
            if(!check_define_logic(deep)){
                changeErrorCode(7);
                return false;
            }

            GEN_EQ_LEFT_SIDE(deep);


        } else if (func && define_accept && token->type == TOKEN_TYPE_LEFT_BRACKET){
            
            get_and_set_token();
            allowed_eol(); // func_name( \n args situation
            define_accept = expression_func_arguments();
            if (define_accept){
                number_of_operands--;
                freeBothCompareLists(); // L I S T   L O G I C
                get_and_set_token();
                define_accept = true;
            }
            GEN_CALL(saved_func_name);
        } else
            define_accept = false;
    }

    return define_accept;
}

bool define_operands(int func){
   
    bool operands_accept = false;

    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_UNDERSCORE || token->type == TOKEN_TYPE_COMMAND_FUNCTION){
        number_of_operands++;

        add_var_to_compare_list(token);
        GEN_ADD_VAR_TO_ASSEMBLY_STACK(token); // A S S E M B L Y

        // C H E C K   E X I S T I N G   F U N C T I O N   L O G I C
        saved_func_name = token;

        get_and_set_token();
        
        if(func && number_of_operands == 1 && token->type == TOKEN_TYPE_LEFT_BRACKET && (saved_func_name->type == TOKEN_TYPE_IDENTIFIER 
        || saved_func_name->type == TOKEN_TYPE_COMMAND_FUNCTION)){
            if(findVariableWithType(saved_func_name, deep, SymTable->var)){
                changeErrorCode(3);   //  ERROR 3, not found function_name
                return false;
            }
            // S Y M T A B L E    L O G I C
            if(findFunction(saved_func_name, SymTable->func))       
                operands_accept = true;
            else {
                operands_accept = false;
                changeErrorCode(6);   //  ERROR 3, not found function_name
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

    
    delete_expr_stack = true;
    expr = createStack();

    count_operands_accept = expression(current_end_condition);
    if(delete_expr_stack){
        deleteStack(&expr);
        delete_expr_stack = false;
    }

    if(WAS_CONDITION){  // WAS LOGICAL OPERATOR IN EXPRESSION
        changeErrorCode(5);
        return false;
    }
  

    if(count_operands_accept && number_of_operands > 0){
        get_and_set_token();
        count_operands_accept = count_operands(end_condition);
    } else if(count_operands_accept && number_of_operands == 0)
        count_operands_accept = true;
    else if (token->type == TOKEN_TYPE_COMMA && number_of_operands == 0){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_IDENTIFIER){
            count_operands_accept = false;
        }
    } else if(token->type == end_condition && number_of_operands > 0){
        count_operands_accept = false;
        changeErrorCode(7);
    }
    return count_operands_accept;
}


//  ------------------------------------ E X P R E S S I O N ------------------------------------

bool expression(int end_condition){
    bool expression_accept = false;
    static int can_be_function = 1;
    static int bracket = 0;
    static int closed_bracket_counter = 0;
    static int was_it_string = 0;



    
    if(token->type == TOKEN_TYPE_LEFT_BRACKET){
        
        push(expr, *token);
        bracket++;
        can_be_function = 0;
        get_and_set_token();
        expression_accept = expression(end_condition);

    } else if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT 
    || token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_LITERAL_STRING || token->type == TOKEN_TYPE_COMMAND_FUNCTION){
       
       if((token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_COMMAND_FUNCTION) 
       && !findFunction(token, SymTable->func) && (!SymTable->var || !findVariableWithType(token, deep, SymTable->var))){
            sort_to_postfix(expr, deep, SymTable->var);
            changeErrorCode(3);
            delete_expr_stack = false;
            return false;
       }

        if(token->type == TOKEN_TYPE_LITERAL_STRING /* or it was string id*/) // to control if that was string
            was_it_string = 1; 

        // C H E C K   E X I S T I N G   F U N C T I O N   L O G I C
        saved_func_name = token;
        get_and_set_token();
        if(token->type != TOKEN_TYPE_LEFT_BRACKET);
            push(expr, *saved_func_name);
    // CHECK CLOSED BRACKETS

        closed_bracket_counter = is_closed_bracket();
        if(closed_bracket_counter){
            bracket -= closed_bracket_counter;
        }


        if(token->type == end_condition){
            if((saved_func_name->type == TOKEN_TYPE_IDENTIFIER || saved_func_name->type == TOKEN_TYPE_COMMAND_FUNCTION) 
            && !findVariableWithType(saved_func_name, deep, SymTable->var)){
                delete_expr_stack = false;
                sort_to_postfix(expr, deep, SymTable->var);
                changeErrorCode(3); // variable not defined 
                return false;
            }

            expression_accept = true;
            was_it_string = 0;
            can_be_function = 1;

           
            delete_expr_stack = false;
            int result = sort_to_postfix(expr, deep, SymTable->var);
            if(result == -1){ // SEGFAULT FIX
                return false;
            }
            add_type_to_compare_list(result); // result

            if(result == 4){
                WAS_CONDITION = true;
            }

        } else if (token->type == TOKEN_TYPE_MATH_OPERATOR || token->type == TOKEN_TYPE_LOGICAL_OPERATOR){
            // ЗАКИНУТЬ В СТЕК 2 (saved_function_name)
            // ЗАКИНУТЬ В СТЕК 3 (token)

            if(saved_func_name->type == TOKEN_TYPE_IDENTIFIER || saved_func_name->type == TOKEN_TYPE_COMMAND_FUNCTION){
               if(!findVariableWithType(saved_func_name, deep, SymTable->var)){
                    delete_expr_stack = false;
                    sort_to_postfix(expr, deep, SymTable->var);
                    changeErrorCode(3); // variable not defined
                    return false;
                } else if (findVariableWithType(saved_func_name, deep, SymTable->var)->type == TOKEN_TYPE_STRING){
                    was_it_string = 1;
                }

            }

            if(was_it_string == 1){  // if used not '+' for string
                if(strcmp(token->data, "+")){

                    changeErrorCode(5);
                    delete_expr_stack = false;
                    sort_to_postfix(expr, deep, SymTable->var);

                    return false;
                }
            }
            push(expr, *token);
            get_and_set_token();
            can_be_function = 0;
            allowed_eol(); //[ a + \n b] situation
            expression_accept = expression(end_condition);
        } else if (token->type == TOKEN_TYPE_LEFT_BRACKET && can_be_function){
            // S Y M T A B L E    L O G I C
            if(findVariableWithType(saved_func_name, deep, SymTable->var)){
                changeErrorCode(3);   //  ERROR 3, not found function_name
                return false;
            }
            if(!findFunction(saved_func_name, SymTable->func)){
                changeErrorCode(6);   //  ERROR 3, not found function_name
                return false;
            }
            get_and_set_token();

            allowed_eol(); // func_name( \n args situation
            GEN_CREATE_FRAME_AND_SET_PARAMS(findFunction(saved_func_name, SymTable->func)->input_params); // A S S E M B L Y
            expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА
            GEN_CALL(saved_func_name);
            if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
                get_and_set_token();


                // OPERANDS ANALYSIS
                if(compareCompareLists() == 0){
                    number_of_operands = 0;
                    end_condition = TOKEN_TYPE_EOL;
                } else {
                    changeErrorCode(7);
                }
                // } else if (compareCompareLists() == -1){
                //    changeErrorCode(7);
                //     return false;
                // } else if (compareCompareLists() == 1){
                //     int tmp = number_of_operands;
                //     outputParams out_tmp = findFunction(saved_func_name, SymTable->func)->output_params;
                //     if(out_tmp == NULL){
                //         changeErrorCode(7); 
                //     } else {
                //         out_tmp = out_tmp->next;
                //         while(out_tmp != NULL){
                //             out_tmp = out_tmp->next;
                //             number_of_operands--;
                //         }
                //     }
                // }

                if(token->type == end_condition){
                    expression_accept = true;
                    can_be_function = 1;
                }
            }
        }
    }

    if(bracket != 0){
        
        if(delete_expr_stack){
            deleteStack(&expr);
            delete_expr_stack = false;
        }
        expression_accept = false;
    }
    if(end_condition == TOKEN_TYPE_COMMA && token->type == TOKEN_TYPE_EOL)
        changeErrorCode(7);
    else if(!expression_accept)
        changeErrorCode(2);
    return expression_accept;
}

// CLOSED_BRACKET_COUNTER
int is_closed_bracket(){
    int closed_bracket_counter = 0;
    while(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        // ЗАКИНУТЬ В СТЕК 4 (token)
        
        push(expr, *token);
        get_and_set_token();
        closed_bracket_counter++;
    }
    return closed_bracket_counter;
}

void allowed_eol(){
    while(token->type == TOKEN_TYPE_EOL) // func_name( \n args situation
        get_and_set_token();
}

//  ------------------------------------ E X P R E S S I O N    A R G U M E N T S ------------------------------------
bool expression_func_arguments(){

    bool func_arguments_accept = false;
    
    // F U N C T I O N   A R G U M E N T S   L O G I C
    function arg_find = findFunction(saved_func_name, SymTable->func);
    if(!arg_find) {
        changeErrorCode(3);  // not found function
        return false;
    }
    inputParams args_check = arg_find->input_params;
    outputParams args_output = arg_find->output_params;

    // ADDING OUTPUT ARGS TO TYPE LIST
            while(args_output != NULL){
                    add_type_to_compare_list(args_output->type);
                    args_output = args_output->next;
                }

    if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        // LOGIC
        
        if(args_check == NULL){ // if after all arguments next argument in sym_table will be null  (except print)
            func_arguments_accept = true;
            // записать аутпуты в лист 


        } else {
            changeErrorCode(6);
        }
    } else 
        func_arguments_accept = expression_func_single_argument(args_check, args_output);
    
    return func_arguments_accept;
}

bool expression_func_single_argument(inputParams args_check, outputParams args_output){
    bool func_single_argument = false;
    
    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_LITERAL_FLOAT 
    || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_LITERAL_STRING){

        if(args_check != NULL){
            // F U N C T I O N   A R G U M E N T S   L O G I C  --   C H E C K    L I T E R A L    T Y P E
            // S Y M T A B L E   F U N C T I O N  (returns  bool/int)
            
            if(args_check->type == 99){ // PRINT INPUT PARAMETERS TYPE
                args_check->next = args_check; // print cloning output argument
                if(token->type == TOKEN_TYPE_IDENTIFIER){
                    if(!findVariableWithType(token, deep, SymTable->var)){
                        args_check->next = NULL;
                        changeErrorCode(3); // not defined variable int printf
                        return false;
                    }
                }
                
            } else if(!compareTwoVariables(token, args_check->type, deep, SymTable->var)){
                changeErrorCode(6);
                return false;     
            } else {
                MOVE_INTO_INPUT_PARAMETER(args_check, token, deep);
            }
        }
        // ---------
        get_and_set_token();
        if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
            // L O G I C   C H E C K    [ args_check->next is NULL, it was the last parameter in SymTable->Func->InputParams ]
            if(args_check == NULL){
                changeErrorCode(6);
                return false;
            }
            
            if(args_check->type == 99)  // CONDITION FOR MAKE VALID PRINT WITH ANY QUANTITY OF PARAMETERS
                args_check->next = NULL;
            if(args_check->next == NULL) {
                func_single_argument = true;
            } else {
                changeErrorCode(6);
                func_single_argument = false;
            }
        } else if(token->type == TOKEN_TYPE_COMMA){
            get_and_set_token();
            allowed_eol(); //[ a (c, \n b)] situation
            func_single_argument = expression_func_single_argument(args_check->next, args_output);
        }
    }
        // args_check->next = NULL;
    return func_single_argument;
}

//  ------------------------------------ R E T U R N    C O N S T R U C T I O N ------------------------------------
// FINISHED HERE
bool return_construction(outputParams out_params){
    bool return_construction_accept = false;
    int return_end_condition = TOKEN_TYPE_EOL;

    if(out_params->next != NULL)
        return_end_condition = TOKEN_TYPE_COMMA;
    delete_expr_stack = true;
    expr = createStack();
    return_construction_accept = expression(return_end_condition);

    if(!return_construction_accept){
       changeErrorCode(6);
        return false;
    }
    
    // L O G I C :  C O M P A R E   T Y P E S   E X P R    A N D    O U T P U T    P A R A M
    if(out_params->type == typeCompareList->type){  // segfault pri funkci
        delete_type_from_compare_list();
    } else {
        changeErrorCode(6);
        return false;
    }

    if(token->type == TOKEN_TYPE_EOL && return_construction_accept){
        
        get_and_set_token();
        // compare two lists with type
        //compare_return_and_output_params_logic();
        freeBothCompareLists();
        return_construction_accept = function_body();
    } else if(token->type == TOKEN_TYPE_COMMA && return_construction_accept){
        get_and_set_token();
        if(out_params->next != NULL)
            return_construction_accept = return_construction(out_params->next);
        else {
            changeErrorCode(6);
        }
    } else if( token->type == TOKEN_TYPE_COMMA || TOKEN_TYPE_EOL){
        changeErrorCode(6);
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
    
    symTab_for_inbuilt_func(SymTable); // adding inbulid functions to symtable
    saved_func_name = saved_arg_name = saved_arg_type = NULL;
    GEN_START_MAIN();
    token = malloc (sizeof(Token));
    token->next = NULL;
    Token *second_run = token;
    if(get_token(token) == 1)
        changeErrorCode(1);
    
    if(error_flag == 0){
        bool result = program_start();
        if(!result && error_flag == 0){
            changeErrorCode(2);
        } 
        if(!wasMainInProgram)
            changeErrorCode(3);
    }
    PROGRAMM_RUN = SECOND_RUN;
    token = second_run;
    if(error_flag == 0){
        bool result = program_start();

        if(!result && error_flag == 0){
            changeErrorCode(2);
        } 
    }

    while(forStack != NULL)
        delete_from_for_stack();
    
    while(elseStack != NULL)
        delete_from_else_stack();
    
    if(delete_expr_stack == true)
        deleteStack(&expr);
    
    freeAllVariables(&(SymTable->var));
    freeBothCompareLists();
    GEN_DELETE_FULL_VAR_ASSEMBLY_STACK();
    freeFunctions(&(SymTable->func));
    dtor(startCommandFuncList);
    free(SymTable);
    dtor(second_run);
    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("                  [PROGRAMM RUN:%d] [ERROR CODE %d]\n", PROGRAMM_RUN, error_flag);
    printf("---------------------------------------------------------------------------------------------------------\n");
    return error_flag;
}