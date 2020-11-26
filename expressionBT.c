#include "expressionBT.h"

void BTInit (BTNodePtr *RootPtr) {
	*RootPtr=NULL;
}

void BTDelete (BTNodePtr *RootPtr) {
	if ((*RootPtr)->LPtr != NULL){
        BTDelete(&((*RootPtr)->LPtr));
        free((*RootPtr)->LPtr);
        (*RootPtr)->LPtr = NULL;
    }
    if ((*RootPtr)->RPtr != NULL) {
        BTDelete(&((*RootPtr)->RPtr));
        free((*RootPtr)->RPtr);
        (*RootPtr)->RPtr = NULL;
    }

        free(*RootPtr);
        *RootPtr = NULL;
        return;
}

void setBTData (BTNodePtr *RootPtr, Token token) {
    if (*RootPtr == NULL) {
		(*RootPtr) = malloc(sizeof(struct BTNode));

		(*RootPtr)->LPtr = NULL;
		(*RootPtr)->RPtr = NULL;
		(*RootPtr)->token = token;

	} else {
		(*RootPtr)->token = token;
    }
}

void setBTLeftChild (BTNodePtr *RootPtr, BTNodePtr *ChildPtr) {
    if (*RootPtr == NULL) {
		(*RootPtr) = malloc(sizeof(struct BTNode));

		(*RootPtr)->LPtr = *ChildPtr;
		(*RootPtr)->RPtr = NULL;

	} else {
		(*RootPtr)->LPtr = *ChildPtr;
    }
}

void setBTRightChild (BTNodePtr *RootPtr, BTNodePtr *ChildPtr) {
    if (*RootPtr == NULL) {
		(*RootPtr) = malloc(sizeof(struct BTNode));
		(*RootPtr)->RPtr = *ChildPtr;
		(*RootPtr)->LPtr = NULL;

	} else {
		(*RootPtr)->RPtr = *ChildPtr;
    }
}

BTStack_t* createBTStack() {
    BTStack_t *out = NULL;
    out = malloc(sizeof(BTStack_t));
    if (out == NULL) {
        exit(OUT_OF_MEMORY_BT_STACK);
    }
    out->size = INIT_SIZE_BT_STACK;
    out->data = malloc(out->size * sizeof(struct BTNode));
    if (out->data == NULL) {
        free(out);
        exit(OUT_OF_MEMORY_BT_STACK);
    }
    out->top = 0;
    return out;
}


void deleteBTStack(BTStack_t **stack) {
        free((*stack)->data);
        free(*stack);
        *stack = NULL;
}


void resizeBTStack(BTStack_t *stack) {
    stack->size *= MULTIPLIER_BT_STACK;
    stack->data = realloc(stack->data, stack->size * sizeof(struct BTNode));
    if (stack->data == NULL) {
        exit(BT_STACK_OVERFLOW);
    }
}


void pushBTStack(BTStack_t *stack, BTNodePtr value) {
    if (stack->top >= stack->size) {
        resizeBTStack(stack);
    }
    stack->data[stack->top] = value;
    stack->top++;
}

BTNodePtr popBTStack(BTStack_t *stack) {
    if (stack->top == 0) {
        exit(BT_STACK_UNDERFLOW);
    }
    stack->top--;
    return stack->data[stack->top];
}

BTNodePtr peekBTstack(const BTStack_t *stack) {
    if (stack->top <= 0) {
        exit(BT_STACK_UNDERFLOW);
    }
    return stack->data[stack->top - 1];
}


void BTStackimplode(BTStack_t *stack) {
    stack->size = stack->top;
    stack->data = realloc(stack->data, stack->size * sizeof(struct BTNode));
}