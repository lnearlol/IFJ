#include "scanner.h"

int allowed_symbol(char state){
    if((state >= ' ' && state <= '"') || state == '\n' || state == TAB || state >= '(' && state <= '>' || 
    state >= 'A' && state <= 'Z' || state == '_' || state >= 'a' && state <= '{' || state == '}' || state == EOF)
        return 1;
    else 
        return 0;
}

int letter_or_num(char state){
    if(state >= '0' && state <= '9' || state >= 'A' && state <= 'Z' || state >= 'a' && state <= 'z' || state == '_')
        return 1;
    else 
        return 0;
}

int num(char state){
    if(state >= '0' && state <= '9')
        return 1;
    else 
        return 0;
}

char read_a_symbol(char state){
    state = fgetc(program_code);
    if(!allowed_symbol(state)){
        fprintf(stderr, "FORBIDDEN SYMBOL!\n");
        exit(1);
    }
    return state;
}

char first_non_space(char state){
    do{
        state = read_a_symbol(state);
    } while(state == ' ');

    return state;
}

char first_non_TAB(char state){
    while(state == TAB)
        state = read_a_symbol(state);

    return state;
}

char first_non_EOL(char state){
    do{
        state = read_a_symbol(state);
    } while(state == '\n' || state == TAB || state == ' ');

    return state;
}

void data_append(Token *token, char state){
    token->data = realloc(token->data, ++token->size);
    strncat(token->data, &state, 1);
}

int check_command_functions(Token *token){
    char command_functions[FUNCTIONS_ARRAY_SIZE][FUNCTIONS_ARRAY_SIZE] = {"inputs", "inputi", "inputf", "print", "int2float", "float2int", "len", "substr", "ord", "chr"};
    for(int i = 0; i < FUNCTIONS_ARRAY_SIZE; i++){
        if(!strcmp(token->data, command_functions[i]))
            return 1;
    }
    return 0;
}

void dtor(Token *first){
    Token *freed;
    while(first != NULL){
        freed = first->next;
        free(first->data);
        free(first);
        first = freed;
    }
}

