#include "token.h"
#include "symTable.h"
#include <stdbool.h>

Token *t;

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
 S->var =NULL;
 S->func = NULL;
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
    newPtr->name = t->data;//malloc(t->size);
    //strcpy(newPtr->name, t->data);
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
            printf("      kak dela         ");
            if(!strcmp(search_in_var->name,newVar->name))
            {  
                printf("The same name\n"); 
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
           printf("%s searchchchchc\n",search_in_var->name );
        }
        if(!strcmp(search_in_var->name,newVar->name))
        {
                printf("The same name\n"); 
            while(search_in_var->other_deep!= NULL)
            {
             printf("%s %d je to search\n",search_in_var->name,search_in_var->deep);
            // printf("%s %d je to search\n",search_in_var->other_deep->name,search_in_var->other_deep->deep);
            // printf("%s %d je to newVar\n",newVar->name,newVar->deep);
             printf("lololololololololo\n");
            search_in_var = search_in_var->other_deep;
            }
            search_in_var->other_deep = newVar;
            printf("The same name\n");
            return;
            // printf("is not deep\n");
            // printf("%s %d  QQQ %s %d  QQQ  %s %d je to newVar\n",S->var->name,S->var->deep,search_in_var->name,search_in_var->deep,search_in_var->other_deep->name,search_in_var->other_deep->deep);
            // return;
        }
  //      printf("is not equal\n");
        search_in_var->next = newVar;
    }
//    printf("%d this is i\n",i);
}

    bool find_var_in_sym_test(Token *t,int deeps,SymTab*S)
    {
        bool finded = false;
        variable newVar = S->var;
      //   printf("uu\n");
         while(newVar!= NULL)
         {
            printf("%s %d  'JE TO FIND %s  %dthis is find\n ",newVar->name, newVar->deep,t->data, deeps);
  //          printf("ppppp\n");
        //     printf("%s %d\n",t->data, deeps);
            if(!strcmp(newVar->name, t->data))
            {  
                printf("FIND AHHHOJ\n"); 
                while(newVar != NULL){
                    printf(" 1111 declaration\n");
                    if(newVar->deep == deeps)
                    {
                      printf("Repeated declaration");
                      finded = true;
    //                  printf("zASE AHOJ\n");
                      return true;
                    }
                    newVar = newVar->other_deep ;
                }  
                return false;
            }
     //      printf("isssss not equal\n");
            newVar = newVar->next;
        }
        return false;
    }

    //     void delete_level(int deeps,SymTab*S)
    // {
    //     printf("  kokokoko DELETE \n");
    //      variable delVar = S->var;
    //       variable delDeep;

        //   if(S->var->next == NULL)
        // {
        //     printf("   DELETE \n");
        //     while (S->var != NULL)
        //     {
        //         printf("   DELETE \n");
        //        if (S->var->deep == deeps)
        //       {
        //         printf("  same DELETE \n");
        //         free(S->var);
        //         return;
        //        }
        //        printf("  pp DELETE \n");
        //        S->var = S->var->other_deep;
        //        printf(" ff  DELETE \n");
        //     }

        // }
        //  while(S->var!= NULL)
        //  {
        //     if(S->var->next == NULL)
        //     {
        //         printf("%s %d DELETE this is find\n ",S->var->name, S->var->deep);
        //         printf("%s\n",S->var->name);
        //         free(S->var);
        //        return;
        //     }
        //     if (S->var->next->deep == deeps)
        //      {
        //         delVar = S->var;
        //         S->var = delVar->next;
        //         printf("%s NEWMEWMEWNEW DELETE\n", delVar->name);
        //         free(delVar);
        //       }
        //   }
    //     while(delVar != NULL){
    //         перескок  a1 -> b2 -> c2 =>  a1->c2
    // printf("%s %d", delVar->name,delVar->deep);
    //         while(delVar->next->deep == deeps){
    //             printf("  NEXT DELETE \n");
    //             delDeep = delVar->next;
    //             delVar->next = delDeep->next;
    //             free(delDeep);
    //         } 
    //         удаление по уровню  (вниз)  а1->a2 => a1
    //         delDeep = delVar->other_deep;
    //         printf("  LEVEL DELETE \n");
    //         while(delDeep != NULL){
    //             if(delDeep->deep == deeps)
    //             {
    //                 printf(" DELETE \n");
    //                 printf("%s %d", delDeep->name,delDeep->deep);
    //                 free(delDeep);
    //                 return;
    //             }
    //             else
    //             delDeep = delDeep->other_deep;
    //         }
    //         delVar = delVar->next;
    //     }
    //}
  
        void delete(SymTab *S)
    {
         variable newVar = S->var;
         variable delVar;
         while(newVar!= NULL)
         {
            if(newVar->next== NULL)
            {
                printf("%s %d DELETE this is find\n ",newVar->name, newVar->deep);
                printf("%s\n",newVar->name);
                free(newVar);
               return;
            }
            printf("%s %d  wwwwwwwwwwww DELETE this is find\n ",newVar->name, newVar->deep);
            printf("%s  kokokoko DELETE %s\n",newVar->name,newVar->next->name);
            delVar=newVar;
            newVar=newVar->next;
            printf("%s NEWMEWMEWNEW DELETE\n", delVar->name);
            free(delVar); 
         }
         newVar = NULL;
    }

int main(){
    program_code = fopen ("file.go", "r");
    
    t = malloc (sizeof(Token));
    t->next = NULL;
    Token *first = t;
    get_token(t);
    printf("%-10s       <-%d\n", t->data, t->type);
    
    int deeps = 0;
    S = declaration(S);
     int kkopalka = find_var_in_sym_test(t,deeps,S);
    printf("TOKEN TYPE: %d\n", kkopalka);
     insert_var(t,deeps, S);
     
    printf("\n");
    int kopalka = find_var_in_sym_test(t,deeps,S);
     printf("TOKEN TYPE: %d\n", kopalka);

       get_and_set_token();
    
    int deepss = 0;
     printf("%-10s       <-%d\n", t->data, t->type);
     int ppalka = find_var_in_sym_test(t,deepss,S);
     printf("TOKEN TYPE: %d\n", ppalka);
       insert_var(t,deepss, S);
     int palka = find_var_in_sym_test(t,deepss,S);
     printf("TOKEN TYPE: %d\n", palka);
   
 
    get_and_set_token();
    printf("%-10s       <-%d\n", t->data, t->type);
    int be =0;
    int gjopalka = find_var_in_sym_test(t,be,S);
    printf("TOKEN TYPE: %d\n", gjopalka);
     insert_var(t,be, S);
    printf("\n");
    int gopalka = find_var_in_sym_test(t,be,S);
    printf("TOKEN ToYPE: %d\n", gopalka);


    //  get_and_set_token();
    
    // printf("%-10s       <-%d\n", t->data, t->type);
    // int beer =0;
    // int ka = find_var_in_sym_test(t,beer,S);
    //  printf("TOKEN TYPE : %d\n", ka);
    //  insert_var(t,beer, S);
    // printf("\n");
    // int hhka = find_var_in_sym_test(t,beer,S);
    // printf("TOKEN TYPE: %d\n", hhka);
    

//     get_and_set_token();
//     printf("%-10s       <-%d\n", t->data, t->type);
//     int beerzz =2;
//     int kaki = find_var_in_sym_test(t,beerzz,S);
//     printf("TOKEN TYPE THIS IS WHAT: %d\n", kaki);
//     insert_var(t,beerzz, S);

//delete_level(boy,S);
   delete(S);
    free(S);
    dtor(first);
    
    fclose(program_code);
    return 0;
}