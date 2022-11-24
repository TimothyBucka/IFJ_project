#include "DLL.h"

DLL *createDLL(){
    DLL *newDLL = malloc(sizeof(DLL));
    newDLL->firstElement = NULL;
    newDLL->activeElement = NULL;
    newDLL->lastElement = NULL;
    return newDLL;
}

void DLL_push(DLL *dll, token *t){
    DLLElementPtr new_element = malloc( sizeof(*new_element));
    new_element->data = *t;
    new_element->previousElement = dll->lastElement;
    new_element->nextElement = NULL;
    if (dll->lastElement != NULL){
        dll->lastElement->nextElement = new_element;
    }
    else{
        dll->firstElement = new_element;
    }
    dll->lastElement = new_element;
    dll->activeElement = dll->lastElement;

}


token *DLL_get_active (DLL *dll){
    if(dll->activeElement == NULL){
        return NULL;
    }
    return &dll->activeElement->data;
}

void DLL_move_active_left(DLL *dll){
    if(dll->activeElement == NULL){
        return;
    }
    dll->activeElement = dll->activeElement->previousElement;
}

void DLL_move_active_right(DLL *dll){
    if(dll->activeElement == NULL){
        return;
    }
    dll->activeElement = dll->activeElement->nextElement;
}