void get_token(Token *token){
    static char state;
    static int state_flag = 0;
    token->size = 1;

    token->data = malloc(token->size);
    token->data[0] = '\0';

    if(state_flag == 0 || state == ' ')
        state = first_non_space(state);
    state_flag = 0;
    
    if(state == TAB)
        state = first_non_TAB(state);

    if(state >= '0' && state <= '9')
        token->type = TOKEN_TYPE_LITERAL_INT;
    else if((state >= 'a' && state <= 'z') || (state >= 'A' && state <= 'Z'))
        token->type = TOKEN_TYPE_IDENTIFIER;
    else if(state == '+' || state == '*' || state == '-' || state == '/'){
        token->type = TOKEN_TYPE_MATH_OPERATOR;
        data_append(token, state);
        return;
    }
    else if(state == '>' || state == '<' || state == '!')
        token->type = TOKEN_TYPE_LOGICAL_OPERATOR;
    else if(state == '\n'){
        token->type = TOKEN_TYPE_EOL;
        state = '$';
        data_append(token, state);
        state = first_non_EOL(state);
        state_flag++;
        return;
    }
    else if(state == '=')
        token->type = TOKEN_TYPE_EQUATING;
    else if(state == '"')
        token->type = TOKEN_TYPE_LITERAL_STRING;
    else if(state == '_')
        token->type = TOKEN_TYPE_UNDERSCORE;
    else if(state == ':')
        token->type = TOKEN_TYPE_DECLARE;
    else  if(state == EOF){
       token->type = TOKEN_TYPE_EOFILE;
       data_append(token, state);
       return;
    }
    else if(state == '{'){
        token->type = TOKEN_TYPE_START_BLOCK;
        data_append(token, state);
        return;
    }
    else if(state == '}'){
        token->type = TOKEN_TYPE_END_BLOCK;
        data_append(token, state);
        return;
    }
    else if(state == '('){
        token->type = TOKEN_TYPE_LEFT_BRACKET;
        data_append(token, state);
        return;
    }
    else if(state == ')'){
        token->type = TOKEN_TYPE_RIGHT_BRACKET;
        data_append(token, state);
        return;
    }
    else if(state == ','){
        token->type = TOKEN_TYPE_COMMA;
        data_append(token, state);
        return;
    }
    else if(state == ';'){
        token->type = TOKEN_TYPE_SEMICOLON;
        data_append(token, state);
        return;
    } else {
        printf("\n\nERROR!!!\n\n");
    }
    
    if(token->type != TOKEN_TYPE_LITERAL_STRING)
        data_append(token, state);
    
    int cycle_flag = 0;
    int Backslash_flag = 0;
    int ASCII_code_flag = 0;
    
    while(state  != EOF){

        if(cycle_flag == 0 && token->type != TOKEN_TYPE_LITERAL_STRING)
            state = read_a_symbol(state);
        
        cycle_flag = 0;

        if(token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_LITERAL_FLOAT){
            if(num(state)){
                if(strcmp(token->data, "0") == 0 && state == '0'){
                    fprintf(stderr, "EXCESS ZERO IN NUMERIC LITERAL\n");
                    exit(1);
                }
                data_append(token, state);
            }
            else if (state == '.'){
                if(strstr(token->data, ".") != NULL){
                    fprintf(stderr, "EXCESS DOT IN NUMERIC LITERAL\n");
                    exit(1);
                }
                data_append(token, state);
                token->type = TOKEN_TYPE_LITERAL_FLOAT;
            }
            else{
                state_flag++;
                return;
            }
            continue;
        }
        else if (token->type == TOKEN_TYPE_IDENTIFIER){
            if(letter_or_num(state)){
                data_append(token, state);
                state = read_a_symbol(state);
                if(!strcmp(token->data, "func") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_FUNC;
                else if(!strcmp(token->data, "package") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_PACKAGE;
                else if(!strcmp(token->data, "return") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_RETURN;
                else if(!strcmp(token->data, "for") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_FOR;
                else if(!strcmp(token->data, "if") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_IF;
                else if(!strcmp(token->data, "else") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_ELSE;
                else if(!strcmp(token->data, "int") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_INT;
                else if(!strcmp(token->data, "float64") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_FLOAT;
                else if(!strcmp(token->data, "string") && !letter_or_num(state))
                    token->type = TOKEN_TYPE_STRING;
                else if(check_command_functions(token) && !letter_or_num(state))
                    token->type = TOKEN_TYPE_COMMAND_FUNCTION;
                else if (letter_or_num(state)){
                    cycle_flag++;
                    continue;
                }
                state_flag++;
                return;
            }
            else{
                state_flag++;
                return;
            }
        }
        else if (token->type == TOKEN_TYPE_EQUATING){
            if(state != '=')
                state_flag++;
            else{
                token->type = TOKEN_TYPE_LOGICAL_OPERATOR;
                data_append(token, state);
            }
            return;
        }
        else if (token->type == TOKEN_TYPE_DECLARE){
            if(state == '='){
                data_append(token, state);
                return;
            }
            else{
                fprintf(stderr, "LEXICAL ERROR (':' NOT RELATED TO DECLARE)\n");
                exit(1);
            }
        }
        else if (token->type == TOKEN_TYPE_LOGICAL_OPERATOR){
            if(state != '=')
                state_flag++;
            else
                data_append(token, state);
            return;
        }
        else if (token->type == TOKEN_TYPE_UNDERSCORE){
            if(!letter_or_num(state))
                state_flag++;
            else{
                token->type = TOKEN_TYPE_IDENTIFIER;
                data_append(token, state);
                continue;
            }
            return;
        }
        else if (token->type == TOKEN_TYPE_LITERAL_STRING){
            ///
            ////
            /////
            //ERROR: \x USED WITH NO FOLLOWING HEX DIGITS
            /////
            ///////
            //
            state = fgetc(program_code);
            
            if(state != '"' || Backslash_flag != 0){
                if(state == '\n'){
                    fprintf(stderr, "ERROR: MISSING TERMINATING\n");
                    exit(1);
                }

                if(ASCII_code_flag !=0){
                    printf("\n    hexa ==> %c\n", state);
                    // data_append(token, state);
                    ASCII_code_flag = 0;
                }
                else{
                    if(state != Backslash){
                        if(Backslash_flag !=0){
                            if(state == 'n')
                                state = '\n';
                            else if(state == 't')
                                state = TAB;
                            /*else if(state == 'x'){
                                ASCII_code_flag++;
                                Backslash_flag = 0;
                                continue;
                            }*/
                            else if(state != '"'){
                                fprintf(stderr, "ERROR: UNKNOWN ESCAPE SEQUENCE\n");
                                exit(1);
                            }
                        }
                        data_append(token, state);
                        Backslash_flag = 0;
                    }
                    else{
                        if(Backslash_flag !=0){
                            data_append(token, state);
                            Backslash_flag = 0;
                        }
                        else
                            Backslash_flag++;
                    }
                }
                continue;
            }
            return;
        }
    }
}
