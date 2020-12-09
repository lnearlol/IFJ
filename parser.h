/**
 * @file parser.h
 * 
 * @brief Parser header file
 * 
 * IFJ Projekt 2020, Tým 55
 * 
 * @author <xstepa64> Stepaniuk Roman, Bc.
*/


#include "symtable.h"

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
bool expression(int end_condition);
int is_closed_bracket();
bool expression_func_arguments();
bool expression_func_single_argument(inputParams args_check, outputParams args_outputFchan);
bool define_func(int end_condition, bool declare, bool equating, bool func);
bool define_operands(int func);
bool count_operands(int end_condition);
bool start_block_new_line();
bool return_construction();
void allowed_eol();

/**
 * @struct Stack representation
 */
typedef struct elseStack {
    int deep;
    struct elseStack *next;
} *else_stack;

/**
 * @struct Container representation
 */
typedef struct stackContainer{
    else_stack elseStack;
    else_stack forStack;
    else_stack endElseStack;
    else_stack jumpForStack;
    else_stack jumpIfStack;
    else_stack jumpElseStack;
} stackContainer;


stackContainer *declareContainer(stackContainer *myContainer);
stackContainer *Container;

void add_to_for_if_stack(else_stack *changeStack, int deep);
void delete_from_for_if_stack(else_stack *changeStack);

// FIRST RUN OF THE PROGRAM
#define FIRST_RUN 1
// SECOND RUN OF THE PROGRAM
#define SECOND_RUN 0
SymTab *SymTable;
Token *saved_func_name, *saved_arg_name, *saved_arg_type;

/**
 * @struct Compare list filled by names of variables
 */
typedef struct variablesCompareList{
    Token *var;
    struct variablesCompareList *next;
} variables_compare_list;

/**
 * @struct Compare list filled by type numbers of expressions
 */
typedef struct typeCompareList{
    int type;
    struct typeCompareList *next;
} type_compare_list;

// C O M P A R E   L I S T S

void add_var_to_compare_list(Token *var);
void add_type_to_compare_list(int type);
void delete_var_from_compare_list();
void delete_type_from_compare_list();
void freeBothCompareLists();



variables_compare_list *varCompareList;
type_compare_list *typeCompareList;


Token *current_function_name;

bool check_define_logic(int deep);
bool check_declare_logic(int deep);


// I F J c o d e 2 0

bool GET_REPEAT_FUNC_RUN();


void GEN_START_OF_FUNCTION(Token *token);
void GEN_END_OF_FUNCTION(Token *token);
void GEN_START_MAIN();
void GEN_CALL(Token *token);
void GEN_CREATE_FRAME_AND_SET_PARAMS(inputParams InParams);
void MOVE_INTO_INPUT_PARAMETER(inputParams InParam, Token *value, int deep);
void GEN_LEN();

void GEN_WRITE_VAR_LITERAL(Token *token, int deep);
void GEN_SET_FRAME_TYPE();
void GEN_RETVAL_CREATER(outputParams outPut);
void GEN_RETVAL_RETURN(outputParams outPut);


/**
 * @struct stack with tokens names of variables for generating IFJcode20
 */
typedef struct var_assembly_stack{
    Token *token_stack;
    struct var_assembly_stack *next;
} var_assembly_stack;

var_assembly_stack *varStack;
void GEN_ADD_VAR_TO_ASSEMBLY_STACK(Token *stack);
void GEN_DELETE_VAR_FROM_ASSEMBLY_STACK();
void GEN_DELETE_FULL_VAR_ASSEMBLY_STACK();
void GEN_CREATE_LEFT_SIDE(int deep);
void GEN_EQ_LEFT_SIDE(int deep);
char* ASM_DATA_APPEND(char* ASM_string, char state, int length);
char *GEN_ASM_STRING(Token *token, char* ASM_string);

void GEN_PRINT_WRITE(Token *token, int deep);

// ---------------- I F  -  F O R

void GEN_JUMP(Token *token, int count, bool if_condition, int for_condition);
void GEN_SCOPE(Token *token, int count, bool if_condition, bool for_condition);

#define NO_FOR 0
#define FOR_JUMP_EQ 1
#define FOR_JUMP 2


// ----------------   I N B U I L D   F U N C T I O N S

void GEN_CALL_INBUILDS();

void GENERATION_INPUTS();
void GENERATION_INPUTI();
void GENERATION_INPUTF();
void GENERATION_LEN();
void GENERATION_FLOAT2INT();
void GENERATION_INT2FLOAT();
void GENERATION_SUBSTR();
void GENERATION_ORD();
void GENERATION_CHR();