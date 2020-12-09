/**
 * @file expression_translator.c
 *
 * @brief Functions for processing expressions
 *
 * IFJ Projekt 2020, Tým 55
 *
 * @author <xpastu04> Pastushenko Vladislav
 */

#include "expression_translator.h"


/**
 * Function for creating stack of tokens.
 * @return New empty stack of tokens
 */
Stack_t* createStack() {
    Stack_t *out = NULL;
    out = malloc(sizeof(Stack_t));
    if (out == NULL) {
        exit(OUT_OF_MEMORY);
    }
    out->size = INIT_SIZE;
    out->data = malloc(out->size * sizeof(Token));
    if (out->data == NULL) {
        free(out);
        exit(OUT_OF_MEMORY);
    }
    out->top = 0;
    return out;
}

/**
 * Function for deleting stack of tokens.
 * @param stack Pointer to the deleting stack.
 */
void deleteStack(Stack_t **stack) {
        if(*stack != NULL) {
                free((*stack)->data);
                free(*stack);
                *stack = NULL;
        }
}

/**
 * Resizing of stack of tokens.
 * @param stack Resized stack.
 */
void resize(Stack_t *stack) {
    stack->size *= MULTIPLIER;
    stack->data = realloc(stack->data, stack->size * sizeof(Token));
    if (stack->data == NULL) {
        exit(STACK_OVERFLOW);
    }
}

/**
 * Push token to the end of stack
 * @param stack Stack to which token will be pushed.
 * @param value Pushed token
 */
void push(Stack_t *stack, Token value) {
    if (stack->top >= stack->size) {
        resize(stack);
    }
    stack->data[stack->top] = value;
    stack->top++;
}

/**
 * Pop token from the end of stack
 * @param stack Stack from which token will be pop.
 * @return Pop token
 */
Token pop(Stack_t *stack) {
    if (stack->top == 0) {
        exit(STACK_UNDERFLOW);
    }
    stack->top--;
    return stack->data[stack->top];
}

/**
 * Pop penultimate token from the stack
 * @param stack Stack from which token will be pop.
 * @return Pop token
 */
Token peek(const Stack_t *stack) {
    if (stack->top <= 0) {
        exit(STACK_UNDERFLOW);
    }
    return stack->data[stack->top - 1];
}

/**
 * Delete all empty blocks from the end of stack
 * @param stack Stack which will be imploded
 */
void implode(Stack_t *stack) {
    stack->size = stack->top;
    stack->data = realloc(stack->data, stack->size * sizeof(Token));
}


typedef enum
{
	S, // <
	E, // =
	R, // >
	N  // ERR
} Prec_table_sign_enum;

int prec_table[7][7] = {
//	|+- | */ | r | ( | ) | i | $ |
	{ R , S  , R , S , R , S , R }, // +-
	{ R , R  , R , S , R , S , R }, // */
	{ S , S  , N , S , R , S , R }, // = <> <= < >= >
	{ S , S  , S , S , E , S , N }, // (
	{ R , R  , R , N , R , N , R }, // )
	{ R , R  , R , N , R , N , R }, // i
	{ S , S  , S , S , N , S , N }  // $
};

/**
 * Check token and return its priority
 * @param token Verifiable token
 * @return An integer representing the priority number
 */
int get_index(Token token) {
    if (token.type == TOKEN_TYPE_MATH_OPERATOR) {
        if (strcmp(token.data, "+") == 0 || strcmp(token.data, "-") == 0) {
            return 0;
        } else if (strcmp(token.data, "*") == 0 || strcmp(token.data, "/") == 0) {
            return 1;
    }
    } else if (token.type == TOKEN_TYPE_LOGICAL_OPERATOR) {
        return 2;
    } else if (token.type == TOKEN_TYPE_LEFT_BRACKET) {
        return 3;
    } else if (token.type == TOKEN_TYPE_RIGHT_BRACKET) {
        return 4;
    } else if (token.type == TOKEN_TYPE_LITERAL_FLOAT ||
               token.type == TOKEN_TYPE_LITERAL_INT ||
               token.type == TOKEN_TYPE_IDENTIFIER ||
               token.type == TOKEN_TYPE_LITERAL_STRING) {
        return 5;
    } else {
        return 6;
    }
}


/**
 * Take stack of tokens, which contains expression, and create new stack of tokens,
 * and controls if it has any errors. If it has error, returns -1, if no - returns type
 * of expression
 * @param deepVar Signifies the deep of the variable
 * @param Var Pointer to the symbol table for variables
 * @return An integer representing the expression type
 */
