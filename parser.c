#include "parser.h"

int token_counter = 1;  // delete later
int deep = -1;
int number_of_operands = 0;
int PROGRAMM_RUN = FIRST_RUN;
int error_flag = 0;
// закинуть в боди аутпут аргументы функшн нейма и сравнивать аргумент с типом экспрешна через лист и перекидывать в ретурн_констракшн(аргумент->некст)
int return_in_function = true;
bool wasMainInProgram = false;

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
    printf("\n");
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

//  ------------------------------------ C O M P A R E    L I S T ------------------------------------

void add_var_to_compare_list(Token *var){
    //printf("C O M P A R E   L I S T   1\n");
    //variable newVariable = findVariable(var, deep, SymTable->var);
    //printf("C O M P A R E   L I S T   2\n");
    if(varCompareList == NULL){
        varCompareList = malloc(sizeof(variables_compare_list));
        varCompareList->var = var;
        varCompareList->next = NULL;
      //  printf("C O M P A R E   L I S T   3\n");
    } else {
       // printf("C O M P A R E   L I S T   else\n");
        variables_compare_list *tmpList = malloc(sizeof(variables_compare_list));
        tmpList->var = var;
        tmpList->next = NULL;
        variables_compare_list *cycleList = varCompareList;
        while(cycleList->next != NULL){
            cycleList = cycleList->next;
        }
        cycleList->next = tmpList;
    }
    //printf("C O M P A R E   L I S T   4\n");
    variables_compare_list *tmp = varCompareList;
    //printf("C O M P A R E   L I S T   5\n");
    //printf("\n\n\n");
    while(tmp != NULL){
        //printf("tmp %s -> ", tmp->var->data);
        tmp = tmp->next;
    }
    printf("\n\n\n");
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
    printf("C O M P A R E   T Y P E   5\n");
    printf("\n\n\n");
    while(tmp != NULL){
        printf("tmp %d -> ", tmp->type);
        tmp = tmp->next;
    }
    printf("\n\n\n");
}

void delete_type_from_compare_list(){

    if(typeCompareList != NULL){
            printf("-------------------<HERE\n");
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
        printf("---------compareCompare %s %d", tmpVar->var->data, tmpType->type);
        tmpVar = tmpVar->next;
        tmpType = tmpType->next;
    }
    if(tmpVar == NULL && tmpType == NULL){
        return 0;
    } else if (tmpVar != NULL){
        return 1;
    } else {
        printf("---------compareCompare %d", tmpType->type);
        return -1;
    }
}

// bool checkOneCompareList(type_compare_list *typeList){
//     if (typeList != NULL){
//         changeErrorCode(3);
//         return false;
//     } else 
//         return true;
// }

// void freeOneCompareList(type_compare_list *typeList){
//     while(typeList != NULL){
//         delete_type_from_compare_list(typeList);
//     }
// }

