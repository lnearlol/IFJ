/**
 * @file logic.c
 * 
 * @brief Logic functions working with SymTable implementation
 * 
 * IFJ Projekt 2020, Tým 55
 * 
 * @author <xstepa64> Stepaniuk Roman, Bc.
*/


#include "parser.h"

/**
 * Function creats variables in SymTable with names which are written to varCompareList and give them types from typeCompareList 
 * and frees that lists
 * 
 * @param deep depth of label
 * @return Function returns true if all expression types are successfully written to variable->type in symtable. And
 * returns false if there was any syntax or semantic error
 */
bool check_declare_logic(int deep){
    while(varCompareList != NULL && typeCompareList != NULL){
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


/**
 * Function compares types of variables with names which are written to varCompareList and types from typeCompareList 
 * and frees that lists
 * 
 * @param deep depth of label
 * @return Function returns true if all types are the same, and returns false if at least only one type will be different
 */
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


