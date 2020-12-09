<<<<<<< Updated upstream
#include "scanner.h"

=======
/**
 * @file scanner.c
 *
 * @brief Functions for the lexical analyzer
 *
 * IFJ Projekt 2020, Tým 55
 *
 * @author <xbahda01> Bahdanovich Viktoryia 
 */

#include "scanner.h"

/**
 * Helper function for `get_token`, checks input characters validation
 * @param state is a character that we check
 * @return Function returns 1 if symbol is allowed, 0 if not
 */
>>>>>>> Stashed changes
int allowed_symbol(char state){
	if((state >= ' ' && state <= '"') || state == '\n' || state == TAB || state >= '(' && state <= '>' || 
	state >= 'A' && state <= 'Z' || state == '_' || state >= 'a' && state <= '{' || state == '}' || state == EOF)
		return 1;
	else 
		return 0;
}
<<<<<<< Updated upstream

=======
/**
 * Helper function for `get_token`, determines if state is a number, letter or an underscore
 * @param state is a character that we check
 * @return Function returns 1 if symbol matches digits, letters or an underscore, 0 if not
 */
>>>>>>> Stashed changes
int letter_or_num(char state){
	if(state >= '0' && state <= '9' || state >= 'A' && state <= 'Z' || state >= 'a' && state <= 'z' || state == '_')
		return 1;
	else 
		return 0;
}
<<<<<<< Updated upstream

=======
/**
 * Helper function for `get_token`, determines if it is a digit
 * @param state is a character that we check
 * @return Function returns 1 if symbol matches digits, 0 if not
 */
>>>>>>> Stashed changes
int num(char state){
	if(state >= '0' && state <= '9')
		return 1;
	else 
		return 0;
}
<<<<<<< Updated upstream

=======
/**
 * Helper function for `get_token`, reads a symbol from stdin
 * @param state is a character in which we write a symbol from stdin, which we check and return
 * @return Function returns state, which we read or `%` if it wasn`t allowed
 */
>>>>>>> Stashed changes
char read_a_symbol(char state){
	state = fgetc(stdin);
	if(!allowed_symbol(state))      //ERROR: FORBIDDEN SYMBOL
		return '%';
	return state;
}
<<<<<<< Updated upstream

=======
/**
 * Helper function for `get_token`, reads first non space symbol
 * @param state is a character that we check, rewrite and return
 * @return Function returns state, which we read
 */
>>>>>>> Stashed changes
char first_non_space(char state){
	do{
		state = read_a_symbol(state);
	} while(state == ' ');

	return state;
}
<<<<<<< Updated upstream

=======
/**
 * Helper function for `get_token`, reads first non TAB symbol
 * @param state is a character that we check, rewrite and return
 * @return Function returns state, which we read
 */
>>>>>>> Stashed changes
char first_non_TAB(char state){
	while(state == TAB)
		state = read_a_symbol(state);

	return state;
}
<<<<<<< Updated upstream

=======
/**
 * Helper function for `get_token`, reads first non TAB, non EOL and non space symbol
 * @param state is a character that we check, rewrite and return
 * @return Function returns state, which we read
 */
>>>>>>> Stashed changes
char first_non_EOL(char state){
	do{
		state = read_a_symbol(state);
	} while(state == '\n' || state == TAB || state == ' ');

	return state;
}
<<<<<<< Updated upstream

void data_append(Token *token, char state){
    token->data = realloc(token->data, ++token->size);
    strncat(token->data, &state, 1);
}

=======
/**
 * Helper function for `get_token`, filling token data
 * @param token Structure instance that contains information about token
 * @param state is a character that we add to token->data
 * @return Function returns 1 if it was an reallocation error, 0 if not
 */
int data_append(Token *token, char state){
	token->data = realloc(token->data, ++token->size);
	if(token->data == NULL){
		changeErrorCode(99);
		return 1;
	}
	strncat(token->data, &state, 1);
	return 0;
}

/**
 * Helper function for `get_token`, checks if token data is a command function
 * @param token Structure instance that contains information about token
 * @return Function returns 1 if token->data contains a command function name, 0 if not
 */
>>>>>>> Stashed changes
int check_command_functions(Token *token){
	char command_functions[FUNCTIONS_ARRAY_SIZE][FUNCTIONS_ARRAY_SIZE] = {"inputs", "inputi", "inputf", "print", "int2float", "float2int", "len", "substr", "ord", "chr"};
	for(int i = 0; i < FUNCTIONS_ARRAY_SIZE; i++){
		if(!strcmp(token->data, command_functions[i]))
			return 1;
	}
	return 0;
}

/**
 * Function deallocates memory
 * @param first The first structure instance that contains information about token
 */
void dtor(Token *first){
	Token *freed;
	while(first != NULL){
		freed = first->next;
		free(first->data);
		free(first);
		first = freed;
	}
}

/**
 * Function of processing and analyzing of program code
 * @param token Structure instance that contains information about token
 * @return Function returns 1 if it was an error, 0 if not
 */