void freeBothCompareLists(){
    while(varCompareList != NULL || typeCompareList != NULL){
        if(typeCompareList != NULL)
            printf("-------------------> %d\n", typeCompareList->type);
        delete_type_from_compare_list();
        delete_var_from_compare_list();
    }
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
        } else if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_COMMAND_FUNCTION)
            changeErrorCode(7);
    }
    if(program_start == false)
        printf("ERROR IN FIRST LINE OR THERE ARE NO (VALID) FUNCTIONS IN YOUR PROGRAM!\n");
    if(!wasMainInProgram)
        changeErrorCode(3);
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
                Print_var(SymTable->var);
            }
            current_function_name = token; // saved name of current function
            saved_func_name = token; // save function name for working with arguments
            get_and_set_token();
            if(token->type == TOKEN_TYPE_LEFT_BRACKET){
                get_and_set_token();
                func = input_parameters();
                if(func){
                    get_and_set_token();
                    //printf("HAI HAI_______________\n");
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
            if(strcmp(saved_func_name->data, "main") == 0){      // I F    M A I N   H A S   I N P U T   P A R A M S
                changeErrorCode(6);
                printf("    ------------------------------->  PARAMETERS IN MAIN\n");
                return false;  // error_flag = 4to-to
            }   
        }
        saved_arg_name = token; // foo(a int) -> saved_arg_name = a

        get_and_set_token();
        if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
            saved_arg_type = token; // // foo(a int) -> saved_arg_type = int
            if(PROGRAMM_RUN){        // A D D   F U N C   A R G S    T O    S Y M T A B L E
                printf("INPUTS PARAMS -%s -%s -%s  strom->%s\n", saved_func_name->data, saved_arg_name->data, saved_arg_type->data, SymTable->func->name);
                addInputArguments(saved_func_name, saved_arg_name, saved_arg_type, SymTable->func);
            } else { // R U N 2,   C R E A T I N G    A R G U M E N T S   A S    Z E R O    L E V E L     V A R S
                insertVariable(saved_arg_name, deep+1, &(SymTable->var));  // var
                putTypeVariable(saved_arg_name, deep+1, saved_arg_type->type, SymTable->var);  // var_type
            }
            // input_single_parameter = true;
            get_and_set_token();
            if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
                printf("PRINTVAR2\n");
                Print_var(SymTable->var);        // P R I N T    T R E E
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
        printf("____HAI_HAI_HAI_________\n");
        output_parameters = output_single_parameters();
    }
    return output_parameters;
}