int expression_processor(Stack_t *stack, int deepVar, variable Var) {

        if (stack->top == 0) {
                deleteStack(&stack);
                return 0;
        }


        //implode(stack);
        int result = 0;
        bool logical_type_flag = false;
        for (int i = 0; i < stack->top; i++) {
                Token token = stack->data[i];
                if (token.type == TOKEN_TYPE_LITERAL_FLOAT ||
                    token.type == TOKEN_TYPE_LITERAL_INT ||
                    token.type == TOKEN_TYPE_IDENTIFIER ||
                    token.type == TOKEN_TYPE_LITERAL_STRING) {

                        // Check and control type of expression
                        if (token.type == TOKEN_TYPE_IDENTIFIER) {
                            if (findVariableWithType(&token, deepVar, Var) == NULL) {
                                changeErrorCode(3);
                                result = -1;
                                break;
                            }
                        }

                        if (result == 0) {
                            if (token.type == TOKEN_TYPE_IDENTIFIER)
                                result = findVariableWithType(&token, deepVar, Var)->type;
                            else
                                result = returnLiteralType(&token);
                        }
                        else if (result != 0) {
                            if (token.type == TOKEN_TYPE_IDENTIFIER) {
                                if (result != findVariableWithType(&token, deepVar, Var)->type) {
                                    changeErrorCode(5);
                                    result = -1;
                                    break;
                                }
                             }
                             else {
                                if (result != returnLiteralType(&token)) {
                                    changeErrorCode(5);
                                    result = -1;
                                    break;
                                }
                             }
                        }
                } else if (token.type == TOKEN_TYPE_LEFT_BRACKET){
                } else if (token.type == TOKEN_TYPE_RIGHT_BRACKET) {
                    if (logical_type_flag) {
                        if (i != stack->top - 1) {
                            logical_type_flag = false;
                            changeErrorCode(5);
                            result = -1;
                            break;
                        }
                    }
                } else if (token.type == TOKEN_TYPE_MATH_OPERATOR || token.type == TOKEN_TYPE_LOGICAL_OPERATOR) {
                        // Check if logical expression. If expression includes few comparing operators it returns an error
                        if (token.type == TOKEN_TYPE_LOGICAL_OPERATOR) {
                            if (!logical_type_flag)
                                logical_type_flag = true;
                            else {
                                logical_type_flag = false;
                                changeErrorCode(5);
                                result = -1;
                                break;
                            }
                        }
                        // Check if divided by zero
                        if (strcmp(token.data, "/") == 0) {
                            if (strcmp(stack->data[i + 1].data, "0") == 0 || strcmp(stack->data[i + 1].data, "0.0") == 0) {
                                changeErrorCode(9);
                                result = -1;
                                break;
                            }
                        }
                }
                else break;
        }

        //printf("result %d\n", result);
        if (result != -1) generateCode(stack, deepVar, Var, result);
        deleteStack(&stack);
        if (logical_type_flag) result = 4;
        return result;

}

/**
 * Prints assembly commands used to process expressions.
 * @param stack Stack of tokens with expression
 * @param deepVar Signifies the deep of the variable
 * @param Var Pointer to the symbol table for variables
 * @param incomingType An integer representing the expression type
 */
