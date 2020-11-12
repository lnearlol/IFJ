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














//   function Create_node_func(Token *token, int deep){

//     function newPtr= malloc(sizeof(struct Function));
//     newPtr->name = malloc(token->size);
//     strcpy(newPtr->name, token->data);
//     newPtr->Tparams = NULL;
//     newPtr->next = NULL;
//     newPtr->used = false;
//     newPtr->declared = false;
//     newPtr->length = token->size;
//     return newPtr;
//  }

//  variable Create_node(Token *token, int deep){

//     variable newPtr= malloc(sizeof(struct Variable));//zkontrolovat úspěšnost operace 
//     newPtr->name = malloc(token->size);
    
//     strcpy(newPtr->name, token->data);
//     newPtr->next = NULL;
//     newPtr->deep = deep;
//     newPtr->other_deep = NULL;
//     newPtr->used = false;
//     newPtr->declared = false;
//     newPtr->length = token->size;
//     return newPtr;
//  }

// void insert_var(Token *token, int deep, SymTab *SymTable){

//     bool first = false;
//     variable newVar;
//     newVar = Create_node(token,deep);
//     if (SymTable->var == NULL){
//       //    SymTable->var = malloc(sizeof(struct Variable));
//         SymTable->var = newVar;
//   //        printf("its VAR \n");
//         first = true;
//     }
//     variable search_in_var = SymTable->var;
//     int i=0;

//     if (!first){
//         while(search_in_var->next!= NULL){
//             printf("                 kak dela         \n");
//             if(!strcmp(search_in_var->name,newVar->name)){  

//                 while(search_in_var->other_deep!= NULL) {
//                     search_in_var = search_in_var->other_deep;
//                 }
//                 search_in_var->other_deep = newVar;
//                 return;
//             }
            
//             search_in_var = search_in_var->next;
//             i++;
//             printf("koko\n");
//             break;
//         }
//         if(!strcmp(search_in_var->name,newVar->name)){

//             while(search_in_var->other_deep!= NULL){
//                 search_in_var = search_in_var->other_deep;
//             }
//             search_in_var->other_deep = newVar;
//         }

//         printf("is not equal\n");
//         search_in_var->next = newVar;
//     }
// }

// bool find_var_in_sym_test(Token *token, int deep, SymTab*SymTable)
// {
//     bool finded = false;
//     variable newVar = SymTable->var;
//     while(newVar!= NULL){
//         if(!strcmp(newVar->name, token->data)){  
//             printf("FIND AHHHOJ\n"); 
//             while(newVar != NULL){
//                //     printf(" 1111 declaration\n");
//                 if(newVar->deep == deep){
//                     printf("Repeated declaration");
//                     finded = true;
//                     printf("zASE AHOJ\n");
//                     return true;
//                 }
//                 newVar = newVar->other_deep ;
//             }  
//             return false;
//         }
//         newVar = newVar->next;
//         }
//         return false;
// }

// void delete_level(Token *token, SymTab *SymTable){
//     bool finded = false;
//     variable newVar = SymTable->var;
//     variable delVar;
//     variable delDeep;
//     int o=0;
//     int L=0;
//     printf("HELLOOOO\n");
//     if (newVar != NULL){
//         printf("HELLO2\n");
//         while(newVar->next!= NULL){
//             printf("HELLLLLOOO3\n");
//             printf("%s LALA\n",token->data);
//             printf("%s %s %d %s %d %s %d QQQQ\n",newVar->name,newVar->next->name,newVar->next->deep,newVar->next->other_deep->name,newVar->next->other_deep->deep,newVar->next->next->name,newVar->next->next->deep);

//             if(!strcmp(newVar->next->name, token->data)){  

//                 printf("HELLLLLOOO4\n");
//                 delVar = newVar->next;   
//                 printf("%s QQQQ\n",delVar->name);
//                 newVar->next = delVar->next;
//                 if(delVar!=NULL){
//                     while (delVar->other_deep!=NULL){
//                         delDeep = delVar->other_deep;
//                         delVar->other_deep = delDeep->other_deep;
//                         free(delDeep);
//                         delVar = delVar->other_deep ;
//                         printf("FFFFFFFF\n"); 
//                         L++;
//                     }
//                     printf("%d LLL\n",L);
//                 }
//                 free(delVar);
//                 printf("FIND\n"); 
//                 return;
//             }
//             printf("isssss not equal\n");
//             newVar = newVar->next;
//         }


//     }
// }


// --------------------------------------   R O M A' S     C H A N G E S   --------------------------------------


function Create_function(Token *token){

    function newPtr = malloc(sizeof(struct Function));//zkontrolovat úspěšnost operace 
    newPtr->name = token->data;
    newPtr->RPtr = NULL;
    newPtr->LPtr = NULL;
    newPtr->length = token->size;
    return newPtr;
 }

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