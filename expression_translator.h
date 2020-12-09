/**
 * @file expression_translator.c
 *
 * @brief Header file for expression_translator.c for processing expressions
 *
 * IFJ Projekt 2020, Tým 55
 *
 * @author <xpastu04> Pastushenko Vladislav
 */

#include "parser.h"

/**
 * Constants for working with stack of tokens
 */
#define INIT_SIZE 10
#define MULTIPLIER 1.5
#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

void     resize          (Stack_t *stack);
Token    pop             (Stack_t *stack);
Token    peek            (const Stack_t *stack);
void     implode         (Stack_t *stack);

void     generateCode    (Stack_t *stack, int deepVar, variable Var, int incomingType);