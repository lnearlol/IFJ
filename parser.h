#include "token.h"
#include <stdbool.h> 
bool errflg = false;
//typedef enum {INPUT_PARAMS, OUTPUT_PARAMS, EXPRESSION_LEFT_SIDE, EXPRESSION_RIGHT_SIDE} nonterminal;
typedef enum {STATE_NULL, STATE_BEGIN, STATE_FUNC, STATE_FREE, STATE_STRICT_EXPRESSION, 
STATE_NON_STRICT_EXPRESSION, STATE_ERROR,
STATE_INPUT_PARAMS, STATE_OUTPUT_PARAMS, STATE_EXPRESSION_LEFT_SIDE, STATE_EXPRESSION_RIGHT_SIDE} Syntax_State; 
typedef enum {CONDITION_STRICTLY, CONDITION_NON_STRICTLY} Syntax_Condition; 

typedef struct Stack_Next {
    Token_type token_type;
    struct Stack_Next *next;
} *stack_next;

typedef struct {
    stack_next top;
} Syntax_Stack;


Syntax_Stack STACK; 

void allocating_error();
void delete_from_stack(Syntax_Stack *S);
void add_to_stack(Syntax_Stack *S, int type);
void create_new_stack(Syntax_Stack *S);
int structure_check(Token *token);