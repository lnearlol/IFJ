
#include "parser.h"

// Write from typeCompareList to varCompareList and free that lists.
bool check_declare_logic(int deep){
    printf("CHECK DECLARED LOGIC \n");
    while(varCompareList != NULL && typeCompareList != NULL){
        printf("CHECK DECLARED LOGIC 1\n");
        insertVariable(varCompareList->var, deep, &(SymTable->var));
        if(putTypeVariable(varCompareList->var, deep, typeCompareList->type, SymTable->var)){
            printf("CHECK DECLARED LOGIC 1\n");
            delete_var_from_compare_list();
            delete_type_from_compare_list(typeCompareList);
        } else 
            break;
    }
    if(varCompareList != NULL || typeCompareList != NULL){
        freeBothCompareLists();
        return false;
    } else 
        return true;
}


// Compare top of varCompareList and typeCompareList and free that lists. 
bool check_define_logic(int deep){
    printf("CHECK EQUATING LOGIC \n");
    while(varCompareList != NULL && typeCompareList != NULL){
        printf("EQUATING LOGIC 1 %s - %d, [%d]\n", varCompareList->var->data, varCompareList->var->type, typeCompareList->type);
        if(compareTwoVariables(varCompareList->var, typeCompareList->type, deep, SymTable->var)){
            // assembly
            printf("EQUATING LOGIC 2 [%d]\n", typeCompareList->type);
            delete_var_from_compare_list();
            delete_type_from_compare_list(typeCompareList);
        } else 
            break;
    } 
    printf("EQUATING LOGIC 3\n");
    if(varCompareList != NULL || typeCompareList != NULL){
        freeBothCompareLists();
        return false;
    } else
        return true;
}


bool compare_return_and_output_params_logic(){
    
    function F = findFunction(current_function_name, SymTable->func);
    outputParams out_params = F->output_params;
    while(out_params != NULL && typeCompareList != NULL){
        delete_type_from_compare_list(typeCompareList);
        if(out_params->type != typeCompareList->type)
            return false;
        out_params = out_params->next;
    }
    if(out_params != NULL || typeCompareList != NULL){
        freeBothCompareLists();
        return false;
    } else
        return true;
}


