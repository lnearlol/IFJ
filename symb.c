#include "token.h"
#include <stdbool.h>

Token *t;

union Types
{
  int value;
  float float_value;
  char *string;
};

typedef struct Variable{
    char *name;
    int type;
    union Types value;
    int length;
    bool used;
    bool declared;
    int deep;
    struct Variable *next;
    struct Variable *other_deep;
} *variable;

typedef struct Function{
    char *name;
    int type;
    int length;
    bool used;
    bool declared;
    struct Function *next;
    struct Function *Tparams;
} *function;

typedef struct params{
    char *type;
    struct params *next;
} *otParams;

typedef struct param{
    char *name;
    char *type;
    struct param *next;
} *itParams;

typedef struct Params{
    itParams input;
    otParams output;
} Tparams;

typedef struct sym_tab
{
  variable var;//struct sym_tab* func;
  function func;//struct sym_tab* var;
}SymTab;

void get_and_set_token(){
    t->next = malloc (sizeof(Token));
    t = t->next;
    t->next = NULL;
    get_token(t);
}

SymTab *declaration(SymTab *S)//Symtab
{
    S = malloc(sizeof(SymTab));
 //printf("%p declaration\n",S);
    if(S == NULL)
        fprintf(stderr,"ERROR!!!\n");

    return S;
 }
  function Create_node_func(Token *t,int deeps)
 {
    function newPtr= malloc(sizeof(struct Function));
    newPtr->name = malloc(t->size);
    strcpy(newPtr->name, t->data);
    newPtr->Tparams = NULL;
    newPtr->next = NULL;
    newPtr->used = false;
    newPtr->declared = false;
    newPtr->length = t->size;
    return newPtr;
 }

 variable Create_node(Token *t,int deeps)
 {
    variable newPtr= malloc(sizeof(struct Variable));//zkontrolovat úspěšnost operace 
    newPtr->name = malloc(t->size);
    
    strcpy(newPtr->name, t->data);
    newPtr->next = NULL;
    newPtr->deep = deeps;
    newPtr->other_deep = NULL;
    newPtr->used = false;
    newPtr->declared = false;
    newPtr->length = t->size;
    return newPtr;
 }

  void insert_var(Token *t,int deeps, SymTab *S)
 {
    bool first = false;
    variable newVar;
    newVar = Create_node(t,deeps);
     if (S->var == NULL)
     {
      //    S->var = malloc(sizeof(struct Variable));
          S->var = newVar;
  //        printf("its VAR \n");
          first = true;
     }
    variable search_in_var = S->var;
    int i=0;

    if (!first)
    {
        while(search_in_var->next!= NULL){
            printf("                 kak dela         \n");
            if(!strcmp(search_in_var->name,newVar->name))
            {  
//                printf("The same name\n"); 
                while(search_in_var->other_deep!= NULL) 
                {
                //   printf("DEEPEE\n");
                    search_in_var = search_in_var->other_deep;
                    
                }
                search_in_var->other_deep = newVar;
                // printf("%s %d je to newVar\n",search_in_var->other_deep->name,search_in_var->other_deep->deep);
                // printf("MK kod is\n");
                return;
            }
            
            search_in_var = search_in_var->next;
            i++;
            printf("koko\n");
            break;
        }
        if(!strcmp(search_in_var->name,newVar->name))
        {
            while(search_in_var->other_deep!= NULL)
            {
            // printf("%s %d je to search\n",search_in_var->name,search_in_var->deep);
            // printf("%s %d je to search\n",search_in_var->other_deep->name,search_in_var->other_deep->deep);
            // printf("%s %d je to newVar\n",newVar->name,newVar->deep);
            // printf("lololololololololo\n");
            search_in_var = search_in_var->other_deep;
            }
            search_in_var->other_deep = newVar;
            // printf("is not deep\n");
            // printf("%s %d  QQQ %s %d  QQQ  %s %d je to newVar\n",S->var->name,S->var->deep,search_in_var->name,search_in_var->deep,search_in_var->other_deep->name,search_in_var->other_deep->deep);
            // return;
        }

        printf("is not equal\n");
        search_in_var->next = newVar;
    }
//    printf("%d this is i\n",i);
}

    bool find_var_in_sym_test(Token *t,int deeps,SymTab*S)
    {
        bool finded = false;
        variable newVar = S->var;
         while(newVar!= NULL)
         {
        //     printf("%s %d    this is find ",newVar->name, newVar->deep);
        //     printf("%s %d\n",t->data, deeps);
            if(!strcmp(newVar->name, t->data))
            {  
                printf("FIND AHHHOJ\n"); 
                while(newVar != NULL){
               //     printf(" 1111 declaration\n");
                    if(newVar->deep == deeps)
                    {
                      printf("Repeated declaration");
                      finded = true;
                      printf("zASE AHOJ\n");
                      return true;
                    }
                    newVar = newVar->other_deep ;
                }  
                return false;
            }
     //       printf("isssss not equal\n");
            newVar = newVar->next;
        }
        return false;
    }

  void delete_level(Token *t, SymTab *S){
        bool finded = false;
        variable newVar = S->var;
        variable delVar;
        variable delDeep;
        int o=0;
        int L=0;
        printf("HELLOOOO\n");
        if (newVar != NULL)
        {
            printf("HELLO2\n");
            while(newVar->next!= NULL)
            {
                printf("HELLLLLOOO3\n");
                //printf(" %s %d %s %d %s %d LALA ",newVar->name,newVar->deep,newVar->other_deep->name,newVar->other_deep->deep,newVar->other_deep->other_deep->name,newVar->other_deep->other_deep->deep);
                printf("%s LALA\n",t->data);
                printf("%s %s %d %s %d %s %d QQQQ\n",newVar->name,newVar->next->name,newVar->next->deep,newVar->next->other_deep->name,newVar->next->other_deep->deep,newVar->next->next->name,newVar->next->next->deep);

                if(!strcmp(newVar->next->name, t->data))
                {  
                   // if(newVar->)
                 //   newVar->name = t->data;
                    printf("HELLLLLOOO4\n");
                    delVar = newVar->next;   
                    printf("%s QQQQ\n",delVar->name);
                    newVar->next = delVar->next;
                    if(delVar!=NULL)
                    {
                        while (delVar->other_deep!=NULL)
                        {
                            delDeep = delVar->other_deep;
                            delVar->other_deep = delDeep->other_deep;
                            free(delDeep);
                            delVar = delVar->other_deep ;
                            printf("FFFFFFFF\n"); 
                            L++;
                        }
                        printf("%d LLL\n",L);
                    }
                    free(delVar);
                    printf("FIND\n"); 
                    return ;
                }
            // if(!strcmp(newVar->name, t->data))
            // {
            //     printf("enter to cyklus\n");
            //     while(newVar->other_deep!=NULL)
            //     {
            //         delDeep = newVar->other_deep;
            //         newVar->other_deep = delDeep->other_deep;
            //         free(delDeep);
            //         printf("enter to cyklu2s\n");
            //         delVar = delVar->other_deep;
            //         o++;
            //     }
            //     free(newVar);
            //     printf("enter 2to cyklus\n");
            //     printf("%d ooo",o);
            // }
                printf("isssss not equal\n");
                newVar = newVar->next;
            }


        }
    }
    
    // Token *freed;
    // while(first != NULL){
    //     freed = first->next;
    //     free(first->data);
    //     free(first);
    //     first = freed;
    // }


