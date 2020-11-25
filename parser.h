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
bool first_run_body();
bool function_body();
bool for_construction();
bool if_construction();
// bool math_expression(int end_condition);
bool logic_expression(int end_condition);
bool expression(int end_condition);
int is_closed_bracket();
bool expression_func_arguments();
bool expression_func_single_argument(inputParams args_check, outputParams args_outputFchan);


bool expression_including_string(int end_condition);
bool define_func(int end_condition, int declare, int equating, bool func);
bool define_operands(int func);
bool count_operands(int end_condition);
bool start_block_new_line();
bool return_construction();


void allowed_eol();

typedef struct elseStack {
    int deep;
    struct elseStack *next;
} else_stack;

void add_to_else_stack();
void delete_from_else_stack();

else_stack *elseStack;
else_stack *forStack;

#define FIRST_RUN 1
#define SECOND_RUN 0
SymTab *SymTable;
Token *saved_func_name, *saved_arg_name, *saved_arg_type;

typedef struct variablesCompareList{
    Token *var;
    struct variablesCompareList *next;
} variables_compare_list;

typedef struct typeCompareList{
    int type;
    struct typeCompareList *next;
} type_compare_list;

// C O M P A R E   L I S T
void add_var_to_compare_list(Token *var);
void add_type_to_compare_list(int type);

void delete_var_from_compare_list();
void delete_type_from_compare_list();
void freeBothCompareLists();
bool checkCompareLists();

// void changeErrorCode(int code);
// bool checkOneCompareList(type_compare_list *typeList);


variables_compare_list *varCompareList;
type_compare_list *typeCompareList;
type_compare_list *retCompareList;

#define RETURN_TYPE 77




Token *current_function_name;


bool check_define_logic(int deep);
bool check_declare_logic(int deep);
bool compare_return_and_output_params_logic();


// A S E M B L Y

void START_OF_FUNCTION(char *id_of_function);
void END_OF_FUNCTION(char *id_of_function);
void START_MAIN();
void END_OF_MAIN();
void CALL(char *id_of_function);
void GENERATION_LEN();