#include "symtable.h"
//typedef enum {INPUT_PARAMS, OUTPUT_PARAMS, EXPRESSION_LEFT_SIDE, EXPRESSION_RIGHT_SIDE} nonterminal;
typedef enum {STATE_NULL, STATE_BEGIN, STATE_FUNC, STATE_FREE, STATE_STRICT_EXPRESSION, 
STATE_NON_STRICT_EXPRESSION, STATE_ERROR,
STATE_INPUT_PARAMS, STATE_OUTPUT_PARAMS, STATE_EXPRESSION_LEFT_SIDE, STATE_EXPRESSION_RIGHT_SIDE} Syntax_State; 
typedef enum {CONDITION_STRICTLY, CONDITION_NON_STRICTLY} Syntax_Condition; 

Token *token;
bool program_start();
bool function_check();
bool input_parameters();
bool input_single_parameters();
bool output_parameters();
bool output_single_parameters();
bool function_body();
bool for_construction();
bool if_construction();
// bool math_expression(int end_condition);
bool logic_expression(int end_condition);
bool expression(int end_condition);
int is_closed_bracket();
bool expression_func_arguments();
bool expression_func_single_argument();


bool expression_including_string(int end_condition);
bool define_func(int end_condition, int declare, int equating, bool func);
bool define_operands();
bool count_operands(int end_condition);
bool start_block_new_line();
bool return_construction();

typedef struct elseStack {
    int deep;
    struct elseStack *next;
} else_stack;

void add_to_else_stack();
void delete_from_else_stack();

else_stack *elseStack;

#define FIRST 1
#define SECOND 2