int get_token(Token *token){
	static char state;
	static int state_flag = 0;
	static int EOL_flag = 0;
	token->size = 1;
	token->type = WRONG_DATA_TOKEN_TYPE;
	token->data = malloc(token->size);
	
	if(token->data == NULL){
		changeErrorCode(99);
		return 1;
	}

	token->data[0] = '\0';

	if(EOL_flag != 0){
		state = first_non_EOL(state);
		state_flag++;
		if(state != '/')
			EOL_flag = 0;
	}

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
		if(data_append(token, state))
			return 1;

		if(state == '/'){
			state = fgetc(stdin);
			if(state != '/' && state != '*'){
				state_flag++;
				EOL_flag = 0;
			}
			else{
				int comment_ending_flag = 0;
				free(token->data);
				if(state == '/'){
					while(state != '\n' && !feof( stdin ))
						state = fgetc(stdin);
					comment_ending_flag = 1;
					state_flag++;
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

		if(data_append(token, state))
			return 1;

		EOL_flag++;
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

		 if(data_append(token, state))
            return 1;
			 
		 return 0;
	}
	else if(state == '{'){
		token->type = TOKEN_TYPE_START_BLOCK;
		
        if(data_append(token, state))
            return 1;

		return 0;
	}
	else if(state == '}'){
		token->type = TOKEN_TYPE_END_BLOCK;
		
        if(data_append(token, state))
            return 1;

		return 0;
	}
	else if(state == '('){
		token->type = TOKEN_TYPE_LEFT_BRACKET;
		
        if(data_append(token, state))
            return 1;

		return 0;
	}
	else if(state == ')'){
		token->type = TOKEN_TYPE_RIGHT_BRACKET;
		
        if(data_append(token, state))
            return 1;

		return 0;
	}
	else if(state == ','){
		token->type = TOKEN_TYPE_COMMA;
		
        if(data_append(token, state))
            return 1;

		return 0;
	}
	else if(state == ';'){
		token->type = TOKEN_TYPE_SEMICOLON;
		
        if(data_append(token, state))
            return 1;

		return 0;
	}
	else if(state == '.')      //ERROR: EMPTY SEQUENCE OF NUMBERS BEFORE `.` IN AN INTEGER PART OF LITERAL FLOAT
		return 1;
	else if (state == '%')
		return 1;
	
	if(token->type != TOKEN_TYPE_LITERAL_STRING){
		if(data_append(token, state))
            return 1;
    }
	
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
				
                if(data_append(token, state))
                    return 1;
			}
			else if(state == '.'){
				if(strstr(token->data, ".") != NULL)    //ERROR: EXCESS DOT IN NUMERIC LITERAL
					return 1;
				
                if(data_append(token, state))
                    return 1;

				token->type = TOKEN_TYPE_LITERAL_FLOAT;
				state = read_a_symbol(state);
				if (state == '%')
					return 1;
				if(state < '0' || state > '9')      //ERROR: EMPTY SEQUENCE OF NUMBERS AFTER `.` IN A FRACTIONAL PART OF LITERAL FLOAT
					return 1;
				else{
					if(data_append(token, state))
                        return 1;
				}
			}
			else if(state == 'e' || state == 'E'){
				
                if(data_append(token, state))
                    return 1;

				token->type = TOKEN_TYPE_LITERAL_FLOAT;
				state = read_a_symbol(state);
				if (state == '%')
					return 1;
				if(state == '+' || state == '-'){
					
                    if(data_append(token, state))
                        return 1;
                        
					state = read_a_symbol(state);

					if (state == '%')
						return 1;
					
					if(state >= '0' && state <= '9'){
						while(state >= '0' && state <= '9'){
							if(data_append(token, state))
                                return 1;
                            
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
						
                        if(data_append(token, state))
                            return 1;

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
				
                if(data_append(token, state))
                    return 1;

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
				
                if(data_append(token, state))
                    return 1;
			}
			return 0;
		}
		else if (token->type == TOKEN_TYPE_DECLARE){
			if(state == '='){
				if(data_append(token, state))
                    return 1;

				return 0;
			}
			else    //LEXICAL ERROR (':' NOT RELATED TO DECLARE)
				return 1;
		}
		else if (token->type == TOKEN_TYPE_LOGICAL_OPERATOR){
			if(state != '=')
				state_flag++;
			else{
				if(data_append(token, state))
                    return 1;
            }
			return 0;
		}
		else if (token->type == TOKEN_TYPE_UNDERSCORE){
			if(!letter_or_num(state))
				state_flag++;
			else{
				token->type = TOKEN_TYPE_IDENTIFIER;
				
                if(data_append(token, state))
                    return 1;

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
						}
						else    //ERROR: TOO FEW HEXADECIMAL DIGITS
							return 1;
					}
					else    //ERROR: \\x USED WITH NO FOLLOWING HEX DIGITS
						return 1;
					
                    if(data_append(token, state))
                        return 1;

					ASCII_code_flag = 0;
				}
				else{
					if(state != Backslash){
						if(Backslash_flag !=0){
							if(state == 'n')
								state = '\n';
							else if(state == 't')
								state = TAB;
							else if(state == 'x'){
								ASCII_code_flag++;
								Backslash_flag = 0;
								continue;
							}
							else if(state != '"')   //ERROR: UNKNOWN ESCAPE SEQUENCE
								return 1;
						}
						
                        if(data_append(token, state))
                            return 1;

						Backslash_flag = 0;
					}
					else{
						if(Backslash_flag !=0){
							if(data_append(token, state))
                                return 1;
                                
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