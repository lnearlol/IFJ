#include "token.h"
union Types
{
  int value;
  float float_value;
  char *string;
};

typedef struct Variable{
    char *name;
    int type;
    //union Types value;
    int length;
    int deep;
    struct Variable *RPtr;
    struct Variable *LPtr;
    struct Variable *prevTree;
} *variable;


typedef struct outParam{
    int type;
    struct outParam *next;
} *outputParams;

typedef struct inParam{
    char *name;
    int type;
    struct inParam *next;
} *inputParams;

typedef struct Function{
    char *name;
    int type;
    int length;
    struct Function *RPtr;
    struct Function *LPtr;
    inputParams input_params;
    outputParams output_params;
} *function;




typedef struct sym_tab
{
  variable var;//struct sym_tab* func;
  function func;//struct sym_tab* var;
}SymTab;
// SymTab *SymTable = NULL;


SymTab *declaration(SymTab *SymTable);
void insertFunction(Token *token, function *Func);
void freeFunctions(function *Func);
function findFunction(Token *token, function Func);

void addInputArguments(Token *func_name, Token *arg_name, Token *arg_type, function Func);
void addOutputArguments(Token *func_name, Token *arg_type, function Func);


void insertVariable(Token *token, int deepVar, variable *Var);
void freeVariablesLastLabel(variable *Var);
void freeAllVariables(variable *Var);
variable findVariable(Token *token, int deepVar, variable Var);
variable findVariableHelper(Token *token, int deepVar, variable Var);
bool putTypeVariable(Token *token, int deepVar, int varType, variable Var);

//--
variable findVariableWithType(Token *token, int deepVar, variable Var);
variable find_var_with_type_helper(Token *token, int deepVar, variable Var);



int returnLiteralType(Token *token);
int compareTwoVariables(Token *var1, Token *var2, int deep, variable Var);



// P R I N T
void Print_func2(function TempTree, char* sufix, char fromdir);
void Print_func(function TempTree);
void Print_var2(variable TempTree, char* sufix, char fromdir);
void Print_var(variable TempTree);

#define VAR_TYPE_UNDEFINED -1
#define PRINT_PARAMETER 4

