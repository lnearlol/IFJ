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
    } while(state == '\n');

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
            token->type = TOKEN_TYPE_COMMAND_FUNCTION;
    }
    return 0;
}

void dctor(Token *first){
    while(first != NULL){
        Token *freed = first;
        free(freed);
        first = first->next;
    }
}

void get_token(Token *token){
    char logical_operators[LOGICAL_OPERATORS_QUANTITY][LOGICAL_OPERATORS_LENGTH] = {">", "<", "==", "!=", ">=", "<="};

    static char state;
    static int state_flag = 0;
    token->size = 1;

    token->data = malloc(token->size);

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
    else if(state == '>' || state == '<' || state == '=' || state == '!')
        token->type = TOKEN_TYPE_LOGICAL_OPERATOR;
    else if(state == '\n'){
        token->type = TOKEN_TYPE_EOL;
        data_append(token, '$');
        state = first_non_EOL(state);
        state_flag++;
        return;
    }
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
        
    data_append(token, state);

    int cycle_flag = 0;
    while(state  != EOF){

        if(token->type <= TOKEN_TYPE_UNDERSCORE && cycle_flag == 0){
            state = read_a_symbol(state);
            cycle_flag = 0;
        }

        if(token->type == TOKEN_TYPE_LITERAL_INT){
            if(num(state))
                data_append(token, state);
            else if (state == '.'){
                data_append(token, state);
                token->type = TOKEN_TYPE_LITERAL_FLOAT;
            }
            else{
                state_flag++;
                return;
            }
        }
        else if (token->type == TOKEN_TYPE_IDENTIFIER){
            if(letter_or_num(state)){
                data_append(token, state);
                state = read_a_symbol(state);
                if(!strcmp(token->data, "func") && !letter_or_num(state)){
                    token->type = TOKEN_TYPE_FUNC;
                }
                else if(!strcmp(token->data, "package") && !letter_or_num(state)){
                    token->type = TOKEN_TYPE_PACKAGE;
                }
                else if(!strcmp(token->data, "int") && !letter_or_num(state)){
                    token->type = TOKEN_TYPE_INT;
                }
                else if(!strcmp(token->data, "return") && !letter_or_num(state)){
                    token->type = TOKEN_TYPE_RETURN;
                }
                else if(!strcmp(token->data, "for") && !letter_or_num(state)){
                    token->type = TOKEN_TYPE_FOR;
                }
                else if(check_command_functions(token) && !letter_or_num(state)){
                    
                }
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
    }
}