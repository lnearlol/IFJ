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
    printf("%-10s      <-%d\n", token->data, token->type);
   
    SymTab *S = declaration(S);
    symTab_for_inbuilt_func(S);

    Print_func(S->func);
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

   Print_func(S->func);

    int deep = 0; 
    get_and_set_token();
    int type = token->type;
    get_and_set_token();
    Token *find_check;
    int find_deep_check = -1;

    // просто каким-то циклом заполняем деревья
    for(int i = 0; i < 16; i++){
        insertVariable(token, deep, &(S->var));
        Print_var(S->var);
        get_and_set_token();
        if (i == 2){
            find_deep_check = deep;
            find_check = token;
        }
        if(i % 4 == 0){
            deep++;
            Print_var(S->var);
        } 
    }
    // проверка того, что деревья связаны
    Print_var(S->var->prevTree->prevTree->prevTree);

    printf("----------------------------------------------------MY OPERATIONS STARTS\n");
    // 1: ИЩЕМ КАКУЮ-ТО ПЕРЕМЕННУЮ В ДЕРЕВЕ НА 3 УРОВНЕ
    printf("%s\n\n",find_check->data);
    variable tmp1 = findVariable(find_check, 3, S->var);
    if(tmp1 != NULL)
        printf("\n 1:      - name: %s, deep: %d type: %d\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf("\n 1:    tmp1 is NULL  \n");

    // 2: КЛАДЕМ В ЭТУ ПЕРЕМЕННУЮ ТИП
    if(putTypeVariable(find_check, 3, type, S->var))
        printf("\n 2: задаем тип    - name: %s, deep: %d type: %d\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf("\n 2:     tmp1 is NULL  \n");

    // 3: КЛАДЕМ В ЭТУ ПЕРЕМЕННУЮ ТИП ЕЩЕ РАЗ (ОЖИДАЕМ ОШИБКУ)
    if(putTypeVariable(find_check, 3, type, S->var))
        printf("\n  3: put    - name: %s, deep: %d type: %d\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf("\n 3:  ПОПЫТКА ЗАДАТЬ ТИП В ПЕРЕМЕННОЙ, КОТОРОЙ ОН УЖЕ ЕСТЬ, ERROR  \n");

    // 4: ОПЯТЬ ИЩЕМ ЭТУ ПЕРЕМЕННУЮ, ПРОВЕРЯЕМ ЕЕ ТИП
    tmp1 = findVariable(find_check, 3, S->var);
    if(tmp1 != NULL)
        printf("\n 4:    - name: %s, deep: %d type: %d\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf("\n 4:     tmp1 is NULL  \n");

    //  ЗАДАЕМ ПЕРЕМЕННУЮ С ТАКИМ ЖЕ НАЗВАНИЕМ, НО НА ДРУГОМ УРОВНЕ
    insertVariable(token, deep, &(S->var));
    get_and_set_token();

    // 5: ПРОВЕРЯЕМ, ЧТО ФУНКЦИЯ ПОИСКА НАЙДЕТ ПЕРЕМЕННУЮ НА БОЛЕЕ ВЫСОКОМ УРОВНЕ
    tmp1 = findVariable(find_check, deep, S->var);
    if(tmp1 != NULL)
        printf("\n 5: new    - name: %s, deep: %d type: %d\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf(" 5:    tmp1 is NULL  \n");

    // 6: ИЩЕМ ПЕРЕМЕННУЮ С ЭТИМ НАЗВАНИЕМ, НО У КОТОРОЙ ВЫСТАВЛЕН ТИП (ОЖИДАЕМ НА БОЛЕЕ НИСКОМ УРОВНЕ)
    tmp1 = findVariableWithType(find_check, deep, S->var);
    if(tmp1 != NULL)
        printf("\n 6: new    - name: %s, deep: %d type: %d\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf("\n 6:     tmp1 is NULL  \n");

    // 7: ДОБАВЛЯЕМ ТИП ВО ВТОРУЮ ПЕРЕМЕННУЮ (ПО ПОСЛЕДНЕЙ ГЛУБИНЕ)
    if(putTypeVariable(find_check, deep, type, S->var))
        printf("\n 7:  задаем тип    - name: %s, deep: %d type: %d\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf("\n   ПОПЫТКА ЗАДАТЬ ТИП В ПЕРЕМЕННОЙ, КОТОРОЙ ОН УЖЕ ЕСТЬ, ERROR  \n");
    // 8: ПРОВЕРЯЕМ, ЧТО ПОИСК С ВЫСТАВЛЕННЫМ ТИПОМ НАЙДЕТ ПЕРЕМЕННУЮ НА БОЛЕЕ ВЫСОКОМ УРОВНЕ
    tmp1 = findVariableWithType(find_check, deep, S->var);
    if(tmp1 != NULL)
        printf("\n 8: new    - name: %s, deep: %d type: %d\n\n\n\n\n", tmp1->name, tmp1->deep, tmp1->type);
    else 
        printf("\n 8:     tmp1 is NULL  \n\n\n\n");


    // ПОПРОБОВАТЬ С РАЗНЫМИ ЗНАЧЕНИЯМИ: ПЕРЕМЕННАЯ С ПЕРЕМЕННОЙ, ПЕРЕМЕННАЯ С ТИПОМ И ПЕРЕМЕННАЯ БЕЗ ТИПА, РАЗНЫЕ КОМБИНАЦИИ ТИПОВ

    int check = compareTwoVariables(find_check, token, deep, S->var);
    printf(" token %s    Result of compareTwoVariables: %d\n", token->data, check);

    printf("----------------------------------------------------MY OPERATIONS ENDS\n");

    // freeVariablesLastLabel(&(S->var));
    freeAllVariables(&(S->var));
    freeFunctions(&(S->func));

    free(S);
    dtor(start);
    dtor(first);

    fclose(program_code);
    return 0;
}
