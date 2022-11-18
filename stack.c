#include "stack.h"

stack *stack_new(){
    stack *new_stack = malloc(sizeof(stack));
    new_stack->next = NULL;
    new_stack->size = 0;
    return new_stack;
}

void stack_push(stack *s, token *t){
    item *new_item = malloc(sizeof(item));
    new_item->next = s->next;
    new_item->token = t;
    s->next = new_item;
    s->size ++;
}

token* stack_pop(stack *s){
    if(s->size == 0){
        return NULL;
    }
    item *new_item = s->next;
    token *tok = new_item->token;
    free(new_item);
    s->size --;
    return tok;
}