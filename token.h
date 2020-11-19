#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *program_code;

typedef enum {
TOKEN_TYPE_IDENTIFIER,
//DATA_TYPE,
TOKEN_TYPE_INT,
TOKEN_TYPE_FLOAT,
TOKEN_TYPE_STRING,
//COMMAND_WORD,
TOKEN_TYPE_FOR,
TOKEN_TYPE_IF,
TOKEN_TYPE_ELSE,
TOKEN_TYPE_FUNC,
TOKEN_TYPE_PACKAGE,
TOKEN_TYPE_RETURN,

TOKEN_TYPE_COMMAND_FUNCTION,
TOKEN_TYPE_LITERAL_INT,
TOKEN_TYPE_LITERAL_FLOAT,
TOKEN_TYPE_LITERAL_STRING,
TOKEN_TYPE_LOGICAL_OPERATOR,
TOKEN_TYPE_DECLARE /*':='*/,
TOKEN_TYPE_EQUATING /*'='*/,

TOKEN_TYPE_UNDERSCORE,
TOKEN_TYPE_MATH_OPERATOR,
TOKEN_TYPE_START_BLOCK,
TOKEN_TYPE_END_BLOCK,
TOKEN_TYPE_LEFT_BRACKET,
TOKEN_TYPE_RIGHT_BRACKET,
TOKEN_TYPE_COMMA,
TOKEN_TYPE_SEMICOLON,
TOKEN_TYPE_EOL,
TOKEN_TYPE_EOFILE} Token_type; 

typedef struct token {
    int type;
    char *data;
    int size;
    struct token *next;
} Token;

int get_token(Token *token);       //function of processing a program code
void dtor(Token *first);            //release the memory allocated for tokens