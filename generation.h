#include "scanner.h"
void START_OF_FUNCTION(char *id_of_function);
void END_OF_FUNCTION();
void START_MAIN();
void END_OF_MAIN();
void CALL(char *id_of_function);
void DEFVAR_TF(Token *token);
void GENERATION_INPUTS();
void GENERATION_INPUTI();
void GENERATION_INPUTF();
void GENERATION_LEN();
void GENERATION_FLOAT2INT();
void GENERATION_INT2FLOAT();
void GENERATION_SUBSTR();
void GENERATION_ORD();
void GENERATION_CHR();
void GENERATION_PRINT();
