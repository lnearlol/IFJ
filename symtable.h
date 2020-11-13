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
    struct Variable *RtPtr;
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

void insert_var(Token *token, int deeps, SymTab *SymTable);
bool find_var_in_sym_test(Token *token,int deeps, SymTab*SymTable);
void delete_level(Token *token, SymTab *SymTable);
SymTab *declaration(SymTab *SymTable);
void insertFunction(Token *token, function *Func);
void freeFunctions(function *Func);
function findFunction(Token *token, function Func);

void addInputArguments(Token *func_name, Token *arg_name, Token *arg_type, function Func);
void addOutputArguments(Token *func_name, Token *arg_type, function Func);

  
// P R I N T
void Print_func2(function TempTree, char* sufix, char fromdir);
void Print_func(function TempTree);