#include "symtable.h"
//#include "tokens.c"

SymTab *declaration(SymTab *SymTable){

    SymTable = malloc(sizeof(struct sym_tab));
    if(SymTable == NULL){
        fprintf(stderr,"SYMTABLE ALLOCATION ERROR!!!\n");
        exit(1);
    }
    SymTable->func = NULL;
    SymTable->var = NULL;

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
        argument = argument->next;
    }
  //  printf("%s[%d] - > ", argument->name, argument->type);

    argument->next = malloc(sizeof(struct inParam));
    argument->next->next = NULL;
    argument->next->name = arg_name->data;
    argument->next->type = arg_type->type;
   // printf("%s[%d]\n ", argument->next->name, argument->next->type);
}
void addOutputArguments(Token *func_name, Token *arg_type, function Func){
     function Found = findFunction(func_name, Func);
    if(Found->output_params == NULL){
        Found->output_params = malloc(sizeof(struct outParam));
        Found->output_params->next = NULL;
        Found->output_params->type = arg_type->type;
    //    printf("[%d] -> \n", Found->output_params->type);
        return;
    }
    outputParams argument = Found->output_params;
    while(argument->next != NULL){
 //       printf("[%d] -> ", argument->type);
        argument = argument->next;
    }
 //   printf("[%d] - > ", argument->type);

    argument->next = malloc(sizeof(struct outParam));
    argument->next->next = NULL;
    argument->next->type = arg_type->type;
 //   printf("[%d]\n ", argument->next->type);
}
 Token *create_and_set_token(Token *helper){
    helper->next = malloc (sizeof(Token));
  //  printf("[%s]\n ", helper->data);
    helper = helper->next;
    helper->next = NULL;
    return helper;
}

