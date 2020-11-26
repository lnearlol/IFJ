#include "expression-translator.h"

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

void deleteStack(Stack_t **stack) {
        if(*stack != NULL) {
                free((*stack)->data);
                free(*stack);
                *stack = NULL;
        }
}


void resize(Stack_t *stack) {
    stack->size *= MULTIPLIER;
    stack->data = realloc(stack->data, stack->size * sizeof(Token));
    if (stack->data == NULL) {
        exit(STACK_OVERFLOW);
    }
}


void push(Stack_t *stack, Token value) {
    if (stack->top >= stack->size) {
        resize(stack);
    }
    stack->data[stack->top] = value;
    stack->top++;
}

Token pop(Stack_t *stack) {
    if (stack->top == 0) {
        exit(STACK_UNDERFLOW);
    }
    stack->top--;
    return stack->data[stack->top];
}
Token peek(const Stack_t *stack) {
    if (stack->top <= 0) {
        exit(STACK_UNDERFLOW);
    }
    return stack->data[stack->top - 1];
}


void implode(Stack_t *stack) {
    stack->size = stack->top;
    stack->data = realloc(stack->data, stack->size * sizeof(Token));
}

int prec(Token token) {
    switch (token.type) {
        case TOKEN_TYPE_MATH_OPERATOR:
                if (strcmp(token.data, "/") == 0 || strcmp(token.data, "*") == 0)
                        return 4;
                else if (strcmp(token.data, "+") == 0 || strcmp(token.data, "-") == 0)
                        return 3;
                break;
        case TOKEN_TYPE_LEFT_BRACKET:
                return 2;
        case TOKEN_TYPE_LOGICAL_OPERATOR:
                return 1;
                break;
        }
}

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

        if (result != -1) generateCode(res, deepVar, result);

        deleteStack(&res);
        return result;

}

void generateCode(Stack_t *stack, int deepVar, int type) {
        for (int i = 0; i < stack->top - 1; i++) {
                 Token token = stack->data[i];
                if (token.type == TOKEN_TYPE_LITERAL_FLOAT ||
                    token.type == TOKEN_TYPE_LITERAL_INT ||
                    token.type == TOKEN_TYPE_IDENTIFIER ||
                    token.type == TOKEN_TYPE_LITERAL_STRING) {
                            printf("PUSHS ");
                            GEN_WRITE_VAR_LITERAL(&token, deepVar);
                            printf("\n");
                } else if (token.type == TOKEN_TYPE_MATH_OPERATOR) {
                        if (strcmp(token.data, "/") == 0) {
                            if(type == 1) {
                                printf("IDIVS\n");
                            }
                            else if (type == 2) {
                                printf("DIVS\n");
                            }
                        }
                        else if (strcmp(token.data, "*") == 0) {
                            printf("MULS\n");
                        }
                        else if (strcmp(token.data, "+") == 0) {
                            printf("ADDS\n");
                        }
                        else if (strcmp(token.data, "-") == 0) {
                            printf("SUBS\n");
                        }

                }
                else if (token.type == TOKEN_TYPE_LOGICAL_OPERATOR) {
                        if (strcmp(token.data, "<") == 0) {
                            printf("LTS\n");
                        }
                        else if (strcmp(token.data, "<=") == 0) {
                            printf("GTS\n");
                            printf("NOTS\n");
                        }
                        else if (strcmp(token.data, ">") == 0) {
                            printf("GTS\n");
                        }
                        else if (strcmp(token.data, ">=") == 0) {
                            printf("LTS\n");
                            printf("NOTS\n");
                        }
                        else if (strcmp(token.data, "==") == 0) {
                            printf("EQS\n");
                        }
                        else if (strcmp(token.data, "!=") == 0) {
                            printf("EQS\n");
                            printf("NOTS\n");
                        }
                }
                else if (token.type == TOKEN_TYPE_EOL) {
                        break;
                }
                else break;
        } printf("\n");
}