void generateCode(Stack_t *stack, int deepVar, variable Var, int incomingType) {
        Token prevToken;
        prevToken.type = TOKEN_TYPE_EOL;
        prevToken.data = "x";
        Token breaker;
        breaker.type = TOKEN_TYPE_END_BLOCK;
        breaker.data = "<";
        Stack_t* tmpStack = createStack();

        push(stack, prevToken);
        push(tmpStack, prevToken);
        int type = 0;

        int i = 0;
        implode(stack);
        while (true) {
            prevToken = tmpStack->data[tmpStack->top -1];
            Token actualToken = stack->data[i];

            if (type == 0) {
                    if (actualToken.type == TOKEN_TYPE_IDENTIFIER)
                        type = findVariableWithType(&actualToken, deepVar, Var)->type;
                    else
                        type = returnLiteralType(&actualToken);
            }
            int sign = prec_table[get_index(prevToken)][get_index(actualToken)];
            bool checker;
            switch (sign)
            {
            case S: // <
                push(tmpStack, breaker);
                push(tmpStack, actualToken);
                i++;
                break;
            case E: // =
                push(tmpStack, actualToken);
                i++;
                break;
            case R: // >
                if (tmpStack->top > 1) {
                    if (tmpStack->data[tmpStack->top-2].type == TOKEN_TYPE_END_BLOCK || tmpStack->data[tmpStack->top-1].type == TOKEN_TYPE_RIGHT_BRACKET)  {

                        checker = true;
                    } else {
                            checker = false;
                    }
                } else {
                    checker = true;
                }

                if (checker) {
                    Token tmpToken = pop(tmpStack);
                    pop(tmpStack);
                    if (tmpToken.type == TOKEN_TYPE_RIGHT_BRACKET) pop(tmpStack);

                    if (tmpToken.type == TOKEN_TYPE_LITERAL_FLOAT ||
                    tmpToken.type == TOKEN_TYPE_LITERAL_INT ||
                    tmpToken.type == TOKEN_TYPE_IDENTIFIER ||
                    tmpToken.type == TOKEN_TYPE_LITERAL_STRING) {
                        if(GET_REPEAT_FUNC_RUN()){
                            printf("PUSHS ");
                            GEN_WRITE_VAR_LITERAL(&tmpToken, deepVar);
                            printf("\n\n");
                        }
                    } else if (tmpToken.type == TOKEN_TYPE_MATH_OPERATOR) {
                        if (type != 3) {
                            if (strcmp(tmpToken.data, "/") == 0) {
                                if(incomingType == 1) {
                                    if(GET_REPEAT_FUNC_RUN()){
                                        printf("POPS GF@tmpDividingByZero\n");
                                        printf("JUMPIFEQ error9 GF@tmpDividingByZero int@0\n");
                                        printf("PUSHS GF@tmpDividingByZero\n");
                                        printf("IDIVS\n");
                                    }
                                }
                                else if (incomingType == 2) {
                                    if(GET_REPEAT_FUNC_RUN()){
                                        printf("POPS GF@tmpDividingByZero\n");
                                        printf("JUMPIFEQ error9 GF@tmpDividingByZero float@0x0p+0\n");
                                        printf("PUSHS GF@tmpDividingByZero\n");
                                        printf("DIVS\n");
                                    }
                                }
                            }
                            else if (strcmp(tmpToken.data, "*") == 0) {
                                if(GET_REPEAT_FUNC_RUN())
                                    printf("MULS\n");
                            }
                            else if (strcmp(tmpToken.data, "+") == 0) {
                                if(GET_REPEAT_FUNC_RUN())
                                    printf("ADDS\n");
                            }
                            else if (strcmp(tmpToken.data, "-") == 0) {
                                if(GET_REPEAT_FUNC_RUN())
                                    printf("SUBS\n");
                            }

                        }
                        else {
                            if (strcmp(tmpToken.data, "+") == 0) {
                                if(GET_REPEAT_FUNC_RUN()){
                                    printf("POPS GF@str2\n");
                                    printf("POPS GF@str1\n");
                                    printf("CONCAT GF@strRes GF@str1 GF@str2\n");
                                    printf("PUSHS GF@strRes\n");
                                }
                            } else
                                break;
                        }
                    }
                    else if (tmpToken.type == TOKEN_TYPE_LOGICAL_OPERATOR) {
                        printf("SAASDSADADASD\n");
                        printf("tmpToken.data %s \n", tmpToken.data);
                        if (strcmp(tmpToken.data, "<") == 0) {
                            if(GET_REPEAT_FUNC_RUN()){
                                printf("LTS\n");
                            }
                        }
                        else if (strcmp(tmpToken.data, "<=") == 0) {
                            if(GET_REPEAT_FUNC_RUN()){
                                printf("GTS\n");
                                printf("NOTS\n");
                            }
                        }
                        else if (strcmp(tmpToken.data, ">") == 0) {
                            if(GET_REPEAT_FUNC_RUN())
                                printf("GTS\n");
                        }
                        else if (strcmp(tmpToken.data, ">=") == 0) {
                            if(GET_REPEAT_FUNC_RUN()){
                                printf("LTS\n");
                                printf("NOTS\n");
                            }
                        }
                        else if (strcmp(tmpToken.data, "==") == 0) {
                            if(GET_REPEAT_FUNC_RUN())
                                printf("EQS\n");
                        }
                        else if (strcmp(tmpToken.data, "!=") == 0) {
                            if(GET_REPEAT_FUNC_RUN()){
                                printf("EQS\n");
                                printf("NOTS\n");
                            }
                        }

                    } else if (tmpToken.type == TOKEN_TYPE_RIGHT_BRACKET) {
                    }
                }
                break;
            case N: // err
                if (prevToken.type == TOKEN_TYPE_EOL && actualToken.type == TOKEN_TYPE_EOL) break;

                deleteStack(&tmpStack);
                changeErrorCode(2);
                return;
                break;
            default:
                break;
            }
            if (prevToken.type == TOKEN_TYPE_EOL && actualToken.type == TOKEN_TYPE_EOL) break;

        }
        deleteStack(&tmpStack);
}