bool output_single_parameters(){
    bool output_single_parameter = false;
    if(PROGRAMM_RUN){
            if(strcmp(saved_func_name->data, "main") == 0){      // I F    M A I N   H A S   I N P U T   P A R A M S
                changeErrorCode(6);
                printf("    ------------------------------->  PARAMETERS IN MAIN\n");
                return false;  // error_flag = 4to-to
            }   
        }
    if(token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_FLOAT || token->type == TOKEN_TYPE_STRING){
        
        if(PROGRAMM_RUN){
            saved_arg_type = token;
            
            //printf("OUTPUTS PARAMS -%s -%s -%s  -%s\n", saved_func_name->data, saved_arg_name->data, saved_arg_type->data, SymTable->func->name);
            addOutputArguments(saved_func_name, saved_arg_type, SymTable->func);
            printf("____HAI_HAI_HAI_________2\n");
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
        if(deep == -1 || error_flag == 1)
            break;
        //printf("TOKEN VZYAL_________________\n");
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



    // W O R K I N G    W I T H    F O R - S T A C K
    if(token->type == TOKEN_TYPE_END_BLOCK && forStack != NULL){
        if(forStack->deep == deep){
            delete_from_for_stack();
            freeVariablesLastLabel(&(SymTable->var));
            deep--;
        }
    } 


    

    // E N D    B L O C K   ( L A S T )
    if(token->type == TOKEN_TYPE_END_BLOCK && deep == -1) {
        function_accept = true;
    if(findFunction(current_function_name, SymTable->func)->output_params != NULL && return_in_function == true){ // was not return command
            printf("---------------------------------> was not return \n");
            changeErrorCode(6); // number of return args and function output args not the same
            return false;
    }
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

        if(token->type == TOKEN_TYPE_EOL && out_params == NULL)
            function_accept = function_body();
        else if(out_params != NULL && token->type != TOKEN_TYPE_EOL)
            function_accept = return_construction(out_params);
        else {
            printf("---------------------------------> number of return args and function output args not the same \n");
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
    add_to_for_stack();
    //printf("            HI FROM FOR_CONSTRUCTION %s\n", token->data);
    if(!define_func(TOKEN_TYPE_SEMICOLON, 1, 1, false)){
            //printf("            here 1 token - %s\n", token->data);
        return false;
    }
    get_and_set_token();
    if(!logic_expression(TOKEN_TYPE_SEMICOLON)){
        //printf("            here 2\n");
        return false;
    }
    //printf("SREEEEEDINA    IIIIF\n");
    get_and_set_token();
    if(!define_func(TOKEN_TYPE_START_BLOCK, 0, 1, false)){
        printf("            here 3\n");
        return false;
    }
    if(token->type == TOKEN_TYPE_START_BLOCK){
        get_and_set_token();
        deep++;                                 
        if(token->type == TOKEN_TYPE_EOL){
            
    //
    // //      P R I N T
    // variable TMPF = SymTable->var;
    // for(int i = SymTable->var->deep; i>=0; i--){
    //     printf("symt_var_deep = %d\n", SymTable->var->deep);
    //     printf("PRINTVAR3\n");
    //     Print_var(TMPF);
    //     TMPF = TMPF->prevTree;
    // }

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
        expr = createStack();
        logic_expression = expression(TOKEN_TYPE_LOGICAL_OPERATOR); //left side + operator
        if(logic_expression){
            get_and_set_token();
            expr = createStack();
            logic_expression = expression(end_condition); //right side + semicolon (for)
        }

        freeBothCompareLists(); // free the logic list

    }
    return logic_expression;
}




//  ------------------------------------ D E C L A R E   &   E Q U A T I N G    E X P R E S S I O N ------------------------------------
//  ------------------------------------ O R    F U N C T I O N ()    F R O M    B O D Y 

bool define_func(int end_condition, int declare, int equating, bool func){
    bool define_accept = false;
//printf("                            IIIIIIIN DEEEFINE  FUNC %s\n", token->data);
    if(token->type == end_condition)
        define_accept = true;
    else {
        
        //printf("                            DEFINE1 returns token [%d] %s\n", end_condition, token->data);
        define_accept = define_operands(func);
        //printf("                            DEFINE2 returns token %s, %d\n", token->data, define_accept);
        if(declare && define_accept && token->type == TOKEN_TYPE_DECLARE){
            
            // token
            // mov token->name, EAX

            get_and_set_token();

            allowed_eol(); // [ a := \n b] situation
            //printf("                            DEFINE3 returns token %s\n", token->data);
            define_accept = count_operands(end_condition);

            // ЗАПИСЬ
            if(!check_declare_logic(deep)){
                printf("             --------------------------------------------------->  WRITE TO SYMPABLE  :=\n");
                changeErrorCode(6);
                return false;
            }

            // CHECKING TYPES FOR SYMTABLE
        } else if (equating && define_accept && token->type == TOKEN_TYPE_EQUATING){
                        printf("                            DEFINE4 (EQUATING) returns token %s\n", token->data);
            get_and_set_token();
            allowed_eol(); //[ a = \n b] situation
            define_accept = count_operands(end_condition);





            // JUST PRINTF
            
            printf("---------------------------------------------------------------------------------------\n");
            type_compare_list *tList = typeCompareList;
            while(tList != NULL){
                printf("TYPELIST %d\n", tList->type);
                tList = tList->next;
            }
            printf("---------------------------------------------------------------------------------------\n");





            // C O M P A R E   T W O   L I S T S
            if(!check_define_logic(deep)){
                printf("             --------------------------------------------------->  COMPARE LISTS  =\n");
                changeErrorCode(6);
                return false;
            }

        } else if (func && define_accept && token->type == TOKEN_TYPE_LEFT_BRACKET){
            
            get_and_set_token();
            allowed_eol(); // func_name( \n args situation
            define_accept = expression_func_arguments();
            if (define_accept){
                number_of_operands--;
                printf("                            DEFINE5 (FUNCTION) returns token %s\n", token->data);

                freeBothCompareLists(); // L I S T   L O G I C
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

    if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_UNDERSCORE || token->type == TOKEN_TYPE_COMMAND_FUNCTION){
        number_of_operands++;

        printf("______________________________NEW STOP\n");
        add_var_to_compare_list(token);

        // C H E C K   E X I S T I N G   F U N C T I O N   L O G I C
        saved_func_name = token;
        printf("define ops   token->data = %s, num_of_op %d\n", token->data, number_of_operands);

        get_and_set_token();
        printf("define ops   token->next->data = %s\n", token->data);
        
        if(func && number_of_operands == 1 && token->type == TOKEN_TYPE_LEFT_BRACKET && (saved_func_name->type == TOKEN_TYPE_IDENTIFIER 
        || saved_func_name->type == TOKEN_TYPE_COMMAND_FUNCTION)){
            printf("iiiiim heeeereee________________________ %s \n", saved_func_name->data);
            if(findVariableWithType(saved_func_name, deep, SymTable->var)){
                printf("             --------------------------------------------------->  function has the same name as var\n");
                changeErrorCode(3);   //  ERROR 3, not found function_name
                return false;
            }
            // S Y M T A B L E    L O G I C
            if(findFunction(saved_func_name, SymTable->func))       
                operands_accept = true;
            else {
                operands_accept = false;
                printf("             --------------------------------------------------->  NOT FOUND FUNCTION NAME  =\n");
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

    expr = createStack();
    count_operands_accept = expression(current_end_condition);
    printf("          count operands      ZASEL #%d   token [%s], counter [%d], accept [%d] \n", token_counter, token->data, number_of_operands, count_operands_accept);





    ///     F I N I S H E D     T A D Y




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
        printf("             --------------------------------------------------->  operands > exprs  =\n");
    }
  

    return count_operands_accept;
}


//  ------------------------------------ E X P R E S S I O N ------------------------------------

// bool expression_including_string(int end_condition){   // MAYBE WORKS
//     bool including_string_accept = false;
//     if(token->type == TOKEN_TYPE_LITERAL_STRING){   
//         get_and_set_token();
//         if(token->type == end_condition)
//             including_string_accept = true;
//     } else
//         including_string_accept = expression(end_condition);
    

//     return including_string_accept;
// }

bool expression(int end_condition){
    bool expression_accept = false;
    static int can_be_function = 1;
    static int bracket = 0;
    static int closed_bracket_counter = 0;
    static int was_it_string = 0;
    if(token->type == TOKEN_TYPE_LEFT_BRACKET){
        // ЗАКИНУТЬ В СТЕК 1
        push(expr, *token);
        bracket++;
        can_be_function = 0;
        get_and_set_token();
        expression_accept = expression(end_condition);

    } else if(token->type == TOKEN_TYPE_LITERAL_FLOAT || token->type == TOKEN_TYPE_LITERAL_INT 
    || token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_LITERAL_STRING){

        if(token->type == TOKEN_TYPE_LITERAL_STRING /* or it was string id*/) // to control if that was string
            was_it_string = 1;

        // C H E C K   E X I S T I N G   F U N C T I O N   L O G I C
        saved_func_name = token;
        push(expr, *token);
        get_and_set_token();
    // CHECK CLOSED BRACKETS

        closed_bracket_counter = is_closed_bracket();
        if(closed_bracket_counter){
            bracket -= closed_bracket_counter;
        }
    // --
        // if(end_condition == RETURN_TYPE){      // ONLY FOR RETURN
        //     if(token->type == TOKEN_TYPE_COMMA || token->type == TOKEN_TYPE_EOL){
        //         expression_accept = true;
        //         was_it_string = 0;
        //         can_be_function = 1;
        //         add_type_to_compare_list(typeCompareList, 1);
        //     }
        // }


        if(token->type == end_condition){
            if((saved_func_name->type == TOKEN_TYPE_IDENTIFIER || saved_func_name->type == TOKEN_TYPE_COMMAND_FUNCTION) 
            && !findVariableWithType(saved_func_name, deep, SymTable->var)){
                printf("                 --------------------> UNDEFINED VARIABLE\n");
                changeErrorCode(3); // variable not defined 
                return false;
            }

            expression_accept = true;
            was_it_string = 0;
            can_be_function = 1;

            for (int i = 0; i < expr->size; i++) {
        } printf("\n ");

            int result = sort_to_postfix(expr, deep, SymTable->var);
            printf("\n type of result is %d\n", result);
            add_type_to_compare_list(1); // result


        } else if (token->type == TOKEN_TYPE_MATH_OPERATOR){
            // ЗАКИНУТЬ В СТЕК 2 (saved_function_name)
            // ЗАКИНУТЬ В СТЕК 3 (token)
            push(expr, *token);
            if((saved_func_name->type == TOKEN_TYPE_IDENTIFIER || saved_func_name->type == TOKEN_TYPE_COMMAND_FUNCTION) 
            && !findVariableWithType(saved_func_name, deep, SymTable->var)){
                printf("                 --------------------> UNDEFINED VARIABLE\n");
                changeErrorCode(3); // variable not defined
                return false;
            }

            if(was_it_string == 1){  // if used not '+' for string
                if(strcmp(token->data, "+")){
                    printf("WAS NOT PLUS FOR STR\n");
                    return false;
                }
            }

            get_and_set_token();
            can_be_function = 0;
            allowed_eol(); //[ a + \n b] situation
            expression_accept = expression(end_condition);
        } else if (token->type == TOKEN_TYPE_LEFT_BRACKET && can_be_function){
            // S Y M T A B L E    L O G I C
            if(findVariableWithType(saved_func_name, deep, SymTable->var)){
                printf("             --------------------------------------------------->  function has the same name as var\n");
                changeErrorCode(3);   //  ERROR 3, not found function_name
                return false;
            }
            if(!findFunction(saved_func_name, SymTable->func)){
                printf("             --------------------------------------------------->  null arguments here, but more in symtable\n");
                changeErrorCode(6);   //  ERROR 3, not found function_name
                return false;
            }
            get_and_set_token();

            allowed_eol(); // func_name( \n args situation

            expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА  
            if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
                get_and_set_token();
                

                // OPERANDS ANALYSIS
                if(compareCompareLists() == 0){
                    number_of_operands = 0;
                    end_condition = TOKEN_TYPE_EOL;
                } else if (compareCompareLists() == -1){
                    printf("             --------------------------------------------------->  RUZNY POCET PARAMETRU LEVA STRANA != PRAVA STRANA\n");
                    changeErrorCode(7);
                    return false;
                } else if (compareCompareLists() == 1){
                    int tmp = number_of_operands;
                    outputParams out_tmp = findFunction(saved_func_name, SymTable->func)->output_params;
                    if(out_tmp == NULL){
                        changeErrorCode(7); 
                    printf("             --------------------------------------------------->  VOID FUNCTION IN EXPRESSION\n");
                    } else {
                        out_tmp = out_tmp->next;
                        while(out_tmp != NULL){
                            out_tmp = out_tmp->next;
                            number_of_operands--;
                        }
                    }
                }

                if(token->type == end_condition){
                    expression_accept = true;
                    can_be_function = 1;
                }
            }
        }
    }
    //     // C O M M A N D    F U N C T I O N S
    //  else if (token->type == TOKEN_TYPE_COMMAND_FUNCTION){
    //             saved_func_name = token;
    //     get_and_set_token();
    //     if (token->type == TOKEN_TYPE_LEFT_BRACKET){
    //         get_and_set_token();
    //         allowed_eol(); // func_name( \n args situation
    //         expression_accept = expression_func_arguments();  // ПОТОМ ПЕРЕДАВАТЬ СЮДА КОПИЮ УКАЗАТЕЛЬ НА ТОКЕН ИДЕНТИФИКАТОРА  
    //         if(expression_accept){                            // (ПЕРЕД ЭТИМ ЕГО СОХРАНИВ) И ОБНУЛИТЬ В КОНЦЕ ПАРАМЕТРОВ
    //             get_and_set_token();

    //             // OPERANDS ANALYSIS
    //             if(compareCompareLists() == 0){
    //                 number_of_operands = 0;
    //                 end_condition = TOKEN_TYPE_EOL;
    //             } else if (compareCompareLists() == -1){
    //                 printf("             --------------------------------------------------->  RUZNY POCET PARAMETRU LEVA STRANA != PRAVA STRANA\n");
    //                 changeErrorCode(7);
    //                 return false;
    //             } else if (compareCompareLists() == 1){
    //                 int tmp = number_of_operands;
    //                 outputParams out_tmp = findFunction(saved_func_name, SymTable->func)->output_params;
    //                 if(out_tmp == NULL){
    //                     changeErrorCode(7); 
    //                 printf("             --------------------------------------------------->  VOID FUNCTION IN EXPRESSION\n");
    //                 } else {
    //                     out_tmp = out_tmp->next;
    //                     while(out_tmp != NULL){
    //                         out_tmp = out_tmp->next;
    //                         number_of_operands--;
    //                     }
    //                 }
    //             }

    //             if(token->type == end_condition){
    //                 expression_accept = true;
    //             }
    //         }
    //     }
    // }


    if(bracket != 0){
        expression_accept = false;
    }
    return expression_accept;
}

// CLOSED_BRACKET_COUNTER
int is_closed_bracket(){
    int closed_bracket_counter = 0;
    while(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        // ЗАКИНУТЬ В СТЕК 4 (token)
        get_and_set_token();
        push(expr, *token);
        closed_bracket_counter++;
    }
    return closed_bracket_counter;
}

void allowed_eol(){
    if(token->type == TOKEN_TYPE_EOL) // func_name( \n args situation
        get_and_set_token();
}

//  ------------------------------------ E X P R E S S I O N    A R G U M E N T S ------------------------------------
// сейчас считаются внутренние аргументы функции, надо добавить аутпут аргументы
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
    printf("<--------------->  WRITING %s   <-------------->\n", arg_find->name);

    // ADDING OUTPUT ARGS TO TYPE LIST
            while(args_output != NULL){
                    printf("<--------------->  WRITING %d   <-------------->\n", args_output->type);
                    add_type_to_compare_list(args_output->type);
                    args_output = args_output->next;
                }

    if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
        // LOGIC
        
        if(args_check == NULL){ // if after all arguments next argument in sym_table will be null  (except print)
            func_arguments_accept = true;
            // записать аутпуты в лист 


        } else {
            printf("             --------------------------------------------------->  null arguments here, but more in symtable\n");
            changeErrorCode(6);
        }
    } else {
        printf("lololsfdofldso\n");
        func_arguments_accept = expression_func_single_argument(args_check, args_output);
        


    }
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
                        printf("                 -------------------------------> PRINT:: NOT DEFINED VAR IN SINGLE_ARGUMENT\n");
                        args_check->next = NULL;
                        changeErrorCode(3); // not defined variable int printf
                        return false;
                    }
                }
                
            } else if(!compareTwoVariables(token, args_check->type, deep, SymTable->var)){
                printf("                 -------------------------------> NOT COMPARABLE IN SINGLE_ARGUMENT\n");
                changeErrorCode(6);
                return false;     
            }
        }
        // ---------
        get_and_set_token();
        if(token->type == TOKEN_TYPE_RIGHT_BRACKET){
            // L O G I C   C H E C K    [ args_check->next is NULL, it was the last parameter in SymTable->Func->InputParams ]
            if(args_check->type == 99)  // CONDITION FOR MAKE VALID PRINT WITH ANY QUANTITY OF PARAMETERS
                args_check->next = NULL;
            if(args_check->next == NULL) {
                func_single_argument = true;
                            
                // while(args_output != NULL){
                //     printf("<--------------->  WRITING %d   <-------------->\n", args_output->type);
                //     add_type_to_compare_list(args_output->type);
                //     args_output = args_output->next;
                // }
            } else {
                printf("                 -------------------------------> WRONG QUANTITY OF ARGUMENTS IN SINGLE_ARGUMENT\n");
                changeErrorCode(6);
                func_single_argument = false;
            }
        } else if(token->type == TOKEN_TYPE_COMMA){
            get_and_set_token();
            allowed_eol(); //[ a (c, \n b)] situation
            func_single_argument = expression_func_single_argument(args_check->next, args_output);
        }
    }
        args_check->next = NULL;
    return func_single_argument;
}

//  ------------------------------------ R E T U R N    C O N S T R U C T I O N ------------------------------------
// FINISHED HERE
bool return_construction(outputParams out_params){
    bool return_construction_accept = false;
    int return_end_condition = TOKEN_TYPE_EOL;

    if(out_params->next != NULL)
        return_end_condition = TOKEN_TYPE_COMMA;
    expr = createStack();
    return_construction_accept = expression(return_end_condition);
    //printf("RETURN 123 %d\n", return_construction_accept)

    if(!return_construction_accept){
       changeErrorCode(6);
        printf("                 ------------------------------->  1!= QUANTITY OF OUTPUT PARAMS AND RETURN PARAMS\n");
        return false;
    }
    
    // L O G I C :  C O M P A R E   T Y P E S   E X P R    A N D    O U T P U T    P A R A M
    if(out_params->type == typeCompareList->type){  // segfault pri funkci
        printf("______here\n");
        delete_type_from_compare_list();
        printf("______here2\n");
    } else {
        printf("        ----------------------------------------->   DIFFERENT TYPES: EXPR AND OUTPUT PARAM\n");
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
        printf("______here3\n");
        get_and_set_token();
        if(out_params->next != NULL)
            return_construction_accept = return_construction(out_params->next);
        else {
            changeErrorCode(6);
        printf("                 ------------------------------->  != QUANTITY OF OUTPUT PARAMS AND RETURN PARAMS\n");
        }
    } else if( token->type == TOKEN_TYPE_COMMA || TOKEN_TYPE_EOL){
        changeErrorCode(6);
        printf("                 ------------------------------->  != QUANTITY OF OUTPUT PARAMS AND RETURN PARAMS\n");
    }
    printf("______here3\n");
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

    token = malloc (sizeof(Token));
    token->next = NULL;
    Token *second_run = token;
    if(get_token(token) == 1)
        changeErrorCode(1);
    
    if(error_flag == 0){
        bool result = program_start();
        if(!result && error_flag == 0){
            printf("\n\n                                                 PROGRAM FINISHED ERROR 2\n");
            changeErrorCode(2);
        } 
    }
     printf("\n\n                                                 PROGRAM FINISHED 1: [%d]\n", error_flag);
    PROGRAMM_RUN = SECOND_RUN;
    token = second_run;
    if(error_flag == 0){
        bool result = program_start();

        if(!result && error_flag == 0){
            printf("\n\n                                                 PROGRAM FINISHED ERROR 2\n");
            changeErrorCode(2);
        } 
    printf("\n\n                                            PROGRAM FINISHED 2: [ERROR CODE %d]\n", error_flag);
    }
    //printf("------- %d\n", SymTable->func->LPtr->LPtr->output_params->next->type);
    

    Print_func(SymTable->func);

    //Print_var(S->var);
    
    // int i = 0;
    // while(second_run != NULL){
    //     printf("[%d] data: %s   number: %d \n", ++i, second_run->data, second_run->type);
    //     second_run = second_run->next;
    // }
    freeAllVariables(&(SymTable->var));
    freeBothCompareLists();
    dtor(startCommandFuncList);
    freeFunctions(&(SymTable->func));
    
    free(SymTable);
    dtor(second_run);
   // fclose(program_code);
    //deleteStack(&expr);
    return error_flag;
}