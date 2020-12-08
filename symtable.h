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
 * @struct Symbol table for functions representation.
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
 * @struct Symbol table for variables representation in the code generator
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

/**
 * Function inserts new variable from function into symbol table for generation
 * @param token Token that contains information about a variable
 * @param deep Signifies the deep of the variable
 * @param genVar Pointer to the symbol table for variables
 * @return Function true in a successful insertion
 */
void insertGenVariable(Token *token, int deep, genVariable *genVar);

// Function deletes all variables into symbol table for generation
void freeGenVariables(genVariable *genVar, bool print);

// Helper function for "insertGenVariable"
void insertDepth(int deep, deepInside *Depth);

/**
 * Function allocates a new symbol table
 * @param SymTable Symbol table to be initialized
 * @return Function returns pointer to the new symbol table
 */
SymTab *declaration(SymTab *SymTable);

/**
 * Function inserts new function in the symbol table
 * @param token Token that contains information about a function
 * @param Func Pointer to the symbol table for functions
 */
void insertFunction(Token *token, function *Func);

// Function deletes all functions with all parameters into symbol table 
void freeFunctions(function *Func);

/**
 * Function finds for a function according to its name in the symbol table  
 * @param token Token that contains information about a function
 * @param Func Signifies the name of function 
 */
function findFunction(Token *token, function Func);

/**
 * Function adds input arguments for a function according to its name in the symbol table  
 * @param func_name Token that contains information about a function
 * @param arg_name Signifies the name of argument 
 * @param arg_type Signifies the type of argument 
 * @param Func Signifies function 
 */
void addInputArguments(Token *func_name, Token *arg_name, Token *arg_type, function Func);

/**
 * Function adds output arguments for a function according to its name in the symbol table  
 * @param func_name Token that contains information about a function
 * @param arg_type Signifies the type of argument 
 * @param Func Signifies function 
 */
void addOutputArguments(Token *func_name, Token *arg_type, function Func);

/**
 * Function inserts new variable in the symbol table.
 * @param token Token that contains information about a variable
 * @param deepVar Signifies the deep of the variable
 * @param Var Pointer to the symbol table for variables
 * @return Function true in a successful insertion
 */
bool insertVariable(Token *token, int deepVar, variable *Var);

/**
 * Function deletes the last appereance of the variable in the symbol table
 * @param Var Signifies variable
 */
void freeVariablesLastLabel(variable *Var);

// Function deletes all variables into symbol table 
void freeAllVariables(variable *Var);

/**
 * Function find variable with the same name on maximum possible level
 * @param token Token that contains information about a variable
 * @param deepVar Signifies the deep of the variable
 * @param Var Signifies variable
 */
variable findVariable(Token *token, int deepVar, variable Var);

// Helper function for "findVariable"
variable findVariableHelper(Token *token, int deepVar, variable Var);

/**
 * Function put the type of a variable according to its name and deep
 * @param token Token that contains information about a variable
 * @param deepVar Signifies the deep of the variable
 * @param varType Signifies the type of the variable
 * @param Var Signifies variable
 */
bool putTypeVariable(Token *token, int deepVar, int varType, variable Var);

//--
/**
 * Function find variable with the type on maximum possible level
 * @param token Token that contains information about a variable
 * @param deepVar Signifies the deep of the variable
 * @param Var Signifies variable
 */
//variable findVariableWithType(Token *token, int deepVar, variable Var);

// Helper function for "findVariableWithType" with the same parametеrs
variable find_var_with_type_helper(Token *token, int deepVar, variable Var);



// Helper function for "compareTwoVariables"
int returnLiteralType(Token *token);

/**
 * Function compare two variables. Searching for a variable by name and deер
 * @param var1 Token that contains information about a variable
 * @param var2 Type of the second variable
 * @param deepVar Signifies the deep of the variable
 * @param Var Signifies variable
 * @return Сorrect type
 */
int compareTwoVariables(Token *var1, int var2, int deep, variable Var);

void createTokens(SymTab *SymTable);

/**
 * Function adds inbuilt functions to the  symbol table when it is called
 * @param SymTable Token that contains information about a function
 */
void symTab_for_inbuilt_func(SymTab *S);

Token *create_and_set_token(Token *helper);

// Create token for function prints
Token *get_print_token(Token *helper);

// Create token for function inputs
Token *get_inputs_token(Token *helper);

// Create token for function inputi
Token *get_inputi_token(Token *helper);

// Create token for function inputf
Token *get_inputf_token(Token *helper);

// Create token for function int2float
Token *get_int2float_token(Token *helper);

// Create token for function float2int
Token *get_float2int_token(Token *helper);

// Create token for function substr
Token *get_substr_token(Token *helper);

// Create token for function ord
Token *get_ord_token(Token *helper);

// Create token for function chr
Token *get_chr_token(Token *helper);

// Create token for function len
Token *get_len_token(Token *helper);

// Create a token for the "int" type
Token *get_int_token(Token *helper);

// Create a token for the "float" type
Token *get_float_token(Token *helper);

// Create a token for the "string" type
Token *get_string_token(Token *helper);

// Create a type token for the "print" function
Token *get_print_type_token(Token *helper);

// Create a token for the "i" name
Token *get_i_token(Token *helper);

// Create a token for the "f" name
Token *get_f_token(Token *helper);

// Create a token for the "s" name
Token *get_s_token(Token *helper);

// Create a token for the "n" name
Token *get_n_token(Token *helper);


#define VAR_TYPE_UNDEFINED -1
#define TOKEN_TYPE_PRINT 99