int main(){
    program_code = fopen ("file.go", "r");
    
    t = malloc (sizeof(Token));
    t->next = NULL;
    Token *first = t;
    get_token(t);
    printf("%-10s       <-%d\n", t->data, t->type);
   
    SymTab *S; 
    int deeps = 0;
 //   S = malloc(sizeof(SymTab));
  //  printf("%p\n",S);
    S = declaration(S);
    int kkopalka = find_var_in_sym_test(t,deeps,S);
    printf("TOKEN TYPE: %d\n", kkopalka);
    insert_var(t,deeps, S);
    printf("\n");
    // int kopalka = find_var_in_sym_test(t,deeps,S);
    // printf("TOKEN TYPE: %d\n", kopalka);
    

     get_and_set_token();//!!!!!!!!!!!!!!!!!
    
     printf("%-10s       <-%d\n", t->data, t->type);
 
     int ppalka = find_var_in_sym_test(t,deeps,S);
    printf("TOKEN TYPE: %d\n", ppalka);
    insert_var(t,deeps, S);
    printf("\n");
    int palka = find_var_in_sym_test(t,deeps,S);
    printf("TOKEN TYPE: %d\n", palka);
    
 
    // get_and_set_token();
    // printf("%-10s       <-%d\n", t->data, t->type);
    // int be =0;
    // int gjopalka = find_var_in_sym_test(t,be,S);
    // printf("TOKEN TYPE: %d\n", gjopalka);
    // insert_var(t,deeps, S);
    // printf("\n");
    // int gopalka = find_var_in_sym_test(t,be,S);
    // printf("TOKEN ToYPE: %d\n", gopalka);

    get_and_set_token();
    
    printf("%-10s       <-%d\n", t->data, t->type);
    int beer =1;
    int ka = find_var_in_sym_test(t,beer,S);
    printf("TOKEN TYPE : %d\n", ka);
    insert_var(t,beer, S);
    printf("\n");
    // int hhka = find_var_in_sym_test(t,beer,S);
    // printf("TOKEN TYPE: %d\n", hhka);

        get_and_set_token();

    // delete_level(t, S);

    printf("%-10s       <-%d\n", t->data, t->type);
    int beerz =2;
    int kak = find_var_in_sym_test(t,beerz,S);
    printf("TOKEN TYPE: %d\n", kak);
    insert_var(t,beerz, S);
    printf("\n");
    // int hhkak = find_var_in_sym_test(t,beerz,S);
    // printf("TOKEN TYPE: %d\n", hhkak);
    
    get_and_set_token();
    printf("%-10s       <-%d\n", t->data, t->type);
    int beerzz =2;
    int kaki = find_var_in_sym_test(t,beerzz,S);
    printf("TOKEN TYPE THIS IS WHAT: %d\n", kaki);
    insert_var(t,beerzz, S);
    printf("\n");
    // int hhkaki = find_var_in_sym_test(t,beerzz,S);
    // printf("TOKEN TYPE: %d\n", hhkaki);
    //delete_level(t, S);

    get_and_set_token();
    printf("%-10s       <-%d\n", t->data, t->type);
    int boy =0;
    int kakik = find_var_in_sym_test(t,boy,S);
    printf("TOKEN TYPE: %d\n", kakik);
    insert_var(t,boy, S);
    printf(" ffffffffffffffffffff\n");
    // int hhkakik = find_var_in_sym_test(t,boy,S);
    // printf("TOKEN TYPE: %d\n", hhkakik);

    get_and_set_token();
    printf("%-10s  lll      <-%d\n", t->data, t->type);
    // get_and_set_token();
    // printf("%-10s       <-%d\n", t->data, t->type);
    // get_and_set_token();
    // printf("%-10s       <-%d\n", t->data, t->type);
    //     get_and_set_token();
    // printf("%-10s       <-%d\n", t->data, t->type);
    // get_and_set_token();
    // printf("%-10s       <-%d\n", t->data, t->type);
    //     get_and_set_token();
    // printf("%-10s       <-%d\n", t->data, t->type);
 
    dtor(first);
    
    fclose(program_code);
    return 0;
}
