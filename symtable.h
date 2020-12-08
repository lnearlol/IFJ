#include "token.h"

Token *startCommandFuncList;//first
union Types
{
  int value;
  float float_value;
  char *string;
};
/**
 * @struct Structure for output function parameters
 */
typedef struct outParam{
    int type;
    struct outParam *next;
} *outputParams;

/**
 * @struct Structure for input function parameters
 */
typedef struct inParam{
    char *name;
    int type;
    struct inParam *next;
} *inputParams;

/**
 * @struct Functions tree in the symbol table
 */
typedef struct Function{
    char *name;
    int type;
    int length;
    struct Function *RPtr;
    struct Function *LPtr;
    inputParams input_params;
    outputParams output_params;
} *function;


typedef struct DeepInside{
  int depthValue;
  struct DeepInside *next;
} *deepInside;

/**
 * @struct Variables tree in the symbol table for code generator
 */
typedef struct GenVariable{
    char *name;
    struct GenVariable *RPtr;
    struct GenVariable *LPtr;
    deepInside depth;
} *genVariable;


/**
 * @struct Symbol table representation
 */
typedef struct sym_tab
{
  variable var;//struct sym_tab* func;
  function func;//struct sym_tab* var;
  genVariable genVar;
}SymTab;


void insertGenVariable(Token *token, int deep, genVariable *genVar);
void freeGenVariables(genVariable *genVar, bool print);
void insertDepth(int deep, deepInside *Depth);

SymTab *declaration(SymTab *SymTable);
void insertFunction(Token *token, function *Func);
void freeFunctions(function *Func);
function findFunction(Token *token, function Func);
void addInputArguments(Token *func_name, Token *arg_name, Token *arg_type, function Func);
void addOutputArguments(Token *func_name, Token *arg_type, function Func);

bool insertVariable(Token *token, int deepVar, variable *Var);
void freeVariablesLastLabel(variable *Var);
void freeAllVariables(variable *Var);
variable findVariable(Token *token, int deepVar, variable Var);
variable findVariableHelper(Token *token, int deepVar, variable Var);
bool putTypeVariable(Token *token, int deepVar, int varType, variable Var);
//--
//variable findVariableWithType(Token *token, int deepVar, variable Var);
variable find_var_with_type_helper(Token *token, int deepVar, variable Var);
int returnLiteralType(Token *token);
int compareTwoVariables(Token *var1, int var2, int deep, variable Var);

void createTokens(SymTab *SymTable);
void symTab_for_inbuilt_func(SymTab *S);
Token *create_and_set_token(Token *helper);
Token *get_print_token(Token *helper);
Token *get_inputs_token(Token *helper);
Token *get_inputi_token(Token *helper);
Token *get_inputf_token(Token *helper);
Token *get_int2float_token(Token *helper);
Token *get_float2int_token(Token *helper);
Token *get_substr_token(Token *helper);
Token *get_ord_token(Token *helper);
Token *get_chr_token(Token *helper);
Token *get_len_token(Token *helper);
Token *get_int_token(Token *helper);
Token *get_float_token(Token *helper);
Token *get_string_token(Token *helper);
Token *get_print_type_token(Token *helper);
Token *get_i_token(Token *helper);
Token *get_f_token(Token *helper);
Token *get_s_token(Token *helper);
Token *get_n_token(Token *helper);


#define VAR_TYPE_UNDEFINED -1
#define TOKEN_TYPE_PRINT 99

