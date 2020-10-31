#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void get_token(Token *token);       //function of processing a program code
//release the allocated memory created with malloc()
void dctor(Token *first);

FILE *program_code;