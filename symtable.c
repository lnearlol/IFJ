#include "symtable.h"

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

// --------------------------------------   V A R I A B L E S   --------------------------------------

 bool insertVariable(Token *token, int deepVar, variable *Var){
    
    if(*Var == NULL){
		*Var = malloc(sizeof(struct Variable));
		(*Var)->name = token->data;
		(*Var)->length = token->size;
		(*Var)->LPtr = NULL;
		(*Var)->RPtr = NULL;
        (*Var)->prevTree = NULL;
        (*Var)->deep = deepVar;
        (*Var)->type = VAR_TYPE_UNDEFINED;  // VAR_TYPE_UNDEFINED = -1
		return true;

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
        return true;
    }
    
    else if(strcmp((*Var)->name, token->data) > 0) {
		insertVariable(token, deepVar, &((*Var)->LPtr));

	} else if(strcmp((*Var)->name, token->data) < 0) {
		insertVariable(token, deepVar, &((*Var)->RPtr));

	} else if(strcmp((*Var)->name, token->data) == 0) {
        return false;
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
    }
    if (deepVar > Var->deep) {
        while(deepVar != Var->deep)
            --deepVar;
    }
    if(deepVar < 0 ){
        return NULL;
    }  else {
        tmp = find_var_with_type_helper(token, deepVar, tmp);
        if(tmp != NULL){
            return tmp;
        }
        else {
            if(Var->prevTree == NULL)
                return NULL;
            findVariableWithType(token, deepVar-1, Var->prevTree);
        }
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

// DELETE WHOLE SYMTABLE->VAR
void freeAllVariables(variable *Var){

    while(*Var != NULL)
        freeVariablesLastLabel(Var);
}

int compareTwoVariables(Token *var1, int var2, int deep, variable Var){
    int type1 = 0, type2 = var2;
    if (var1 == NULL)
        return 0;

    if(var1->type == TOKEN_TYPE_IDENTIFIER){      
        variable tmp = findVariableWithType(var1, deep, Var);

        if(!tmp){
            changeErrorCode(3);
            return 0;
        }
        type1 = tmp->type;
    } else if (var1->type == TOKEN_TYPE_UNDERSCORE) {
        return type2;
    } else {
        type1 = returnLiteralType(var1);
    }

    
    if(type1 == type2){
        return type1;
    } else 
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
        changeErrorCode(3);
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
    argument->next = malloc(sizeof(struct inParam));
    argument->next->next = NULL;
    argument->next->name = arg_name->data;
    argument->next->type = arg_type->type;
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
        argument = argument->next;
    }
    argument->next = malloc(sizeof(struct outParam));
    argument->next->next = NULL;
    argument->next->type = arg_type->type;
}







// ---------------------------------------   I N B U I L D    F U N C T I O N S   ---------------------------------------
Token *create_and_set_token(Token *helper){
    helper->next = malloc(sizeof(Token));
    helper = helper->next;
    helper->next = NULL;
    return helper;
}

Token *get_print_token(Token *prints){
    prints->size = 6;
    prints->data = malloc(prints->size);
    strcpy(prints->data,"print\0");
    prints->type = 10 ;
    prints->next = NULL;
    return prints;
}

Token *get_inputs_token(Token *inputs){
    inputs->size = 7;
    inputs->data = malloc(inputs->size);
    strcpy(inputs->data,"inputs\0");
    inputs->type = 10;
    inputs->next = NULL;
    return inputs;
}
Token *get_inputi_token(Token *inputi){
    inputi->size = 7;
    inputi->data = malloc(inputi->size);
    strcpy(inputi->data,"inputi\0");
    inputi->type =10;
    inputi->next = NULL;
    return inputi;
}
Token *get_inputf_token(Token *inputf){
    inputf->size = 7;
    inputf->data = malloc(inputf->size);
    strcpy(inputf->data,"inputf\0");
    inputf->type = 10;
    inputf->next = NULL;
    return inputf;
}
Token *get_int2float_token(Token *int2float){
    int2float->size = 10;
    int2float->data = malloc(int2float->size);
    strcpy(int2float->data,"int2float\0");
    int2float->type = 10;
    int2float->next = NULL;
    return int2float;
}
Token *get_float2int_token(Token *float2int){
    float2int->size = 10;
    float2int->data = malloc(float2int->size);
    strcpy(float2int->data,"float2int\0");
    float2int->type = 10;
    float2int->next = NULL;
    return float2int;
}
Token *get_len_token(Token *len){  
    len->size = 4;
    len->data = malloc(len->size);
    strcpy(len->data,"len\0");
    len->type =10;
    len->next = NULL;
    return len;
}
Token *get_substr_token(Token *substr){
    substr->size = 10;
    substr->data = malloc(substr->size);
    strcpy(substr->data,"substr\0");
    substr->type = 10;
    substr->next = NULL;
    return substr;
}
Token *get_ord_token(Token *ord){
    ord->size = 4;
    ord->data = malloc(ord->size);
    strcpy(ord->data,"ord\0");
    ord->type = 10;
    ord->next = NULL;
    return ord;
}
Token *get_chr_token(Token *chr){
    chr->size = 4;
    chr->data = malloc(chr->size);
    strcpy(chr->data,"chr\0");
    chr->type = 10;
    chr->next = NULL;
    return chr;
}
Token *get_int_token(Token *integer){
    integer->size = 4;
    integer->data = malloc(integer->size);
    strcpy(integer->data,"int\0");
    integer->type = 1;
    integer->next = NULL;
    return integer;
}
Token *get_float_token(Token *floating){
    floating->size = 8;
    floating->data = malloc(floating->size);
    strcpy(floating->data,"float64\0");
    floating->type = 2;
    floating->next = NULL;
    return floating;
}
Token *get_string_token(Token *string){
    string->size = 7;
    string->data = malloc(string->size);
    strcpy(string->data,"string\0");
    string->type = 3;
    string->next = NULL;
    return string;
}
Token *get_i_token(Token *i){
    i->size = 2;
    i->data = malloc(i->size);
    strcpy(i->data,"i\0");
    i->type = 0;
    i->next = NULL;
return i;
}
Token *get_f_token(Token *f){
    f->size = 2;
    f->data = malloc(f->size);
    strcpy(f->data,"f\0");
    f->type = 0;
    f->next = NULL;
return f;
}
Token *get_s_token(Token *s){
    s->size = 2;
    s->data = malloc(s->size);
    strcpy(s->data,"s\0");
    s->type = 0;
    s->next = NULL;
return s;
}
Token *get_n_token(Token *n){
    n->size = 2;
    n->data = malloc(n->size);
    strcpy(n->data,"n\0");
    n->type = 0;
    n->next = NULL;
return n;
}
Token *get_print_type_token(Token *print_type)
{
    print_type->size = 11;
    print_type->data = malloc(print_type->size);
    strcpy(print_type->data,"print_type\0");
    print_type->type = TOKEN_TYPE_PRINT;
    print_type->next = NULL;
return print_type;
}

void symTab_for_inbuilt_func(SymTab *SymTable){
    Token *prints, *inputs, *inputi, *inputf, *int2float, *float2int, *len, *substr, 
    *ord, *chr, *integer, *floating, *string, *i, *f, *s, *n, *helper, *print_type;
    
    helper = malloc (sizeof(Token));
    helper->next = NULL;
    startCommandFuncList = helper;

    helper = get_print_token(helper);
    prints = helper;
    insertFunction(prints, &(SymTable->func));
    helper = create_and_set_token(helper);
    helper = get_i_token(helper);
    i = helper;
    helper = create_and_set_token(helper);
    helper = get_print_type_token(helper);
    print_type = helper;
    addInputArguments(prints, i, print_type, SymTable->func);
    helper = create_and_set_token(helper);
    helper = get_inputs_token(helper);
    inputs = helper;
    insertFunction(inputs, &(SymTable->func));
    helper = create_and_set_token(helper);
    helper = get_string_token(helper);
    string = helper;
    helper = create_and_set_token(helper);
    helper = get_int_token(helper);
    integer = helper;
    addOutputArguments(inputs, string, SymTable->func);
    addOutputArguments(inputs, integer, SymTable->func);

    helper = create_and_set_token(helper);
    helper = get_inputi_token(helper);
    inputi = helper;
    insertFunction(inputi, &(SymTable->func));
    addOutputArguments(inputi, integer, SymTable->func);
    addOutputArguments(inputi, integer, SymTable->func);

    helper = create_and_set_token(helper);
    helper = get_inputf_token(helper);
    inputf = helper;
    insertFunction(inputf, &(SymTable->func));
    helper = create_and_set_token(helper);
    helper = get_float_token(helper);
    floating = helper;
    addOutputArguments(inputf, floating, SymTable->func);
    addOutputArguments(inputf, integer, SymTable->func);

    helper = create_and_set_token(helper);
    helper = get_int2float_token(helper);
    int2float = helper;
    insertFunction(int2float, &(SymTable->func));
    addInputArguments(int2float, i, integer, SymTable->func);
    addOutputArguments(int2float, floating, SymTable->func);

    helper = create_and_set_token(helper);
    helper = get_float2int_token(helper);
    float2int = helper;
    insertFunction(float2int, &(SymTable->func));
    helper = create_and_set_token(helper);
    helper = get_f_token(helper);
    f = helper;
    addInputArguments(float2int, f, floating, SymTable->func);
    addOutputArguments(float2int, integer, SymTable->func);

    helper = create_and_set_token(helper);
    helper = get_len_token(helper);
    len = helper;
    insertFunction(len, &(SymTable->func));
    helper = create_and_set_token(helper);
    helper = get_s_token(helper);
    s = helper;
    addInputArguments(len, s, string, SymTable->func);
    addOutputArguments(len, integer, SymTable->func);

    helper = create_and_set_token(helper);
    helper = get_substr_token(helper);
    substr = helper;
    insertFunction(substr, &(SymTable->func));
    helper = create_and_set_token(helper);
    helper = get_n_token(helper);
    n = helper;
    addInputArguments(substr, s, string, SymTable->func);
    addInputArguments(substr, i, integer, SymTable->func);
    addInputArguments(substr, n, integer, SymTable->func);
    addOutputArguments(substr, string, SymTable->func);
    addOutputArguments(substr, integer, SymTable->func);

    helper = create_and_set_token(helper);
    helper = get_ord_token(helper);
    ord = helper;
    insertFunction(ord, &(SymTable->func));
    addInputArguments(ord, s, string, SymTable->func);
    addInputArguments(ord, i, integer, SymTable->func);
    addOutputArguments(ord, integer, SymTable->func);
    addOutputArguments(ord, integer, SymTable->func);
    
    helper = create_and_set_token(helper);
    helper = get_chr_token(helper);
    chr = helper;
    insertFunction(chr, &(SymTable->func));
    addInputArguments(chr, i, integer, SymTable->func);
    addOutputArguments(chr, string, SymTable->func);
    addOutputArguments(chr, integer, SymTable->func);
}
