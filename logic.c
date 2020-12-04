
#include "parser.h"

// Write from typeCompareList to varCompareList and free that lists.
bool check_declare_logic(int deep){
    while(varCompareList != NULL && typeCompareList != NULL){
        // printf("varCompareList - %s, typeCompareList - %d\n", varCompareList->var->data, typeCompareList->type);
        if(!insertVariable(varCompareList->var, deep, &(SymTable->var)))
            changeErrorCode(3);
        else {
            if(!GET_REPEAT_FUNC_RUN()){
                insertGenVariable(varCompareList->var, deep, &(SymTable->genVar));
            }
        }
        if(putTypeVariable(varCompareList->var, deep, typeCompareList->type, SymTable->var)){
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
    while(varCompareList != NULL && typeCompareList != NULL){
        if(compareTwoVariables(varCompareList->var, typeCompareList->type, deep, SymTable->var)){
            // assembly
            delete_var_from_compare_list();
            delete_type_from_compare_list();
        } else 
            break;
    } 
    if(varCompareList != NULL || typeCompareList != NULL){
        freeBothCompareLists();
        return false;
    } else {
        return true;
    }
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


