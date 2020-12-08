#include "symtable.h"

Token *token;

/**
 * PROGRAM_SRART -> package main eol FUNCTION_CHECK eol eof
 */
bool program_start();

/**
 * FUNCTION_CHECK -> func IDENTIFIER ( ALLOWED_EOL  INPUT_PARAMETERS  OUTPUT_PARAMETERS { eol FUNCTION_BODY } eol
 */
bool function_check();

/**
 * INPUT_PARAMETERS -> )
 * INPUT_PARAMETERS -> INPUT_SINGLE_PARAMETERS
 */
bool input_parameters();

/**
 * INPUT_SINGLE_PARAMETERS -> IDENTIFIER int/float/string , INPUT_SINGLE_PARAMETERS
 * INPUT_SINGLE_PARAMETERS -> IDENTIFIER int/float/string )
 */
bool input_single_parameters();

/**
 * OUTPUT_PARAMETERS -> {
 * OUTPUT_PARAMETERS -> ( )
 * OUTPUT_PARAMETERS -> ( OUTPUT_SINGLE_PARAMETERS
 */
bool output_parameters();

/**
 * OUTPUT_SINGLE_PARAMETERS -> int/float/string , OUTPUT_SINGLE_PARAMETERS
 * OUTPUT_SINGLE_PARAMETERS -> int/float/string ) {
 */
bool output_single_parameters();

/**
 * Function works only on first run of program. Ignore all types of tokens except '}'
 * Function gets next tokens until finds } which will be last in this function (depth will be the same as it was on start)
 */
bool first_run_body();

/**
 * FUNCTION_BODY -> } eol  FUNCTION_BODY
 * FUNCTION_BODY -> { eol  FUNCTION_BODY
 * FUNCTION_BODY -> eol  FUNCTION_BODY
 * FUNCTION_BODY -> if IF_CONSTRUCTION  FUNCTION_BODY
 * FUNCTION_BODY -> else  {  eol  FUNCTION_BODY
 * FUNCTION_BODY -> for FOR_CONSTRUCTION  FUNCTION_BODY
 * FUNCTION_BODY -> IDENTIFIER  DEFINE_FUNC  FUNCTION_BODY
 * FUNCTION_BODY -> COMMAND_FUNCTION  DEFINE_FUNC  FUNCTION_BODY
 * FUNCTION_BODY -> _  DEFINE_FUNC  FUNCTION_BODY
 * FUNCTION_BODY -> return eol  FUNCTION_BODY
 * FUNCTION_BODY -> return  RETURN_CONSTRUCTION FUNCTION_BODY
 */
bool function_body();

/**
 * FOR_CONSTRUCTION ->  DEFINE_FUNC ; EXPRESSION ; DEFINE_FUNC { eol FUNCTION_BODY
 */
bool for_construction();

/**
 * IF_CONSTRUCTION -> EXPRESSION { eol FUNCTION_BODY
 */
bool if_construction();

/**
 * EXPRESSION -> (  ALLOWED_EOL  EXPRESSION
 * EXPRESSION -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  IS_CLOSED_BRACKET
 * EXPRESSION -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  IS_CLOSED_BRACKET  OPERATOR  ALLOWED_EOL  EXPRESSION
 * EXPRESSION -> IDENTIFIER/COMMAND_FUNCTION  IS_CLOSED_BRACKET  (  ALLOWED_EOL  EXPRESSION_FUNC_ARGUMENTS
 * 
 * Function parses syntax of expressions
 * @param end_condition anticipation end condition of the function. If in result current token->type will be the same as end_condition,
 * function will return true, else function will return false.
 */
bool expression(int end_condition);

/**
 * IS_CLOSED_BRACKET -> )  IS_CLOSED_BRACKET
 * IS_CLOSED_BRACKET -> eps
 * 
 * Function counts closed brackets
 * @return counter of closed brackets
 */
int is_closed_bracket();

/**
 * EXPRESSION_FUNC_ARGUMENTS -> )
 * EXPRESSION_FUNC_ARGUMENTS -> EXPRESSION_FUNC_SINGLE_ARGUMENT
 */
bool expression_func_arguments();

