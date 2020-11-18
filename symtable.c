#include "symtable.h"


SymTab *declaration(SymTab *SymTable){

    SymTable = malloc(sizeof(struct sym_tab));
    if(SymTable == NULL){
        fprintf(stderr,"SYMTABLE ALLOCATION ERROR!!!\n");
        exit(1);
    }
    SymTable->func = NULL;
    SymTable->var = NULL;
// FUNC()
    return SymTable;
}

// ------------------------------------------------    P R I N T      T R E E   ------------------------------------------------

void Print_func2(function TempTree, char* sufix, char fromdir)
/* vykresli sktrukturu binarniho stromu */

{
     if (TempTree != NULL)
     {
	char* suf2 = (char*) malloc(strlen(sufix) + 4);
	strcpy(suf2, sufix);
        if (fromdir == 'L')
	{
	   suf2 = strcat(suf2, "  |");
	   printf("%s\n", suf2);
	}
	else
	   suf2 = strcat(suf2, "   ");
	Print_func2(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s,%d]\n", sufix, TempTree->name, TempTree->length);
	strcpy(suf2, sufix);
        if (fromdir == 'R')
	   suf2 = strcat(suf2, "  |");	
	else
	   suf2 = strcat(suf2, "   ");
	Print_func2(TempTree->LPtr, suf2, 'L');
	if (fromdir == 'R') printf("%s\n", suf2);
	free(suf2);
    }
}

void Print_func(function TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_func2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
} 


void Print_var2(variable TempTree, char* sufix, char fromdir)
/* vykresli sktrukturu binarniho stromu */

{
     if (TempTree != NULL)
     {
	char* suf2 = (char*) malloc(strlen(sufix) + 4);
	strcpy(suf2, sufix);
        if (fromdir == 'L')
	{
	   suf2 = strcat(suf2, "  |");
	   printf("%s\n", suf2);
	}
	else
	   suf2 = strcat(suf2, "   ");
	Print_var2(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s,%d,%d]\n", sufix, TempTree->name, TempTree->type, TempTree->deep);
	strcpy(suf2, sufix);
        if (fromdir == 'R')
	   suf2 = strcat(suf2, "  |");	
	else
	   suf2 = strcat(suf2, "   ");
	Print_var2(TempTree->LPtr, suf2, 'L');
	if (fromdir == 'R') printf("%s\n", suf2);
	free(suf2);
    }
}

void Print_var(variable TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_var2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
} 







// --------------------------------------   R O M A' S     C H A N G E S   --------------------------------------

 void insertVariable(Token *token, int deepVar, variable *Var){
    
    if(*Var == NULL){
		*Var = malloc(sizeof(struct Variable));
		(*Var)->name = token->data;
		(*Var)->length = token->size;
		(*Var)->LPtr = NULL;
		(*Var)->RPtr = NULL;
        (*Var)->prevTree = NULL;
        (*Var)->deep = deepVar;
        (*Var)->type = VAR_TYPE_UNDEFINED;  // VAR_TYPE_UNDEFINED = -1
		return;

	} else if ((*Var != NULL) && ((*Var)->deep < deepVar)){  
        variable tmp = malloc(sizeof(struct Variable));
        tmp->LPtr = NULL;
        tmp->RPtr = NULL;
        tmp->prevTree = (*Var);
        tmp->deep = deepVar;
        tmp->name = token->data;
        tmp->length = token->size;
        tmp->type = VAR_TYPE_UNDEFINED;  // VAR_TYPE_UNDEFINED = -1
        *Var = tmp;   
        return;
    }
    
    else if(strcmp((*Var)->name, token->data) > 0) {
		insertVariable(token, deepVar, &((*Var)->LPtr));

	} else if(strcmp((*Var)->name, token->data) < 0) {
		insertVariable(token, deepVar, &((*Var)->RPtr));

	} else if(strcmp((*Var)->name, token->data) == 0) {
        fprintf(stderr,"VARIABLE WITH THE SAME NAME ALREADY EXISTS!!!\n");
        exit(1);
		return;
	}
}

