#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
TOKEN_TYPE_IDENTIFIER,  // 0
                    //DATA_TYPE,
TOKEN_TYPE_INT,  // 1
TOKEN_TYPE_FLOAT,  // 2
TOKEN_TYPE_STRING,  // 3
                    //COMMAND_WORD,
TOKEN_TYPE_FOR,  // 4
TOKEN_TYPE_IF,  // 5
TOKEN_TYPE_ELSE,  // 6
TOKEN_TYPE_FUNC,  // 7
TOKEN_TYPE_PACKAGE,  // 8
TOKEN_TYPE_RETURN,  // 9

TOKEN_TYPE_COMMAND_FUNCTION,  // 10
TOKEN_TYPE_LITERAL_INT,  // 11
TOKEN_TYPE_LITERAL_FLOAT,  // 12
TOKEN_TYPE_LITERAL_STRING,  // 13
TOKEN_TYPE_LOGICAL_OPERATOR,  // 14
TOKEN_TYPE_DECLARE /*':='*/,  // 15


TOKEN_TYPE_UNDERSCORE,  // 16
TOKEN_TYPE_EQUATING /*'='*/,  // 17
TOKEN_TYPE_MATH_OPERATOR,  // 18
TOKEN_TYPE_START_BLOCK,  // 19
TOKEN_TYPE_END_BLOCK,  // 20
TOKEN_TYPE_LEFT_BRACKET,  // 21
TOKEN_TYPE_RIGHT_BRACKET,  // 22
TOKEN_TYPE_COMMA,  // 23
TOKEN_TYPE_SEMICOLON,  // 24
TOKEN_TYPE_EOL,  // 25
TOKEN_TYPE_EOFILE  // 26
} Token_type;

typedef struct token {
    int type;
    char *data;
    int size;
    struct token *next;
} Token;

int get_token(Token *token);       //function of processing a program code
//release the allocated memory created with malloc()
void dtor(Token *first);
void changeErrorCode(int code);



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

variable findVariableWithType(Token *token, int deepVar, variable Var);




// EXPRESSION TRANLSATION
typedef struct Stack_tag {
    Token *data;
    size_t size;
    size_t top;
} Stack_t;
Stack_t *expr;


Stack_t* createStack();
void     deleteStack     (Stack_t **stack);
void     resize          (Stack_t *stack);
void     push            (Stack_t *stack, Token value);
Token    pop             (Stack_t *stack);
Token    peek            (const Stack_t *stack);
void     implode         (Stack_t *stack);
int      sort_to_postfix (Stack_t *stack, int deepVar, variable Var);
void     makeBT          (Stack_t *stack);

#define INIT_SIZE_BT_STACK        10
#define MULTIPLIER_BT_STACK       1.5
#define BT_STACK_OVERFLOW         -100
#define BT_STACK_UNDERFLOW        -101
#define OUT_OF_MEMORY_BT_STACK    -102

typedef struct BTNode	{
    Token token;
    struct BTNode * LPtr;
    struct BTNode * RPtr;
} *BTNodePtr;


void BTInit (BTNodePtr *RootPtr);
void setBTData (BTNodePtr *RootPtr, Token token);
void setBTLeftChild (BTNodePtr *RootPtr, BTNodePtr *ChildPtr);
void setBTRightChild (BTNodePtr *RootPtr, BTNodePtr *ChildPtr);
void Print_tree(BTNodePtr TempTree);
void Print_tree2(BTNodePtr TempTree, char* sufix, char fromdir);
void BTDelete (BTNodePtr *RootPtr);
typedef struct BTStack_tag {
    BTNodePtr *data;
    size_t size;
    size_t top;
} BTStack_t;

BTStack_t* createBTStack();
void       deleteBTStack          (BTStack_t **stack);
void       resizeBTStack          (BTStack_t *stack);
void       pushBTStack            (BTStack_t *stack, BTNodePtr value);
BTNodePtr  popBTStack             (BTStack_t *stack);
BTNodePtr  peekBTStack            (const BTStack_t *stack);
void       BTStackimplode         (BTStack_t *stack);

FILE *program_code;