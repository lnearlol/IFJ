#include "token.h"
#include <stdbool.h> 
bool errflg = false;
//typedef enum {INPUT_PARAMS, OUTPUT_PARAMS, EXPRESSION_LEFT_SIDE, EXPRESSION_RIGHT_SIDE} nonterminal;
typedef enum {STATE_NULL, STATE_BEGIN, STATE_FUNC, STATE_FREE, STATE_STRICT_EXPRESSION, 
STATE_NON_STRICT_EXPRESSION, STATE_ERROR,
STATE_INPUT_PARAMS, STATE_OUTPUT_PARAMS, STATE_EXPRESSION_LEFT_SIDE, STATE_EXPRESSION_RIGHT_SIDE} Syntax_State; 
typedef enum {CONDITION_STRICTLY, CONDITION_NON_STRICTLY} Syntax_Condition; 

Token *token;
bool program_start();
bool function();
bool input_parameters();
bool input_single_parameters();
bool output_parameters();
bool output_single_parameters();
bool function_body();
bool for_construction();
bool math_expression(int end_condition);
bool logic_expression(int end_condition);
bool expression(int end_condition);