// FIND VARIABLE WITH THE SAME NAME ON MAXIMUM POSSIBLE LABEL
variable findVariable(Token *token, int deepVar, variable Var){
    variable tmp = Var;
    if(Var == NULL){
        return NULL;
    } if(deepVar < 0 || deepVar > Var->deep){
        return NULL;
    } else {
        tmp = findVariableHelper(token, deepVar, tmp);
        if(tmp != NULL){
            return tmp;
        }
        else 
            findVariable(token, deepVar-1, Var->prevTree);
    }
}


variable findVariableHelper(Token *token, int deepVar, variable Var){
    if (Var == NULL) {
        return NULL;
    } else if(strcmp(Var->name, token->data) < 0) {
		findVariableHelper(token, deepVar, Var->RPtr);

	} else if(strcmp(Var->name, token->data) > 0) {
		findVariableHelper(token, deepVar, Var->LPtr);
		
	} else if(strcmp(Var->name, token->data) == 0) 
		return Var;
}


//------------------- F I N D    E X I S T I N G    V A R I A B L E    W I T H   T Y P E

// FIND VARIABLE WITH NON-EMPTY TYPE WITH THE SAME NAME ON MAXIMUM POSSIBLE LABEL
variable findVariableWithType(Token *token, int deepVar, variable Var){
    variable tmp = Var;
    if(Var == NULL){
        return NULL;
    } if(deepVar < 0 || deepVar > Var->deep){
        return NULL;
    } else {
        tmp = find_var_with_type_helper(token, deepVar, tmp);
        if(tmp != NULL){
            return tmp;
        }
        else 
            findVariableWithType(token, deepVar-1, Var->prevTree);
    }

}

variable find_var_with_type_helper(Token *token, int deepVar, variable Var){
    if (Var == NULL) {
        return NULL;
    } else if(strcmp(Var->name, token->data) < 0) {
		find_var_with_type_helper(token, deepVar, Var->RPtr);
	} else if(strcmp(Var->name, token->data) > 0) {
		find_var_with_type_helper(token, deepVar, Var->LPtr);
	} else if(strcmp(Var->name, token->data) == 0 && Var->type != VAR_TYPE_UNDEFINED) {
		return Var;
    } else if (strcmp(Var->name, token->data) == 0 && Var->type == VAR_TYPE_UNDEFINED)
        return NULL;
}




//--------------------------------------

bool putTypeVariable(Token *token, int deepVar, int varType, variable Var){
    variable PutTypeVar = findVariable(token, deepVar, Var);
    if(PutTypeVar == NULL || PutTypeVar->type != VAR_TYPE_UNDEFINED){
        return false;
    } else {
        PutTypeVar->type = varType;
        return true;
    }
}


// DELETE ONLY LAST LABEL   (2->1->0  =>  1->0)
void freeVariablesLastLabel(variable *Var){
    if(*Var == NULL)
		return;
    else {
        variable tmp = (*Var)->prevTree;
        freeVariablesLastLabel(&(*Var)->LPtr);
        freeVariablesLastLabel(&(*Var)->RPtr);
        free(*Var);
        *Var = tmp;
        return;
    }
}

// int RIGHT_SIDE_TYPE   для высчета правой стороны перенести в парсер (в expression)
// сделать в expression(int previous_token) и закинуть -1, и каждый раз сравнивать если previous_token != -1 и делать compareTwoTokens
//  и закидывать его результат в previous_token 
//                                              и так до конца, и когда наткнулись на end_condition записать это дело в RIGHT_SIDE_TYPE

// DELETE WHOLE SYMTABLE->VAR
void freeAllVariables(variable *Var){

    while(*Var != NULL)
        freeVariablesLastLabel(Var);
}

// Для присваивания мне надо токен, токен, deep:  a := 2  <- a, 2, deep::::  addVar(a), check(right_side), set_type(a, 2, deep)
//                                                a := b expression должен вернуть хотя бы  TOKOKEN_TYPE нужного типа
//                                               нужна функция compare(token, token, deep) для двух переменных это
//                                               findVar(a), findVar(b) записать их типы и сравнить на совместимость


int compareTwoVariables(Token *var1, Token *var2, int deep, variable Var){
    int type1 = 0, type2 = 0;

    if(var1->type == TOKEN_TYPE_IDENTIFIER){
        type1 = findVariableWithType(var1, deep, Var)->type;
        printf("\n 1) type1: %d", type1);
    } else {
        type1 = returnLiteralType(var1);
        printf("\n 2) type1: %d", type1);
    }

    if(var2->type == TOKEN_TYPE_IDENTIFIER){
        type2 = findVariableWithType(var2, deep, Var)->type;
        printf("\n 3) type2: %d\n", type2);
    } else { 
        type2 = returnLiteralType(var2);
        printf("\n 4) type2: %d\n", type2);
    }
    if(type1 == type2)
        return type1;
    else 
        return 0;
}

