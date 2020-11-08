#include <stdbool.h>
union Types
{
  int value;
  float float_value;
  char *string;
};

typedef struct Variable{
    char *name;
    int type;
    union Types value;
    int length;
    bool used;
    bool declared;
    int deep;
    struct Variable *next;
    struct Variable *other_deep;
} *variable;

typedef struct Function{
    char *name;
    int type;
    int length;
    bool used;
    bool declared;
    struct Function *next;
    struct Function *Tparams;
} *function;

typedef struct params{
    char *type;
    struct params *next;
} *otParams;

typedef struct param{
    char *name;
    char *type;
    struct param *next;
} *itParams;

typedef struct Params{
    itParams input;
    otParams output;
} Tparams;

typedef struct sym_tab
{
  variable var;//struct sym_tab* func;
  function func;//struct sym_tab* var;
}SymTab;
SymTab *S;