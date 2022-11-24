#ifndef DLL_H
#define DLL_H

#include "stack.h"

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

#endif
