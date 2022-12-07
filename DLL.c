/**
 * Project: IFJ22 Compiler
 * 
 * @file DLL.c
 * @brief Implementation of doubly linked list
 * 
 * @authors xmacur09
*/

#include "DLL.h"
#include <stdio.h>

/**
 * @brief Creates new doubly linked list
 * 
 * @return DLL* 
 */
DLL *createDLL() {
    DLL *newDLL = malloc(sizeof(DLL));
    newDLL->firstElement = NULL;
    newDLL->activeElement = NULL;
    newDLL->lastElement = NULL;
    return newDLL;
}

/**
 * @brief Pushes new token to the end of the list
 * 
 * @param dll 
 * @param t 
 */
void DLL_push(DLL *dll, token *t) {
    DLLElementPtr new_element = malloc(sizeof(*new_element));
    new_element->data = *t;
    new_element->previousElement = dll->lastElement;
    new_element->nextElement = NULL;
    if (dll->lastElement != NULL) {
        dll->lastElement->nextElement = new_element;
    }
    else {
        dll->firstElement = new_element;
    }
    dll->lastElement = new_element;
    dll->activeElement = dll->lastElement;
}

/**
 * @brief Returns token from the end of the list
 * 
 * @param dll 
 * @return token* 
 */
token *DLL_get_active(DLL *dll) {
    if (dll->activeElement != NULL) {
        return &dll->activeElement->data;
    }
    return NULL;
}

/**
 * @brief Moves active element to the left
 * 
 * @param dll 
 */
void DLL_move_active_left(DLL *dll) {
    if (dll->activeElement == NULL) {
        return;
    }
    dll->activeElement = dll->activeElement->previousElement;
}

/**
 * @brief Moves active element to the right
 * 
 * @param dll 
 */
void DLL_move_active_right(DLL *dll) {
    if (dll->activeElement == NULL) {
        dll->activeElement = dll->firstElement;
        return;
    }
    dll->activeElement = dll->activeElement->nextElement;
}

/**
 * @brief Frees the list
 * 
 * @param dll 
 */
void DLL_free(DLL *dll) {
    DLLElementPtr current = dll->firstElement;
    DLLElementPtr next;
    while (current != NULL) {
        next = current->nextElement;
        if (current->data.VAL.string!= NULL && current->data.ID !=TOKEN_ID_KEYWORD) {
            free(current->data.VAL.string);
        }
        free(current);
        current = next;
    }
    free(dll);
}
