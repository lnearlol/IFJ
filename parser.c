#include "parser.h"

void get_and_set_token(){
    token->next = malloc (sizeof(Token));
    token = token->next;
    token->next = NULL;
    get_token(token);
}


int main(){
    program_code = fopen ("file.ifj20", "r");
    
    token = malloc (sizeof(Token));
    token->next = NULL;
    Token *first = token;
    get_token(token);
    printf("%-10s       <-%d\n", token->data, token->type);
   
    SymTab *S = declaration(S);

    

    Token *func, *arg_name, *arg_type;

    
    insertFunction(token, &(S->func));
    get_and_set_token();

    insertFunction(token, &(S->func));
    func = token;
    //1
    get_and_set_token();
    arg_name = token;
    get_and_set_token();
    arg_type = token;
    addInputArguments(func, arg_name, arg_type, S->func);
    addOutputArguments(func, arg_type, S->func);
    //2
    get_and_set_token();
    arg_name = token;
    get_and_set_token();
    arg_type = token;
    addInputArguments(func, arg_name, arg_type, S->func);
    addOutputArguments(func, arg_type, S->func);
    get_and_set_token();
    arg_name = token;
    get_and_set_token();
    arg_type = token;
    addInputArguments(func, arg_name, arg_type, S->func);
    addOutputArguments(func, arg_type, S->func);

    printf("- %d\n", S->func->LPtr->input_params->next->type);
    printf("- %d\n", S->func->LPtr->output_params->next->type);
    





    // while(token->type != TOKEN_TYPE_EOFILE){

    //     insertFunction(token, &(S->func));
    //     get_and_set_token();
    // }

     Print_func(S->func);
    // function tmp;
    // if(findFunction(first->next->next, S->func)){
    //     tmp = findFunction(first->next->next, S->func);
    //     printf("%s FOUND\n", tmp->name);
    // } else 
    //     printf("%s NOT FOUND\n", first->next->next->data);

    freeFunctions(&(S->func));
    free(S);
    dtor(first);
    
    fclose(program_code);
    return 0;
}
