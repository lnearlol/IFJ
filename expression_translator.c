/**
 * @file expression_translator.c
 *
 * @brief Functions for processing expressions
 *
 * IFJ Projekt 2020, Tým 55
 *
 * @author <xstepa64> Stepaniuk Roman, Bc.
 * @author <xpastu04> Pastushenko Vladislav
 * @author <xbahda01> Bahdanovich Viktoryia
 * @author <xtomas34> Tomason Viktoryia
 */

#include "expression_translator.h"


/**
 * Function for creating stack of tokens.
 * @return New empty stack
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

/**
 * Check token and return its priority
 * @param Token Verifiable token
 * @return An integer representing the priority number
 */
int prec(Token token) {
    switch (token.type) {
        case TOKEN_TYPE_MATH_OPERATOR:
                if (strcmp(token.data, "/") == 0 || strcmp(token.data, "*") == 0)
                        return 4;
                else if (strcmp(token.data, "+") == 0 || strcmp(token.data, "-") == 0)
                        return 3;
                break;
        case TOKEN_TYPE_LEFT_BRACKET:
                return 1;
                break;
        case TOKEN_TYPE_LOGICAL_OPERATOR:
                return 2;
                break;
        }
}


/**
 * Take stack of tokens, which contains expression, and create new stack of tokens,
 * which will contain same expression but in postfix form.
 * After that, it runs the generateCode function with new stack.
 * Also gets and returns type of expression. Returns -1 if any error and sets error flags.
 * @param stack Stack of tokens with expression
 * @param deepVar Signifies the deep of the variable
 * @param Var Pointer to the symbol table for variables
 * @return An integer representing the expression type
 */
int sort_to_postfix(Stack_t *stack, int deepVar, variable Var) {

        if (stack->top == 0) {
                deleteStack(&stack);
                return 0;
        }

        Stack_t* opstack = createStack();
        Stack_t* res = createStack();

        implode(stack);
        int result = 0;
        bool logical_type_flag = false;
        for (int i = 0; i < stack->size; i++) {
                Token token = stack->data[i];
                if (token.type == TOKEN_TYPE_LITERAL_FLOAT ||
                    token.type == TOKEN_TYPE_LITERAL_INT ||
                    token.type == TOKEN_TYPE_IDENTIFIER ||
                    token.type == TOKEN_TYPE_LITERAL_STRING) {

                        push(res, token);

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
                } else if (token.type == TOKEN_TYPE_LEFT_BRACKET) {
                        push(opstack, token);
                } else if (token.type == TOKEN_TYPE_RIGHT_BRACKET) {
                        for (Token poped_token = pop(opstack); poped_token.type != TOKEN_TYPE_LEFT_BRACKET; poped_token = pop(opstack)) {
                                if (opstack->top == 0) {
                                    result = -1;
                                    changeErrorCode(2);
                                    break;
                                }
                                push(res, poped_token);
                        }
                        if (result == -1) break;
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
                        while (opstack->top > 0 && prec(peek(opstack)) >= prec(token)) {
                                if (prec(peek(opstack)) >= prec(token)) {
                                        push(res, pop(opstack));
                                }
                        }
                        push(opstack, token);
                } else if (token.type == TOKEN_TYPE_EOL) {
                        break;
                }
                else break;
        }

        if (result != -1) {
                if (logical_type_flag)
                    result = 4;

                for (int i = 0; opstack->top > 0; i++) {
                        if(opstack->data[i].type == TOKEN_TYPE_LEFT_BRACKET || opstack->data[i].type == TOKEN_TYPE_RIGHT_BRACKET){
                                deleteStack(&opstack);
                                deleteStack(&stack);
                                deleteStack(&res);
                                changeErrorCode(2);
                                return -1;
                        }
                        push(res, pop(opstack));
                }
                push(res, pop(stack));
        }

        deleteStack(&opstack);
        deleteStack(&stack);

        if (result != -1) generateCode(res, deepVar, Var, result);

        deleteStack(&res);
        return result;

}

/**
 * Prints assembly commands used to process expressions.
 * @param stack Stack of tokens with expression in postfix form
 * @param deepVar Signifies the deep of the variable
 * @param Var Pointer to the symbol table for variables
 * @param incomingType An integer representing the expression type
 */
void generateCode(Stack_t *stack, int deepVar, variable Var, int incomingType) {
        int type = 0;
        for (int i = 0; i < stack->top - 1; i++) {
                Token token = stack->data[i];
                if (type == 0) {
                    if (token.type == TOKEN_TYPE_IDENTIFIER)
                        type = findVariableWithType(&token, deepVar, Var)->type;
                    else
                        type = returnLiteralType(&token);
                }
                if (token.type == TOKEN_TYPE_LITERAL_FLOAT ||
                    token.type == TOKEN_TYPE_LITERAL_INT ||
                    token.type == TOKEN_TYPE_IDENTIFIER ||
                    token.type == TOKEN_TYPE_LITERAL_STRING) {
                        if(GET_REPEAT_FUNC_RUN()){
                            printf("PUSHS ");
                            GEN_WRITE_VAR_LITERAL(&token, deepVar);
                            printf("\n");
                        }
                } else if (token.type == TOKEN_TYPE_MATH_OPERATOR) {
                    if (type != 3) {
                        if (strcmp(token.data, "/") == 0) {
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
                        else if (strcmp(token.data, "*") == 0) {
                            if(GET_REPEAT_FUNC_RUN())
                                printf("MULS\n");
                        }
                        else if (strcmp(token.data, "+") == 0) {
                            if(GET_REPEAT_FUNC_RUN())
                                printf("ADDS\n");
                        }
                        else if (strcmp(token.data, "-") == 0) {
                            if(GET_REPEAT_FUNC_RUN())
                                printf("SUBS\n");
                        }

                    }
                    else {
                        if (strcmp(token.data, "+") == 0) {
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
                else if (token.type == TOKEN_TYPE_LOGICAL_OPERATOR) {
                    if (strcmp(token.data, "<") == 0) {
                        if(GET_REPEAT_FUNC_RUN()){
                            printf("LTS\n");
                        }
                    }
                    else if (strcmp(token.data, "<=") == 0) {
                        if(GET_REPEAT_FUNC_RUN()){
                            printf("GTS\n");
                            printf("NOTS\n");
                        }
                    }
                    else if (strcmp(token.data, ">") == 0) {
                        if(GET_REPEAT_FUNC_RUN())
                            printf("GTS\n");
                    }
                    else if (strcmp(token.data, ">=") == 0) {
                        if(GET_REPEAT_FUNC_RUN()){
                            printf("LTS\n");
                            printf("NOTS\n");
                        }
                    }
                    else if (strcmp(token.data, "==") == 0) {
                        if(GET_REPEAT_FUNC_RUN())
                            printf("EQS\n");
                    }
                    else if (strcmp(token.data, "!=") == 0) {
                        if(GET_REPEAT_FUNC_RUN()){
                            printf("EQS\n");
                            printf("NOTS\n");
                        }
                    }

                }
                else if (token.type == TOKEN_TYPE_EOL) {
                        break;
                }
                else break;
        }
        if(GET_REPEAT_FUNC_RUN())
            printf("\n");
}
