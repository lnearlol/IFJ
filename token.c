enum {IDENTIFICATOR, INT, FLOAT64, STRING, LITERAL_INT, LITERAL_FLOAT, LITERAL_STRING, OPERATOR, START_BLOCK, END_BLOCK, LEFT_BRACKET, RIGHT_BRACKET, EOL, EOF}
typedef struct Token {
    int t_type;
    char* id_name;
    int* int_value;
    char* string;
    char[2] operator_name;
    int *priority_operator;
    struct Token *next;
} token;