/**
 * EXPRESSION_FUNC_SINGLE_ARGUMENT -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  )
 * EXPRESSION_FUNC_SINGLE_ARGUMENT -> IDENTIFIER/LITERAL/COMMAND_FUNCTION  ,  EXPRESSION_FUNC_SINGLE_ARGUMENT
 * 
 * @param arg_check pointer on list of input function arguments (SymTable)
 * @param args_output pointer on list of output function arguments (SymTable)
 */
bool expression_func_single_argument(inputParams args_check, outputParams args_outputFchan);


/**
 * DEFINE_FUNC -> DEFINE_OPERANDS := COUNT_OPERANDS
 * DEFINE_FUNC -> DEFINE_OPERANDS = COUNT_OPERANDS
 * DEFINCE_FUNC -> DEFINE_OPERANDS ( EXPRESSION_FUNC_ARGUMENTS
 * 
 * @param end_condition anticipation end condition of the function. If in result current token->type will be the same as end_condition,
 * function will return true, else function will return false.
 * @param declare is true when declaration is allowed :=
 * @param equating is true when define is allowed =
 * @param func is true when function is allowed
 */
bool define_func(int end_condition, bool declare, bool equating, bool func);

/**
 * DEFINE_OPERANDS -> IDENTIFIER
 * DEFINE_OPERANDS -> IDENTIFIER , DEFINDE_OPERANDS
 * DEFINE_OPERANDS -> COMMAND_FUNCTION
 * DEFINE_OPERANDS -> COMMAND_FUNCTION , DEFINE_OPERANDS
 * DEFINE_OPERANDS -> _
 * DEFINE_OPERANDS -> _ , DEFINE_OPERANDS
 * 
 * @param func is true when function is allowed
 */
bool define_operands(int func);

/**
 * COUNT_OPERANDS -> EXPRESSION
 * COUNT_OPERANDS -> EXPRESSION , COUNT_OPERANDS
 * 
 * function for recursive calling expressions + count of operands
 * @param end_condition anticipation end condition of the function. If in result current token->type will be the same as end_condition,
 * function will return true, else function will return false.
 */
bool count_operands(int end_condition);

/**
 * START_BLOCK_NEW_LINE -> { eol
 */
bool start_block_new_line();

/**
 * RETURN_CONSTRUCTION -> eol
 * RETURN_CONSTRUCTION -> EXPRESSION  eol
 * RETURN_CONSTRUCTION -> EXPRESSION  ,  RETURN_CONSTRUCTION
 * 
 * @param out_params pointer on list of output function arguments (SymTable)
 */
bool return_construction();

/**
 * ALLOWED_EOL -> eol  ALLOWED_EOL
 * ALLOWED_EOL -> eps
 */
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

/**
 * Function adds one instanse of some stack and allocates memory for it
 * @param changeStack pointer on some kind of stack
 * @param deep depth of label
 */
void add_to_for_if_stack(else_stack *changeStack, int deep);

/**
 * Function frees one instanse of some stack
 * @param changeStack pointer on some kind of stack
 */
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

/**
 * Function adds variable to compare list and allocates memory for it
 * @param var Token contains name of variable
 */
void add_var_to_compare_list(Token *var);

/**
 * Function adds type to compare list and allocates memory for it
 * @param type Token contains name of variable
 */
void add_type_to_compare_list(int type);

/**
 * Function frees last variable from compare list
 */
void delete_var_from_compare_list();

/**
 * Function frees last type from compare list
 */
void delete_type_from_compare_list();

/**
 * Function frees typeCompareList and varCompareList
 */
void freeBothCompareLists();



variables_compare_list *varCompareList;
type_compare_list *typeCompareList;


Token *current_function_name;

/**
 * Function compares types of variables with names which are written to varCompareList and types from typeCompareList 
 * and frees that lists
 * 
 * @param deep depth of label
 * @return Function returns true if all types are the same, and returns false if at least only one type will be different
 */
bool check_define_logic(int deep);

/**
 * Function creats variables in SymTable with names which are written to varCompareList and give them types from typeCompareList 
 * and frees that lists
 * 
 * @param deep depth of label
 * @return Function returns true if all expression types are successfully written to variable->type in symtable. And
 * returns false if there was any syntax or semantic error
 */
bool check_declare_logic(int deep);


// A S E M B L Y

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