Token *get_print_token(Token *prints){
    prints->size = 6;
    prints->data = malloc (prints->size);
    strcpy(prints->data,"print\0");
    prints->type = 10 ;
    prints->next = NULL;
    return prints;
}
Token *get_inputs_token(Token *inputs){
    inputs->size = 7;
    inputs->data = malloc (inputs->size);
    strcpy(inputs->data,"inputs\0");
    inputs->type = 10;
    inputs->next = NULL;
    return inputs;
}
Token *get_inputi_token(Token *inputi){
    inputi->size = 7;
    inputi->data = malloc (inputi->size);
    strcpy(inputi->data,"inputi\0");
    printf("%s\n", inputi->data);
    inputi->type =10;
    inputi->next = NULL;
    return inputi;
}
Token *get_inputf_token(Token *inputf){
    inputf->size = 7;
    inputf->data = malloc (inputf->size);
    strcpy(inputf->data,"inputf\0");
    printf("%s\n", inputf->data);
    inputf->type = 10;
    inputf->next = NULL;
    return inputf;
}
Token *get_int2float_token(Token *int2float){
    int2float->size = 10;
    int2float->data = malloc (int2float->size);
    strcpy(int2float->data,"int2float\0");
    int2float->type = 10;
    int2float->next = NULL;
    return int2float;
}
Token *get_float2int_token(Token *float2int){
    float2int->size = 10;
    float2int->data = malloc (float2int->size);
    strcpy(float2int->data,"float2int\0");
    float2int->type = 10;
    float2int->next = NULL;
    return float2int;
}
Token *get_len_token(Token *len){  
    len->size = 4;
    len->data = malloc (len->size);
    strcpy(len->data,"len\0");
    printf("%s\n", len->data);
    len->type =10;
    len->next = NULL;
    return len;
}
Token *get_substr_token(Token *substr){
    substr->size = 10;
    substr->data = malloc (substr->size);
    strcpy(substr->data,"substr\0");
    substr->type = 10;
    substr->next = NULL;
    return substr;
}
Token *get_ord_token(Token *ord){
    ord->size = 4;
    ord->data = malloc (ord->size);
    strcpy(ord->data,"ord\0");
    ord->type = 10;
    ord->next = NULL;
    return ord;
}
Token *get_chr_token(Token *chr){
    chr->size = 4;
    chr->data = malloc (chr->size);
    strcpy(chr->data,"chr\0");
    chr->type = 10;
    chr->next = NULL;
    return chr;
}
Token *get_int_token(Token *integer){
    integer->size = 4;
    integer->data = malloc (integer->size);
    strcpy(integer->data,"int");
    integer->type = 1;
    integer->next = NULL;
    return integer;
}
Token *get_float_token(Token *floating){
    floating->size = 8;
    floating->data = malloc (floating->size);
    strcpy(floating->data,"float64");
    floating->type = 2;
    floating->next = NULL;
    return floating;
}
Token *get_string_token(Token *string){
    string->size = 7;
    string->data = malloc (string->size);
    strcpy(string->data,"string");
    string->type = 3;
    string->next = NULL;
    return string;
}
Token *get_i_token(Token *i){
    i->size = 2;
    i->data = malloc (i->size);
    strcpy(i->data,"i");
    i->type = 0;
    i->next = NULL;
return i;
}
Token *get_f_token(Token *f){
    f->size = 2;
    f->data = malloc (f->size);
    strcpy(f->data,"f");
    f->type = 0;
    f->next = NULL;
return f;
}
Token *get_s_token(Token *s){
    s->size = 2;
    s->data = malloc (s->size);
    strcpy(s->data,"s");
    s->type = 0;
    s->next = NULL;
return s;
}
Token *get_n_token(Token *n){
    n->size = 2;
    n->data = malloc (n->size);
    strcpy(n->data,"n");
    n->type = 0;
    n->next = NULL;
return n;
}
Token *get_term_token(Token *term){
    term->size = 5;
    term->data = malloc (term->size);
    strcpy(term->data,"term");
    term->type = 0;
    term->next = NULL;
return term;
}
Token *get_print_type_token(Token *print_type)
{
    print_type->size = 11;
    print_type->data = malloc (print_type->size);
    strcpy(print_type->data,"print_type\0");
    print_type->type = TOKEN_TYPE_PRINT;
    print_type->next = NULL;
return print_type;
}
void *symTab_for_inbuilt_func(Token *helper,SymTab *S)
{
    Token *prints;
    Token *inputs;
    Token *inputi;
    Token *inputf;
    Token *int2float;
    Token *float2int;
    Token *len;
    Token *substr;
    Token *ord;
    Token *chr;
    Token *integer;
    Token *floating;
    Token *string;
    Token *i;
    Token *f;
    Token *s;
    Token *n;

    Token *term;
    Token *print_type;
    
    helper = malloc (sizeof(Token));
    helper->next = NULL;
    start = helper;

    helper = get_print_token(helper);
    prints = helper;
    insertFunction(prints, &(S->func));
    helper = create_and_set_token(helper);
    helper = get_term_token(helper);
    term = helper;
    helper = create_and_set_token(helper);
    helper = get_print_type_token(helper);
    print_type = helper;
    addInputArguments(prints, term, print_type, S->func);
    addInputArguments(prints, term, print_type, S->func);
    addInputArguments(prints, term, print_type, S->func);
    addInputArguments(prints, term, print_type, S->func);

    //S->func->input_params->next = S->func->input_params;
    helper = create_and_set_token(helper);
    helper = get_inputs_token(helper);
    inputs = helper;
    insertFunction(inputs, &(S->func));
    helper = create_and_set_token(helper);
    helper = get_string_token(helper);
    string = helper;
    helper = create_and_set_token(helper);
    helper = get_int_token(helper);
    integer = helper;
    addOutputArguments(inputs, string, S->func);
    addOutputArguments(inputs, integer, S->func);

    helper = create_and_set_token(helper);
    helper = get_inputi_token(helper);
    inputi = helper;
    insertFunction(inputi, &(S->func));
    addOutputArguments(inputi, integer, S->func);
    addOutputArguments(inputi, integer, S->func);

    helper = create_and_set_token(helper);
    helper = get_inputf_token(helper);
    inputf = helper;
    insertFunction(inputf, &(S->func));
    helper = create_and_set_token(helper);
    helper = get_float_token(helper);
    floating = helper;
    addOutputArguments(inputf, floating, S->func);
    addOutputArguments(inputf, integer, S->func);

    helper = create_and_set_token(helper);
    helper = get_int2float_token(helper);
    int2float = helper;
    insertFunction(int2float, &(S->func));
    helper = create_and_set_token(helper);
    helper = get_i_token(helper);
    i = helper;
    addInputArguments(int2float, i, integer, S->func);
    addOutputArguments(int2float, floating, S->func);

    helper = create_and_set_token(helper);
    helper = get_float2int_token(helper);
    float2int = helper;
    insertFunction(float2int, &(S->func));
    helper = create_and_set_token(helper);
    helper = get_f_token(helper);
    f = helper;
    addInputArguments(float2int, f, floating, S->func);
    addOutputArguments(float2int, integer, S->func);

    helper = create_and_set_token(helper);
    helper = get_len_token(helper);
    len = helper;
    insertFunction(len, &(S->func));
    helper = create_and_set_token(helper);
    helper = get_s_token(helper);
    s = helper;
    addInputArguments(len, s, string, S->func);
    addOutputArguments(len, integer, S->func);

    helper = create_and_set_token(helper);
    helper = get_substr_token(helper);
    substr = helper;
    insertFunction(substr, &(S->func));
    helper = create_and_set_token(helper);
    helper = get_n_token(helper);
    n = helper;
    addInputArguments(substr, s, string, S->func);
    addInputArguments(substr, i, integer, S->func);
    addInputArguments(substr, n, integer, S->func);
    addOutputArguments(substr, string, S->func);
    addOutputArguments(substr, integer, S->func);

    helper = create_and_set_token(helper);
    helper = get_ord_token(helper);
    ord = helper;
    insertFunction(ord, &(S->func));
    addInputArguments(ord, s, string, S->func);
    addInputArguments(ord, i, integer, S->func);
    addOutputArguments(ord, integer, S->func);
    addOutputArguments(ord, integer, S->func);
    
    helper = create_and_set_token(helper);
    helper = get_chr_token(helper);
    chr = helper;
    insertFunction(chr, &(S->func));
    addInputArguments(chr, i, integer, S->func);
    addOutputArguments(chr, string, S->func);
    addOutputArguments(chr, integer, S->func);

   // Print_func(S->func);
    // int z = 0;
    // while(start){
    //     printf("%s ",start->data);
    //     start= start->next;
    //     z++;}
    //     printf("%d\n",z);
   
   // return S;
}





