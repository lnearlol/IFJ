#include "parser.h"

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