// HELPS compareTwoString
int returnLiteralType(Token *token){
    if(token->type == TOKEN_TYPE_LITERAL_INT){
        return 1;
    } else if(token->type == TOKEN_TYPE_LITERAL_FLOAT){
        return 2;
    } else if(token->type == TOKEN_TYPE_LITERAL_STRING) {
        return 3;
    } else {
        return 0;
    }
}



// --------------------------------------------------  F  U  N  C  T  I  O  N  S  ----------------------------------------------------


void insertFunction(Token *token, function *Func){

	if(*Func == NULL){
		*Func = malloc(sizeof(struct Function));
		(*Func)->name = token->data;
		(*Func)->length = token->size;
        (*Func)->input_params = NULL;
        (*Func)->output_params = NULL;
		(*Func)->LPtr = NULL;
		(*Func)->RPtr = NULL;
		return;

	} else if(strcmp((*Func)->name, token->data) > 0) {
		insertFunction(token, &((*Func)->LPtr));

	} else if(strcmp((*Func)->name, token->data) < 0) {
		insertFunction(token, &((*Func)->RPtr));

	} else if(strcmp((*Func)->name, token->data) == 0) {
        fprintf(stderr,"FUNCTION WITH THE SAME NAME ALREADY EXISTS!!!\n");
        exit(1);
		return;
	}
}


void freeFunctions(function *Func){
    if(*Func == NULL){
		return;
	}
	freeFunctions(&(*Func)->LPtr);
	freeFunctions(&(*Func)->RPtr);
    inputParams itmp;
	while((*Func)->input_params != NULL){
        itmp = (*Func)->input_params->next;
        free((*Func)->input_params);
        (*Func)->input_params = itmp;
    }
    outputParams otmp;
    while((*Func)->output_params != NULL){
        otmp = (*Func)->output_params->next;
        free((*Func)->output_params);
        (*Func)->output_params = otmp;
    }

	free(*Func);
	*Func = NULL;
	
	return;
}

function findFunction(Token *token, function Func){
   
    if(Func == NULL){
		return NULL;
	} else if(strcmp(Func->name, token->data) < 0) {
		findFunction(token, Func->RPtr);

	} else if(strcmp(Func->name, token->data) > 0) {
		findFunction(token, Func->LPtr);
		
	} else if(strcmp(Func->name, token->data) == 0) {
		return Func;
	}
}

void addInputArguments(Token *func_name, Token *arg_name, Token *arg_type, function Func){
    function Found = findFunction(func_name, Func);

    if(Found->input_params == NULL){
        Found->input_params = malloc(sizeof(struct inParam));
        Found->input_params->next = NULL;
        Found->input_params->name = arg_name->data;
        Found->input_params->type = arg_type->type;
        return;
    }

    inputParams argument = Found->input_params;
    while(argument->next != NULL){
        printf("%s[%d] -> ", argument->name, argument->type);
        argument = argument->next;
    }
    printf("%s[%d] - > ", argument->name, argument->type);

    argument->next = malloc(sizeof(struct inParam));
    argument->next->next = NULL;
    argument->next->name = arg_name->data;
    argument->next->type = arg_type->type;
    printf("%s[%d]\n ", argument->next->name, argument->next->type);
}

void addOutputArguments(Token *func_name, Token *arg_type, function Func){
     function Found = findFunction(func_name, Func);

    if(Found->output_params == NULL){
        Found->output_params = malloc(sizeof(struct outParam));
        Found->output_params->next = NULL;
        Found->output_params->type = arg_type->type;
        return;
    }

    outputParams argument = Found->output_params;
    while(argument->next != NULL){
        printf("[%d] -> ", argument->type);
        argument = argument->next;
    }
    printf("[%d] - > ", argument->type);

    argument->next = malloc(sizeof(struct outParam));
    argument->next->next = NULL;
    argument->next->type = arg_type->type;
    printf("[%d]\n ", argument->next->type);
}