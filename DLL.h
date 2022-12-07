/**
 * Project: IFJ22 Compiler
 * 
 * @file DLL.h
 * @brief Interface for doubly linked list
 * 
 * @authors xmacur09
*/

#ifndef DLL_H
#define DLL_H

#include "token.h"
#include <stdlib.h>

typedef struct DLLElement {
    token data;
    struct DLLElement *previousElement;
    struct DLLElement *nextElement;
} * DLLElementPtr;

typedef struct {
    DLLElementPtr firstElement;
    DLLElementPtr activeElement;
    DLLElementPtr lastElement;
} DLL;

DLL *createDLL();

void DLL_push(DLL *dll, token *t);

token *DLL_pop(DLL *dll);

token *DLL_get_active(DLL *dll);

void DLL_move_active_left(DLL *dll);

void DLL_move_active_right(DLL *dll);

void DLL_free(DLL *dll);

#endif
