#include "token.h"
#define FUNCTIONS_ARRAY_SIZE 10         //length and quantity of strings in array of command functions
#define LOGICAL_OPERATORS_LENGTH 3      //length of string in array of logical operators
#define LOGICAL_OPERATORS_QUANTITY 6    //quantity of strings in array of logical operators
#define TAB 9                           //horizontal tab ASCII code

//input validation functions
int allowed_symbol(char state);                     
int letter_or_num(char state);
int num(char state);
//reading a symbol from program code
char read_a_symbol(char state);
//function for reading the first non-space char
char first_non_space(char state);
//function for reading the first non-TAB char
char first_non_TAB(char state);
//function for reading the first non-EOL char
char first_non_EOL(char state);
//function of token data filling
void data_append(Token *token, char state);
//searching of command functions tokens
int check_command_functions(Token *token);
