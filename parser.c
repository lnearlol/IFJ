#include "parser.h"
int deep = -1;
bool function_body();
void allocating_error(){
     printf ("*ERROR* Memory was not alocated.\n");
    errflg = true;                      /* globální proměnná -- příznak chyby */
}

void get_and_set_token(){
    token->next = malloc (sizeof(Token));
    token = token->next;
    token->data = NULL;
    token->next = NULL;
    token->size = 0;
    token->type = 0;
    get_token(token);
}

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

bool function_body(){
    bool function_accept = false;
    if(token->type == TOKEN_TYPE_END_BLOCK){
       deep--;
    } else if(token->type == TOKEN_TYPE_START_BLOCK)
        deep++;


    printf("DEEP = %d\n\n", deep);
    if(token->type == TOKEN_TYPE_END_BLOCK && deep == -1)
        function_accept = true;
    else if(token->type == TOKEN_TYPE_EOL){
        get_and_set_token();
        printf("HI FROM FUNCTION_BODY %s\n", token->data);
        function_accept = function_body();
    }
    else if(token->type == TOKEN_TYPE_START_BLOCK){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
            printf("HI FROM FUNCTION_BODY %s\n", token->data);
            function_accept = function_body();
        }
    }
    else if(token->type == TOKEN_TYPE_END_BLOCK && deep != -1){
        get_and_set_token();
        if(token->type == TOKEN_TYPE_EOL){
            get_and_set_token();
                printf("HI FROM FUNCTION_BODY %s\n", token->data);
            function_accept = function_body();
        }
    } else if(token->type == TOKEN_TYPE_FOR){
        get_and_set_token();
        function_accept = for_construction();
    }
    

    return function_accept;
}

bool for_construction(){
    bool for_accept = false;
    printf("            HI FROM FOR_CONSTRUCTION %s\n", token->data);
    if(!math_expression(TOKEN_TYPE_SEMICOLON)){
            printf("            here 1\n");
        return false;
    }
    get_and_set_token();
    if(!logic_expression(TOKEN_TYPE_SEMICOLON)){
        printf("            here 2\n");
        return false;
    }
    get_and_set_token();
    if(!math_expression(TOKEN_TYPE_START_BLOCK)){
        printf("            here 3\n");
        return false;
    }
    if(token->type == TOKEN_TYPE_START_BLOCK){
        for_accept = function_body();
        printf("            here 4\n");
    }

    return for_accept;
}

bool math_expression(int end_condition){
    bool math_expression = false;
    if(token->type == end_condition)
        math_expression = true;

    return math_expression;
}

bool logic_expression(int end_condition){
    bool logic_expression = false;
    if(token->type == end_condition)
        logic_expression = true;

    return math_expression;
}

int main(){
    program_code = fopen ("file.ifj20", "r");

    token = malloc (sizeof(Token));
    token->data = NULL;
    token->next = NULL;
    token->size = 0;
    token->type = 0;
    Token *first = token;
    get_token(token);

    bool result = program_start();
    printf("PROGRAM RESULT:  %d\n", result);


    // while(first != NULL){
    //     printf("data: %s   number: %d \n", first->data, first->type);
    //     first = first->next;
    // }
    dctor(first);
    fclose(program_code);
    return 0;
}