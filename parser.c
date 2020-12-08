/**
 * @file parser.c
 * 
 * @brief Parser and function main implementation
 * 
 * IFJ Projekt 2020, Tým 55
 * 
 * @author <xstepa64> Stepaniuk Roman, Bc.
*/

#include "parser.h"

// control if expression stack will be free in the end of program
bool delete_expr_stack = false;
// depth of label
int deep = -1;
// count of variables on the left part of expression, to compare the right side
int number_of_operands = 0;
// number of run of program
int PROGRAMM_RUN = FIRST_RUN;
// error detection. Program will return the error_flag.
int error_flag = 0;
// check if each function has own return in case of need
int return_in_function = true;
// check if main function was in program
bool wasMainInProgram = false;
// separation of mathematical expression and logic expression to compare expressions return types
bool WAS_CONDITION = false;
// run of function_body() in each function
bool repeatFunctionRun = false;
// number of depth where was 'if' to possible edition 'else' in each function
int if_else_counter = -1;
// number of depth where for to adding new depth label
int for_counter = -1;

/**
 * Function ckecks run of statement
 * If function returns true, IFJcode20 will generate
 * @return Function returns true if it's second run of statement, and false if it's first run of statement
 */
bool GET_REPEAT_FUNC_RUN(){
    return repeatFunctionRun;
}


/**
 * Function allocates new token, moves pointer from current token to new token and calls get_token()
 * Function calls changeErrorCode() if something goes wrong
 */
void get_and_set_token(){
        if(PROGRAMM_RUN == FIRST_RUN){
        token->next = malloc (sizeof(Token));
        if(token->next == NULL){
            changeErrorCode(99);
            return;
        }
        token = token->next;
        token->next = NULL;
        if (get_token(token) == 1)
            changeErrorCode(1);
    } else if (PROGRAMM_RUN == SECOND_RUN)
        token = token->next;
}

//  ------------------------------------ E R R O R    C O D E ------------------------------------

/**
 * @param code number of error
 * Function change error_flag if some kind of error was found
 */
void changeErrorCode(int code){
    if(error_flag == 0)
        error_flag = code;
}


//  ------------------------------------  S T A C K   C O N T A I N E R  ------------------------------------

/**
 * Function allocates container of stacks
 * @param myContainer pointer on container which contains pointers on all stacks
 * @return Function returns pointer on stackContainer
 */
stackContainer *declareContainer(stackContainer *myContainer){
    myContainer = malloc(sizeof(struct stackContainer));
    if(myContainer == NULL){
            changeErrorCode(99);
            return NULL;
    }
    myContainer->elseStack = NULL;
    myContainer->forStack = NULL;
    myContainer->endElseStack = NULL;
    myContainer->jumpIfStack = NULL;
    myContainer->jumpElseStack = NULL;
    myContainer->jumpForStack = NULL;
    return myContainer;
}


//  ------------------------------------  F O R  /  I F   S T A C K  ------------------------------------

/**
 * Function adds one instanse of some stack and allocates memory for it
 * @param changeStack pointer on some kind of stack
 * @param deep depth of label
 */
void add_to_for_if_stack(else_stack *changeStack, int deep){
    
    if (*changeStack == NULL){
        (*changeStack) = malloc(sizeof(struct elseStack));
        if(*changeStack == NULL){
            changeErrorCode(99);
            return;
        }
        (*changeStack)->deep = deep;
        (*changeStack)->next = NULL;
    } else {
        else_stack tmpforStack = *changeStack;
        *changeStack = malloc(sizeof(struct elseStack));
        if(*changeStack == NULL){
            changeErrorCode(99);
            return;
        }
        (*changeStack)->deep = deep;
        (*changeStack)->next = tmpforStack;
    }
}

/**
 * Function frees one instanse of some stack
 * @param changeStack pointer on some kind of stack
 */
void delete_from_for_if_stack(else_stack *changeStack){
    if(*changeStack != NULL){
        else_stack tmpforStack = (*changeStack)->next;
        free(*changeStack);
        *changeStack = tmpforStack;
    }
}

//  ------------------------------------ C O M P A R E    L I S T ------------------------------------

/**
 * Function adds variable to compare list and allocates memory for it
 * @param var Token contains name of variable
 */
