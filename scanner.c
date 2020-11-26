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
    state = fgetc(stdin);
    if(!allowed_symbol(state))      //ERROR: FORBIDDEN SYMBOL
        return '%';
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

int get_token(Token *token){
    static char state;
    static int state_flag = 0;
    token->size = 1;
    token->type = WRONG_DATA_TOKEN_TYPE;
    token->data = malloc(token->size);
    token->data[0] = '\0';

    if(state_flag == 0 || state == ' ')
        state = first_non_space(state);
    state_flag = 0;
    
    if(state == TAB)
        state = first_non_TAB(state);
    
    if (state == '%')
        return 1;

    if(state >= '0' && state <= '9')
        token->type = TOKEN_TYPE_LITERAL_INT;
    else if((state >= 'a' && state <= 'z') || (state >= 'A' && state <= 'Z'))
        token->type = TOKEN_TYPE_IDENTIFIER;
    else if(state == '+' || state == '*' || state == '-' || state == '/'){
        data_append(token, state);

        if(state == '/'){
            state = fgetc(stdin);
            if(state != '/' && state != '*')
                state_flag++;
            else{
                int comment_ending_flag = 0;
                free(token->data);
                if(state == '/'){
                    while(state != '\n' && !feof( stdin ))
                        state = fgetc(stdin);
                    comment_ending_flag = 1;
                }
                else if(state == '*'){
                    while(!feof( stdin )){
                        state = fgetc(stdin);
                        if(state == '*'){
                            state = fgetc(stdin);
                            if(state == '/'){
                                comment_ending_flag = 1;
                                break;
                            }
                        }
                    }
                }

                state = first_non_EOL(state);
                if (state == '%')
                    return 1;
                state_flag++;
                if(get_token(token) || !comment_ending_flag)
                    return 1;

                return 0;
            }
        }

        token->type = TOKEN_TYPE_MATH_OPERATOR;
        return 0;
    }
    else if(state == '>' || state == '<' || state == '!')
        token->type = TOKEN_TYPE_LOGICAL_OPERATOR;
    else if(state == '\n'){
        token->type = TOKEN_TYPE_EOL;
        state = '$';
        data_append(token, state);
        state = first_non_EOL(state);

        if (state == '%')
            return 1;

        state_flag++;
        return 0;
    }
    else if(state == '=')
        token->type = TOKEN_TYPE_EQUATING;
    else if(state == '"')
        token->type = TOKEN_TYPE_LITERAL_STRING;
    else if(state == '_')
        token->type = TOKEN_TYPE_UNDERSCORE;
    else if(state == ':')
        token->type = TOKEN_TYPE_DECLARE;
    else  if(feof( stdin )){
       token->type = TOKEN_TYPE_EOFILE;
       data_append(token, state);
       return 0;
    }
    else if(state == '{'){
        token->type = TOKEN_TYPE_START_BLOCK;
        data_append(token, state);
        return 0;
    }
    else if(state == '}'){
        token->type = TOKEN_TYPE_END_BLOCK;
        data_append(token, state);
        return 0;
    }
    else if(state == '('){
        token->type = TOKEN_TYPE_LEFT_BRACKET;
        data_append(token, state);
        return 0;
    }
    else if(state == ')'){
        token->type = TOKEN_TYPE_RIGHT_BRACKET;
        data_append(token, state);
        return 0;
    }
    else if(state == ','){
        token->type = TOKEN_TYPE_COMMA;
        data_append(token, state);
        return 0;
    }
    else if(state == ';'){
        token->type = TOKEN_TYPE_SEMICOLON;
        data_append(token, state);
        return 0;
    }
    else if(state == '.')      //ERROR: EMPTY SEQUENCE OF NUMBERS BEFORE `.` IN AN INTEGER PART OF LITERAL FLOAT
        return 1;
    else if (state == '%')
        return 1;
    
    if(token->type != TOKEN_TYPE_LITERAL_STRING)
        data_append(token, state);
    
    int cycle_flag = 0;
    int Backslash_flag = 0;
    int ASCII_code_flag = 0;
    
    while(!feof( stdin )){

        if(cycle_flag == 0 && token->type != TOKEN_TYPE_LITERAL_STRING)
            state = read_a_symbol(state);
        
        if (state == '%')
            return 1;
        
        cycle_flag = 0;

        if(token->type == TOKEN_TYPE_LITERAL_INT || token->type == TOKEN_TYPE_LITERAL_FLOAT){
            if(num(state)){
                if(strcmp(token->data, "0") == 0)   //ERROR: EXCESS ZERO IN NUMERIC LITERAL
                    return 1;
                data_append(token, state);
            }
            else if(state == '.'){
                if(strstr(token->data, ".") != NULL)    //ERROR: EXCESS DOT IN NUMERIC LITERAL
                    return 1;
                data_append(token, state);
                token->type = TOKEN_TYPE_LITERAL_FLOAT;
                state = read_a_symbol(state);
                if (state == '%')
                    return 1;
                if(state < '0' || state > '9')      //ERROR: EMPTY SEQUENCE OF NUMBERS AFTER `.` IN A FRACTIONAL PART OF LITERAL FLOAT
                    return 1;
                else{
                    data_append(token, state);
                }
            }
            else if(state == 'e' || state == 'E'){
                data_append(token, state);
                token->type = TOKEN_TYPE_LITERAL_FLOAT;
                state = read_a_symbol(state);
                if (state == '%')
                    return 1;
                if(state == '+' || state == '-'){
                    data_append(token, state);
                    state = read_a_symbol(state);

                    if (state == '%')
                        return 1;
                    
                    if(state >= '0' && state <= '9'){
                        while(state >= '0' && state <= '9'){
                            data_append(token, state);
                            state = read_a_symbol(state);

                            if (state == '%')
                                return 1;
                        }
                    }
                    else    //ERROR: EMPTY SEQUENCE OF NUMBERS AFTER `e` IN LITERAL FLOAT
                        return 1;
                }
                else if(state >= '0' && state <= '9'){
                    while(state >= '0' && state <= '9'){
                        data_append(token, state);
                        state = read_a_symbol(state);

                        if (state == '%')
                            return 1;
                    }
                }
                else    //ERROR: EMPTY SEQUENCE OF NUMBERS AFTER `e` IN LITERAL FLOAT
                    return 1;
                state_flag++;
                return 0;
            }
            else{
                state_flag++;
                return 0;
            }
            continue;
        }
        else if (token->type == TOKEN_TYPE_IDENTIFIER){
            if(letter_or_num(state)){
                data_append(token, state);
                state = read_a_symbol(state);

                if (state == '%')
                    return 1;
                
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
                return 0;
            }
            else{
                state_flag++;
                return 0;
            }
        }
        else if (token->type == TOKEN_TYPE_EQUATING){
            if(state != '=')
                state_flag++;
            else{
                token->type = TOKEN_TYPE_LOGICAL_OPERATOR;
                data_append(token, state);
            }
            return 0;
        }
        else if (token->type == TOKEN_TYPE_DECLARE){
            if(state == '='){
                data_append(token, state);
                return 0;
            }
            else    //LEXICAL ERROR (':' NOT RELATED TO DECLARE)
                return 1;
        }
        else if (token->type == TOKEN_TYPE_LOGICAL_OPERATOR){
            if(state != '=')
                state_flag++;
            else
                data_append(token, state);
            return 0;
        }
        else if (token->type == TOKEN_TYPE_UNDERSCORE){
            if(!letter_or_num(state))
                state_flag++;
            else{
                token->type = TOKEN_TYPE_IDENTIFIER;
                data_append(token, state);
                continue;
            }
            return 0;
        }
        else if (token->type == TOKEN_TYPE_LITERAL_STRING){
            state = fgetc(stdin);
            
            if(state != '"' || Backslash_flag != 0){
                if(state == '\n')   //ERROR: MISSING TERMINATING
                    return 1;

                if(ASCII_code_flag != 0){
                    if((state >= '0' && state <= '9') || (state >= 'A' && state <= 'F') || (state >= 'a' && state <= 'f')){
                        char str[HEX_DIGITS_STRING_SIZE], *end;

                        str[0] = state;
                        state = fgetc(stdin);

                        if((state >= '0' && state <= '9') || (state >= 'A' && state <= 'F') || (state >= 'a' && state <= 'f')){
                            str[1] = state;
                            state = strtol(str, &end, 16);
                            if((state >= 0 && state <= 32) || state == 35 || state == 92){
                                char *ASCII_code=(char*)malloc(ASCII_CODE_ARRAY_SIZE);
                                sprintf(ASCII_code, "%03ld", strtol(str, &end, 16));
                                state = Backslash;
                                data_append(token, state);
                                state = ASCII_code[0];
                                data_append(token, state);
                                state = ASCII_code[1];
                                data_append(token, state);
                                state = ASCII_code[2];
                                free(ASCII_code);
                            }
                        }
                        else    //ERROR: TOO FEW HEXADECIMAL DIGITS
                            return 1;
                    }
                    else    //ERROR: \\x USED WITH NO FOLLOWING HEX DIGITS
                        return 1;
                    data_append(token, state);
                    ASCII_code_flag = 0;
                }
                else{
                    if(state != Backslash){
                        if(Backslash_flag !=0){
                            if(state == 'n'){
                                state = Backslash;
                                data_append(token, state);
                                state = '0';
                                data_append(token, state);
                                state = '1';
                                data_append(token, state);
                                state = '0';
                            }
                            else if(state == 't'){
                                state = Backslash;
                                data_append(token, state);
                                state = '0';
                                data_append(token, state);
                                state = '0';
                                data_append(token, state);
                                state = '9';
                            }
                            else if(state == 'x'){
                                ASCII_code_flag++;
                                Backslash_flag = 0;
                                continue;
                            }
                            else if(state != '"')   //ERROR: UNKNOWN ESCAPE SEQUENCE
                                return 1;
                        }
                        else if(state == ' '){
                            state = Backslash;
                            data_append(token, state);
                            state = '0';
                            data_append(token, state);
                            state = '3';
                            data_append(token, state);
                            state = '2';
                        }
                        else if(state == '#'){
                            state = Backslash;
                            data_append(token, state);
                            state = '0';
                            data_append(token, state);
                            state = '3';
                            data_append(token, state);
                            state = '5';
                        }
                        data_append(token, state);
                        Backslash_flag = 0;
                    }
                    else{
                        if(Backslash_flag !=0){
                            state = Backslash;
                            data_append(token, state);
                            state = '0';
                            data_append(token, state);
                            state = '9';
                            data_append(token, state);
                            state = '2';
                            data_append(token, state);
                            Backslash_flag = 0;
                        }
                        else
                            Backslash_flag++;
                    }
                }
                continue;
            }
            return 0;
        }
    }
}