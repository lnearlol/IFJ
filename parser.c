#include "parser.h"

void allocating_error(){
     printf ("*ERROR* Memory was not alocated.\n");
    errflg = true;                      /* globální proměnná -- příznak chyby */
}

// deleting the element from the top of the stack
void delete_from_stack(Syntax_Stack *S){
    stack_next tmp = S->top->next;
    free(S->top);
    S->top = tmp; 
}

// creating new element on top of the syntax_stack
void add_to_stack(Syntax_Stack *S, int type){
    stack_next new_in_stack = malloc(sizeof(struct Stack_Next));
    if(new_in_stack == NULL)
        allocating_error();
    new_in_stack->token_type = type;
    
    if(S->top != NULL)
        new_in_stack->next = S->top;
    else
        new_in_stack->next = NULL;
    S->top = new_in_stack;
}

void create_new_stack(Syntax_Stack *S){
    add_to_stack(S, TOKEN_TYPE_EOFILE);
    add_to_stack(S, TOKEN_TYPE_END_BLOCK);
    add_to_stack(S, TOKEN_TYPE_EOL);
    add_to_stack(S, TOKEN_TYPE_START_BLOCK);
    add_to_stack(S, TOKEN_TYPE_RIGHT_BRACKET); // maybe
        // STRICTLY

    add_to_stack(S, TOKEN_TYPE_LEFT_BRACKET);
    add_to_stack(S, TOKEN_TYPE_IDENTIFIER);  //some
    add_to_stack(S, TOKEN_TYPE_FUNC);  // strict
    add_to_stack(S, TOKEN_TYPE_EOL);
    add_to_stack(S, TOKEN_TYPE_IDENTIFIER);
    add_to_stack(S, TOKEN_TYPE_PACKAGE);
}

int structure_check(Token *token){
    static Syntax_State SYNTAX_STATE = STATE_NULL;  // to define in which SYNTAX_STATE programm is [for, if, define, func, else, return, }]
    static Syntax_Condition syntax_condition = CONDITION_STRICTLY; // to define that if we should strictly follow stack, or not
    static Token actual_variable_token;
    static int deep = -1;

    // creating new stack
    if(SYNTAX_STATE == STATE_NULL && token->type != TOKEN_TYPE_EOL){
        create_new_stack(&STACK);
        SYNTAX_STATE = STATE_BEGIN;
        // ignore all '\n' symbols on the beginning
    } else if (SYNTAX_STATE == STATE_NULL && token->type == TOKEN_TYPE_EOL)
        return 1;

    // count number of current label
    if(token->type == TOKEN_TYPE_START_BLOCK)
        deep++;
    else if(token->type == TOKEN_TYPE_END_BLOCK)
        deep--;

    if(deep >= 0 && token->type == TOKEN_TYPE_EOL)
        syntax_condition = CONDITION_NON_STRICTLY;

    // STATE == BEGIN
    if(syntax_condition == CONDITION_NON_STRICTLY && token->type == TOKEN_TYPE_FUNC){
        SYNTAX_STATE = STATE_FUNC;
        syntax_condition = CONDITION_STRICTLY; 

    // STATE == FUNC
    } else if(SYNTAX_STATE == STATE_FUNC){
        if (strcmp(token->data, "main")){
            printf("HELLO IM IN MAIN\n");
        } else if (!strcmp(token->data, "main")){
            // dodej ramec
            add_to_stack(&STACK, TOKEN_TYPE_END_BLOCK);
            add_to_stack(&STACK, TOKEN_TYPE_EOL);
            add_to_stack(&STACK, TOKEN_TYPE_START_BLOCK);
            add_to_stack(&STACK, TOKEN_TYPE_RIGHT_BRACKET); // maybe
            add_to_stack(&STACK, TOKEN_TYPE_LEFT_BRACKET);
            add_to_stack(&STACK, TOKEN_TYPE_IDENTIFIER);
        } else {

        }
    }

        Syntax_Stack S = STACK;
              while(S.top != NULL){
            printf("%d ", S.top->token_type);
            S.top = S.top->next;
        }
        printf("\n");

    printf("TOKEN data: %s    number: %d   deep:   %d   ", token->data, token->type, deep);
    if(syntax_condition == CONDITION_STRICTLY && STACK.top->token_type == token->type){
        printf("Deleted %d\n", STACK.top->token_type);
        delete_from_stack(&STACK);
    }
    else {
        printf(" token_strict = %d stack: %d", syntax_condition, STACK.top->token_type);
        allocating_error();
    }
}


int main(){
    program_code = fopen ("file.ifj20", "r");

    Token *t;
    t = malloc (sizeof(Token));
    Token *first = t;

    for(int i = 0; ; i++) {
        get_token(t);
        //printf("TOKEN data: %s    number: %d\n", t->data, t->type);
        structure_check(t);
        if(t->type == TOKEN_TYPE_EOFILE)
            break;
        t->next = malloc (sizeof(Token));
        t = t->next;
    }


    // while(first != NULL){
    //     printf("data: %s   number: %d \n", first->data, first->type);
    //     first = first->next;
    // }

    fclose(program_code);
    return 0;
}