void add_var_to_compare_list(Token *var){
    if(varCompareList == NULL){
        varCompareList = malloc(sizeof(variables_compare_list));
        if(varCompareList == NULL){
            changeErrorCode(99);
            return;
        }
        varCompareList->var = var;
        varCompareList->next = NULL;
    } else {
        variables_compare_list *tmpList = malloc(sizeof(variables_compare_list));
        if(tmpList == NULL){
            changeErrorCode(99);
            return;
        }
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

/**
 * Function frees last variable from compare list
 */
void delete_var_from_compare_list(){
    if(varCompareList != NULL){
        variables_compare_list *tmpList = varCompareList->next;
        free(varCompareList);
        varCompareList = tmpList;
    }
}

/**
 * Function adds type to compare list and allocates memory for it
 * @param type Token contains name of variable
 */
void add_type_to_compare_list(int type){
    if(typeCompareList == NULL){
        typeCompareList = malloc(sizeof(type_compare_list));
        if(typeCompareList == NULL){
            changeErrorCode(99);
            return;
        }
        typeCompareList->type = type;
        typeCompareList->next = NULL;
    } else {
        type_compare_list *tmpTypeList = malloc(sizeof(type_compare_list));
        if(tmpTypeList == NULL){
            changeErrorCode(99);
            return;
        }
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

/**
 * Function frees last type from compare list
 */
void delete_type_from_compare_list(){

    if(typeCompareList != NULL){
        type_compare_list *tmpList = typeCompareList->next;
        free(typeCompareList);
        typeCompareList = tmpList;
    }
}

/**
 * Function compares types of typeCompareList and varCompareList
 * @return Function returns 0 if both stacks are empty, 1 if varCompareList isn't empty, -1 if typeCompareList isn't empty
 */
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

/**
 * Function frees typeCompareList and varCompareList
 */
void freeBothCompareLists(){
    while(varCompareList != NULL || typeCompareList != NULL){
        if(typeCompareList != NULL)
        delete_type_from_compare_list();
        delete_var_from_compare_list();
    }
}

//  ------------------------------------ P R O G R A M    S T A R T ------------------------------------

/**
 * PROGRAM_SRART -> package main eol FUNCTION_CHECK eol eof
 */
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

/**
 * FUNCTION_CHECK -> func IDENTIFIER ( ALLOWED_EOL  INPUT_PARAMETERS  OUTPUT_PARAMETERS { eol FUNCTION_BODY } eol
 */
bool function_check(){
    bool func = false;
    if(token->type == TOKEN_TYPE_FUNC){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_IDENTIFIER){
            if(!strcmp(token->data, "main"))
                wasMainInProgram = true;
            // adding functions to symtable
            if(PROGRAMM_RUN){          
                insertFunction(token, &(SymTable->func));
            } else 
                GEN_START_OF_FUNCTION(token);
            current_function_name = token; // saved name of current function
            saved_func_name = token; // save function name for working with arguments
            get_and_set_token();
            if(token->type == TOKEN_TYPE_LEFT_BRACKET){
                get_and_set_token();
                allowed_eol();

                Token *repeat_function_run = NULL;
                Token *saved_func_name_new = saved_func_name;
                Token *saved_arg_name_new = saved_arg_name;
                Token *saved_arg_type_new = saved_arg_type;
                repeat_function_run = token;
                
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
                                else {
                                    func = function_body(); 

                                    repeatFunctionRun = true;
                                    if(func){
                                        saved_func_name = saved_func_name_new;
                                        saved_arg_name = saved_arg_name_new;
                                        saved_arg_type = saved_arg_type_new;
                                        token = repeat_function_run;
                                        input_parameters();
                                        get_and_set_token();
                                        output_parameters();
                                        get_and_set_token();
                                        get_and_set_token();
                                        freeGenVariables(&(SymTable->genVar), true);
                                        deep++;
                                        func = function_body();
                                        repeatFunctionRun = false;
                                    }
                                }
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

/**
 * INPUT_PARAMETERS -> )
 * INPUT_PARAMETERS -> INPUT_SINGLE_PARAMETERS
 */
bool input_parameters(){
    bool input_parameters = false;
    
    if(token->type == TOKEN_TYPE_RIGHT_BRACKET)
        input_parameters = true;
    else 
        input_parameters = input_single_parameters();
    return input_parameters;
}

/**
 * INPUT_SINGLE_PARAMETERS -> IDENTIFIER int/float/string , INPUT_SINGLE_PARAMETERS
 * INPUT_SINGLE_PARAMETERS -> IDENTIFIER int/float/string )
 */
bool input_single_parameters(){
    bool input_single_parameter = false;
    if(token->type == TOKEN_TYPE_IDENTIFIER){
        if(PROGRAMM_RUN){
            if(strcmp(saved_func_name->data, "main") == 0){      // if function main has input parameters
                changeErrorCode(6);
                return false; 
            }   
        }
        saved_arg_name = token; // foo(a int) -> saved_arg_name = a

        get_and_set_token();
        if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
            saved_arg_type = token; // // foo(a int) -> saved_arg_type = int
            if(PROGRAMM_RUN){        // add function arguments to SymTable
               addInputArguments(saved_func_name, saved_arg_name, saved_arg_type, SymTable->func);
            } else { // R U N 2,  creating arguments as zero-level vars
                insertVariable(saved_arg_name, deep+1, &(SymTable->var));  // var
                putTypeVariable(saved_arg_name, deep+1, saved_arg_type->type, SymTable->var);  // var_type
            }
            get_and_set_token();
            if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
                return true;
            }
            else if (token->type == TOKEN_TYPE_COMMA){
                get_and_set_token();
                allowed_eol();
                input_single_parameter = input_single_parameters();
            } else {
                input_single_parameter = false;
            }
        }
    } 

    return input_single_parameter;
}

//  ------------------------------------ O U T P U T    P A R A M E T E R S ------------------------------------

/**
 * OUTPUT_PARAMETERS -> {
 * OUTPUT_PARAMETERS -> ( )
 * OUTPUT_PARAMETERS -> ( OUTPUT_SINGLE_PARAMETERS
 */
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

/**
 * OUTPUT_SINGLE_PARAMETERS -> int/float/string , OUTPUT_SINGLE_PARAMETERS
 * OUTPUT_SINGLE_PARAMETERS -> int/float/string ) {
 */

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

/**
 * Function works only on first run of program. Ignore all types of tokens except '}'
 * Function gets next tokens until finds } which will be last in this function (depth will be the same as it was on start)
 */
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

/**
 * FUNCTION_BODY -> } eol  FUNCTION_BODY
 * FUNCTION_BODY -> { eol  FUNCTION_BODY
 * FUNCTION_BODY -> eol  FUNCTION_BODY
 * FUNCTION_BODY -> if IF_CONSTRUCTION  FUNCTION_BODY
 * FUNCTION_BODY -> else  {  eol  FUNCTION_BODY
 * FUNCTION_BODY -> for FOR_CONSTRUCTION  FUNCTION_BODY
 * FUNCTION_BODY -> IDENTIFIER  DEFINE_FUNC  FUNCTION_BODY
 * FUNCTION_BODY -> COMMAND_FUNCTION  DEFINE_FUNC  FUNCTION_BODY
 * FUNCTION_BODY -> _  DEFINE_FUNC  FUNCTION_BODY
 * FUNCTION_BODY -> return eol  FUNCTION_BODY
 * FUNCTION_BODY -> return  RETURN_CONSTRUCTION FUNCTION_BODY
 */
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

    // ignoring empty blocks   -> { /n  /n }, but not if{ /n }
    if(token->type != TOKEN_TYPE_END_BLOCK && token->type != TOKEN_TYPE_EOL){
        empty_block = false;
    }

    // working with else-stack
    if(token->type == TOKEN_TYPE_ELSE && Container->elseStack != NULL){
        if(Container->elseStack->deep == deep)
            else_condition_flag = true;

        add_to_for_if_stack(&(Container->jumpElseStack), Container->jumpIfStack->deep);
        // jump2
        GEN_JUMP(current_function_name, Container->jumpElseStack->deep, false, NO_FOR);
        // scope 1
        GEN_SCOPE(current_function_name, Container->jumpIfStack->deep, true, false);
        delete_from_for_if_stack(&(Container->jumpIfStack));

            delete_from_for_if_stack(&(Container->elseStack));
    } else if (Container->elseStack != NULL) {
        if(Container->elseStack->deep == deep && token->type != TOKEN_TYPE_END_BLOCK){
            delete_from_for_if_stack(&(Container->elseStack));
            // scope 1
            GEN_SCOPE(current_function_name, Container->jumpIfStack->deep, true, false);
            delete_from_for_if_stack(&(Container->jumpIfStack));
            
        } else if (Container->elseStack->deep > deep){
            delete_from_for_if_stack(&(Container->elseStack));
            // scope 1
            GEN_SCOPE(current_function_name, Container->jumpIfStack->deep, true, false);
            delete_from_for_if_stack(&(Container->jumpIfStack));
        }
    }

    // W O R K I N G    W I T H    F O R - S T A C K
    if(token->type == TOKEN_TYPE_END_BLOCK && Container->forStack != NULL){
        if(Container->forStack->deep == deep){
            delete_from_for_if_stack(&(Container->forStack));
            
            // jumpFor2
            GEN_JUMP(current_function_name, Container->jumpForStack->next->deep, false, FOR_JUMP);
            // scopeFor4
            GEN_SCOPE(current_function_name, Container->jumpForStack->next->next->next->deep, false, true);
            for(int i = 0; i < 4; i++)
                delete_from_for_if_stack(&(Container->jumpForStack));
            

            if(SymTable->var != NULL && deep == SymTable->var->deep)
                freeVariablesLastLabel(&(SymTable->var));
            deep--;
        }
    } 

    // W O R K I N G    W I T H    E N D   E L S E - S T A C K
    if(token->type == TOKEN_TYPE_END_BLOCK && Container->endElseStack != NULL){
        if(Container->endElseStack->deep == deep){
            delete_from_for_if_stack(&(Container->endElseStack));
            // navěšti 2
            GEN_SCOPE(current_function_name, Container->jumpElseStack->deep, false, false);
            delete_from_for_if_stack(&(Container->jumpElseStack));
        }
    } 

    // E N D    B L O C K   ( L A S T )
    if(token->type == TOKEN_TYPE_END_BLOCK && deep == -1){
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
        add_to_for_if_stack(&(Container->endElseStack), deep);
        function_accept = start_block_new_line();

    // I D E N T I F I E R    --   D E C L A R E,  E Q U A T I N G,  F U N C T I O N
    }  else if (token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_COMMAND_FUNCTION){
        
        if(define_func(TOKEN_TYPE_EOL, true, true, true)){
            get_and_set_token();
            function_accept = function_body();   
        }

    // U N D E R S C O R E  --  D E C L A R E,  E Q U A T I N G
    } else if (token->type == TOKEN_TYPE_UNDERSCORE){
        
        if(define_func(TOKEN_TYPE_EOL, true, true, false)){
            get_and_set_token();
            function_accept = function_body();   
        }

    // R E T U R N
    } else if (token->type == TOKEN_TYPE_RETURN){
        return_in_function = false;
        get_and_set_token();
        function F = findFunction(current_function_name, SymTable->func);
        outputParams out_params = F->output_params;      // compare return and output parameters

        // GEN_RETVAL_RETURN(out_params); // assembly

        if(token->type == TOKEN_TYPE_EOL && out_params == NULL){
            GEN_END_OF_FUNCTION(current_function_name);
            function_accept = function_body();
        } else if(out_params != NULL && token->type != TOKEN_TYPE_EOL){
            function_accept = return_construction(out_params);
        } else {
            changeErrorCode(6); // number of return args and function output args not the same
            return false;
        }
        
    }
    return function_accept;
}

//  ------------------------------------ F O R    C O N S T R U C T I O N ------------------------------------

/**
 * FOR_CONSTRUCTION ->  DEFINE_FUNC ; EXPRESSION ; DEFINE_FUNC { eol FUNCTION_BODY
 */
bool for_construction(){
    bool for_accept = false;
    deep++;

    if(token->type != TOKEN_TYPE_SEMICOLON){
        if(!define_func(TOKEN_TYPE_SEMICOLON, true, true, false)){
            return false;
        }
    }
    get_and_set_token();


    if(token->type != TOKEN_TYPE_SEMICOLON){

        // assembly
        for(int i = 0; i < 4; i++)    
            add_to_for_if_stack(&(Container->jumpForStack), ++for_counter);
        // scopeFor1
        GEN_SCOPE(current_function_name, Container->jumpForStack->deep, false, true);

        delete_expr_stack = true;
        expr = createStack();

        for_accept = expression(TOKEN_TYPE_SEMICOLON);
        
        if(token->type != TOKEN_TYPE_SEMICOLON && !for_accept){
            return false;
        }
    } else {
        changeErrorCode(2);
        return false;
    }
    
    get_and_set_token();
    if(!WAS_CONDITION && for_accept){
        changeErrorCode(5);
        return false;
    }
    for_accept = false;
    WAS_CONDITION = false;
    freeBothCompareLists();
    
    // jumpFor3
    GEN_JUMP(current_function_name, Container->jumpForStack->next->next->deep, false, FOR_JUMP_EQ);
    // jumpFor4
    GEN_JUMP(current_function_name, Container->jumpForStack->next->next->next->deep, false, FOR_JUMP);

    // scopeFor2
    GEN_SCOPE(current_function_name, Container->jumpForStack->next->deep, false, true);

    if(!define_func(TOKEN_TYPE_START_BLOCK, false, true, false)){
        return false;
    }
    // jumpFor1
    GEN_JUMP(current_function_name, Container->jumpForStack->deep, false, FOR_JUMP);
    if(token->type == TOKEN_TYPE_START_BLOCK){
        get_and_set_token();
        add_to_for_if_stack(&(Container->forStack), deep);
        deep++;
    // scopeFor3
    GEN_SCOPE(current_function_name, Container->jumpForStack->next->next->deep, false, true);                                 
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            for_accept = function_body();
        }
    }
    return for_accept;
}

//  ------------------------------------ I F    C O N S T R U C T I O N ------------------------------------

/**
 * IF_CONSTRUCTION -> EXPRESSION { eol FUNCTION_BODY
 */
bool if_construction()
{
    delete_expr_stack = true;
    expr = createStack();
    bool if_accept = false;
    if_accept = expression(TOKEN_TYPE_START_BLOCK);
    if(if_accept){
        if(!WAS_CONDITION ){
            changeErrorCode(5);
            return false;
        }
        add_to_for_if_stack(&(Container->elseStack), deep);
        // assembly
        add_to_for_if_stack(&(Container->jumpIfStack), ++if_else_counter);
        // jump1
        GEN_JUMP(current_function_name, Container->jumpIfStack->deep, true, NO_FOR);

        deep++;
    }
    if (if_accept && WAS_CONDITION){
        WAS_CONDITION = false;
        get_and_set_token();
        if_accept = false;
        freeBothCompareLists();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            if_accept = function_body();
        }
    }


    return if_accept;
}


//  ------------------------------------ D E C L A R E   &   E Q U A T I N G    E X P R E S S I O N ------------------------------------
//  ------------------------------------ O R    F U N C T I O N ()    F R O M    B O D Y 

/**
 * DEFINE_FUNC -> DEFINE_OPERANDS := COUNT_OPERANDS
 * DEFINE_FUNC -> DEFINE_OPERANDS = COUNT_OPERANDS
 * DEFINCE_FUNC -> DEFINE_OPERANDS ( EXPRESSION_FUNC_ARGUMENTS
 * 
 * @param end_condition anticipation end condition of the function. If in result current token->type will be the same as end_condition,
 * function will return true, else function will return false.
 * @param declare is true when declaration is allowed :=
 * @param equating is true when define is allowed =
 * @param func is true when function is allowed
 */
bool define_func(int end_condition, bool declare, bool equating, bool func){
    bool define_accept = false;
    number_of_operands = 0;

    if(token->type == end_condition){
        define_accept = true;
    } else {
        define_accept = define_operands(func);
        if(declare && define_accept && token->type == TOKEN_TYPE_DECLARE){

            get_and_set_token();

            allowed_eol(); // [ a := \n b] situation
            define_accept = count_operands(end_condition);
            if(define_accept && !check_declare_logic(deep)){
                changeErrorCode(7);
                return false;
            } else if(define_accept){
                GEN_CREATE_LEFT_SIDE(deep);
        }

            // checking types for SymTable
        } else if (equating && define_accept && token->type == TOKEN_TYPE_EQUATING){
            get_and_set_token();
            allowed_eol(); //[ a = \n b] situation
            define_accept = count_operands(end_condition);

            // compare two lists
            if(!check_define_logic(deep)){
                changeErrorCode(5);
                return false;
            }

            GEN_EQ_LEFT_SIDE(deep);


        } else if (func && define_accept && token->type == TOKEN_TYPE_LEFT_BRACKET){
            GEN_DELETE_FULL_VAR_ASSEMBLY_STACK();
            get_and_set_token();
            allowed_eol(); // func_name( \n args situation
            if(strcmp(saved_func_name->data, "print"))
                GEN_CREATE_FRAME_AND_SET_PARAMS(findFunction(saved_func_name, SymTable->func)->input_params); // assembly

            define_accept = expression_func_arguments();
            if (define_accept){
                number_of_operands--;
                freeBothCompareLists(); // list logic : deleting lists
                get_and_set_token();
                define_accept = true;
            }
            if(strcmp(saved_func_name->data, "print"))
                GEN_CALL(saved_func_name);
        } else
            define_accept = false;
    }

    return define_accept;
}

/**
 * DEFINE_OPERANDS -> IDENTIFIER
 * DEFINE_OPERANDS -> IDENTIFIER , DEFINDE_OPERANDS
 * DEFINE_OPERANDS -> COMMAND_FUNCTION
 * DEFINE_OPERANDS -> COMMAND_FUNCTION , DEFINE_OPERANDS
 * DEFINE_OPERANDS -> _
 * DEFINE_OPERANDS -> _ , DEFINE_OPERANDS
 * 
 * @param func is true when function is allowed
 */
bool define_operands(int func){
   
    bool operands_accept = false;

    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_UNDERSCORE || token->type == TOKEN_TYPE_COMMAND_FUNCTION){
        number_of_operands++;

        add_var_to_compare_list(token);
        GEN_ADD_VAR_TO_ASSEMBLY_STACK(token); // assembly

        // check existing function logic
        saved_func_name = token;
        get_and_set_token();
        
        if(func && number_of_operands == 1 && token->type == TOKEN_TYPE_LEFT_BRACKET && (saved_func_name->type == TOKEN_TYPE_IDENTIFIER 
        || saved_func_name->type == TOKEN_TYPE_COMMAND_FUNCTION)){
            if(findVariableWithType(saved_func_name, deep, SymTable->var)){
                changeErrorCode(3);   //  ERROR 3, not found function_name
                return false;
            }
            // SymTable logic
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


/**
 * COUNT_OPERANDS -> EXPRESSION
 * COUNT_OPERANDS -> EXPRESSION , COUNT_OPERANDS
 * 
 * function for recursive calling expressions + count of operands
 * @param end_condition anticipation end condition of the function. If in result current token->type will be the same as end_condition,
 * function will return true, else function will return false.
 */
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

    if(WAS_CONDITION){  // if expression had logical operators
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

/**
 * EXPRESSION -> (  ALLOWED_EOL  EXPRESSION
 * EXPRESSION -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  IS_CLOSED_BRACKET
 * EXPRESSION -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  IS_CLOSED_BRACKET  OPERATOR  ALLOWED_EOL  EXPRESSION
 * EXPRESSION -> IDENTIFIER/COMMAND_FUNCTION  IS_CLOSED_BRACKET  (  ALLOWED_EOL  EXPRESSION_FUNC_ARGUMENTS
 * 
 * Function parses syntax of expressions
 * @param end_condition anticipation end condition of the function. If in result current token->type will be the same as end_condition,
 * function will return true, else function will return false.
 */
bool expression(int end_condition){
    bool expression_accept = false;
    static int can_be_function = 1;
    static int bracket = 0;
    static int closed_bracket_counter = 0;
    static int was_it_string = 0;
    static bool normal_quantity_of_expressions = true;

    if(token->type == TOKEN_TYPE_LEFT_BRACKET){
        push(expr, *token);
        bracket++;
        can_be_function = 0;
        get_and_set_token();
        allowed_eol();
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

        // check existing function logic
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
            if (!normal_quantity_of_expressions){
                delete_expr_stack = false;
                sort_to_postfix(expr, deep, SymTable->var);
                changeErrorCode(7);
                return false;
            }
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
            if(result == -1){ // segfault fix, check if expression_tranlator goes fine
                return false;
            }
            add_type_to_compare_list(result); // result

            if(result == 4){
                WAS_CONDITION = true;
            }

        } else if (token->type == TOKEN_TYPE_MATH_OPERATOR || token->type == TOKEN_TYPE_LOGICAL_OPERATOR){
            
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
                if(strcmp(token->data, "+") && token->type != TOKEN_TYPE_LOGICAL_OPERATOR){

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
            // SymTable logic
            if(findVariableWithType(saved_func_name, deep, SymTable->var)){
                changeErrorCode(3);   //  ERROR 3, not found current_function_name
                return false;
            }
            if(!findFunction(saved_func_name, SymTable->func)){
                changeErrorCode(6);   //  ERROR 6, not found current_function_name
                return false;
            }
            get_and_set_token();

            allowed_eol(); // func_name( \n args situation
            if(strcmp(saved_func_name->data, "print"))
                GEN_CREATE_FRAME_AND_SET_PARAMS(findFunction(saved_func_name, SymTable->func)->input_params); // assembly
            expression_accept = expression_func_arguments(); 
            if(strcmp(saved_func_name->data, "print"))
                GEN_CALL(saved_func_name);
            if(expression_accept){                            
                get_and_set_token();


                // operands anylysis
                if(compareCompareLists() == 0){
                    number_of_operands = 0;
                    end_condition = TOKEN_TYPE_EOL;
                } else {
                    changeErrorCode(7);
                }

                if(token->type == end_condition){
                    expression_accept = true;
                    can_be_function = 1;
                }
            }
        } else if(token->type == TOKEN_TYPE_COMMA){
            normal_quantity_of_expressions = false;
            get_and_set_token();
            expression_accept = expression(TOKEN_TYPE_EOL);
        }
    } else if (token->type == TOKEN_TYPE_UNDERSCORE){
        changeErrorCode(3);
        return false;
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

/**
 * IS_CLOSED_BRACKET -> )  IS_CLOSED_BRACKET
 * IS_CLOSED_BRACKET -> eps
 * 
 * Function counts closed brackets
 * @return counter of closed brackets
 */
int is_closed_bracket(){
    int closed_bracket_counter = 0;
    while(token->type == TOKEN_TYPE_RIGHT_BRACKET){   
        push(expr, *token);
        get_and_set_token();
        closed_bracket_counter++;
    }
    return closed_bracket_counter;
}

/**
 * ALLOWED_EOL -> eol  ALLOWED_EOL
 * ALLOWED_EOL -> eps
 */
void allowed_eol(){
    while(token->type == TOKEN_TYPE_EOL) // func_name( \n args situation
        get_and_set_token();
}

//  ------------------------------------ E X P R E S S I O N    A R G U M E N T S ------------------------------------
/**
 * EXPRESSION_FUNC_ARGUMENTS -> )
 * EXPRESSION_FUNC_ARGUMENTS -> EXPRESSION_FUNC_SINGLE_ARGUMENT
 */
bool expression_func_arguments(){

    bool func_arguments_accept = false;
    
    // function arguments logic
    function arg_find = findFunction(saved_func_name, SymTable->func);
    if(!arg_find) {
        changeErrorCode(3);  // not found function
        return false;
    }

    inputParams args_check = arg_find->input_params;
    outputParams args_output = arg_find->output_params;

    // adding output arguments to typeList
    while(args_output != NULL){
        add_type_to_compare_list(args_output->type);
        args_output = args_output->next;
    }

    if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        // function arguments logic
        if(args_check == NULL || !strcmp(arg_find->name, "print")){ // if after all arguments next argument in sym_table will be null  (except print)
            func_arguments_accept = true;
        } else {
            changeErrorCode(6);
        }
    } else 
        func_arguments_accept = expression_func_single_argument(args_check, args_output);
    
    return func_arguments_accept;
}


/**
 * EXPRESSION_FUNC_SINGLE_ARGUMENT -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  )
 * EXPRESSION_FUNC_SINGLE_ARGUMENT -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  ,  EXPRESSION_FUNC_SINGLE_ARGUMENT
 * 
 * @param arg_check pointer on list of input function arguments (SymTable)
 * @param args_output pointer on list of output function arguments (SymTable)
 */
bool expression_func_single_argument(inputParams args_check, outputParams args_output){
    bool func_single_argument = false;
    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_LITERAL_FLOAT 
    || token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_LITERAL_STRING /* added*/ || token->type == TOKEN_TYPE_COMMAND_FUNCTION){

        if(args_check != NULL){
            // function arguments logic: check literal type
            // (returns  bool/int)
            if(args_check->type == TOKEN_TYPE_PRINT){ // PRINT INPUT PARAMETERS TYPE

                GEN_PRINT_WRITE(token, deep);
                args_check->next = args_check; // print cloning output argument
                if(token->type == TOKEN_TYPE_IDENTIFIER){
                    if(!findVariableWithType(token, deep, SymTable->var)){
                        args_check->next = NULL;
                        changeErrorCode(3); // not defined variable int 
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
        get_and_set_token();
        if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
            // logic check :  [ args_check->next is NULL, it was the last parameter in SymTable->Func->InputParams ]
            if(args_check == NULL){
                changeErrorCode(6);
                return false;
            }
            
            if(args_check->type == TOKEN_TYPE_PRINT)  // condition for making valid function PRINT with any quantity of parameters
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
    } else if (token->type == TOKEN_TYPE_UNDERSCORE){
        changeErrorCode(3);
        return false;
    }
    return func_single_argument;
}

//  ------------------------------------ R E T U R N    C O N S T R U C T I O N ------------------------------------

/**
 * RETURN_CONSTRUCTION -> eol
 * RETURN_CONSTRUCTION -> EXPRESSION  eol
 * RETURN_CONSTRUCTION -> EXPRESSION  ,  RETURN_CONSTRUCTION
 * 
 * @param out_params pointer on list of output function arguments (SymTable)
 */
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
    
    // logic : compare types expr and output parameters
    if(out_params->type == typeCompareList->type){  
        delete_type_from_compare_list();
    } else {
        changeErrorCode(6);
        return false;
    }

    if(token->type == TOKEN_TYPE_EOL && return_construction_accept){
        
        get_and_set_token();
        // compare two lists with type
        freeBothCompareLists();
        GEN_END_OF_FUNCTION(current_function_name);
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

/**
 * START_BLOCK_NEW_LINE -> { eol
 */
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
    SymTable = declaration(SymTable);
    Container = declareContainer(Container);
    
    symTab_for_inbuilt_func(SymTable); // adding inbulid functions to symtable
    saved_func_name = saved_arg_name = saved_arg_type = NULL;
    GEN_START_MAIN();
    GEN_CALL_INBUILDS();

    token = malloc (sizeof(Token));
    if(token == NULL){
            changeErrorCode(99);
    } else {
        token->next = NULL;
    }
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

    while(Container->forStack != NULL)
        delete_from_for_if_stack(&(Container->forStack));
    
    while(Container->elseStack != NULL)
        delete_from_for_if_stack(&(Container->elseStack));
    
    while(Container->endElseStack != NULL)
        delete_from_for_if_stack(&(Container->endElseStack));

    while(Container->jumpIfStack != NULL)
        delete_from_for_if_stack(&(Container->jumpIfStack));
    
    while(Container->jumpForStack != NULL)
        delete_from_for_if_stack(&(Container->jumpForStack));
    
    while(Container->jumpElseStack != NULL)
        delete_from_for_if_stack(&(Container->jumpElseStack));    

    printf("LABEL $END_OF_PROGRAM\n");
    if(delete_expr_stack == true)
        deleteStack(&expr);
    
    freeAllVariables(&(SymTable->var));
    freeGenVariables(&(SymTable)->genVar, false);
    freeBothCompareLists();
    GEN_DELETE_FULL_VAR_ASSEMBLY_STACK();
    freeFunctions(&(SymTable->func));
    dtor(startCommandFuncList);
    free(Container);
    free(SymTable);
    dtor(second_run);
    return error_flag;
}