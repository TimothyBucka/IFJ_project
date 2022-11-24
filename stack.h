#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"

typedef struct stack {
    int size;
    struct item *next;
} stack;

typedef struct item
{
    token *token;
    struct item *next;
}item;


stack *stack_new();

void stack_push(stack *s, token *t);

token *stack_pop(stack *s);

#endif
