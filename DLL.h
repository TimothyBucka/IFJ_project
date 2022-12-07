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

/**
 * @brief Creates new doubly linked list
 * 
 * @return DLL* 
 */
DLL *createDLL();

/**
 * @brief Pushes new token to the end of the list
 * 
 * @param dll 
 * @param t 
 */
void DLL_push(DLL *dll, token *t);

token *DLL_pop(DLL *dll);

/**
 * @brief Returns token from the end of the list
 * 
 * @param dll 
 * @return token* 
 */
token *DLL_get_active(DLL *dll);

/**
 * @brief Moves active element to the left
 * 
 * @param dll 
 */
void DLL_move_active_left(DLL *dll);

/**
 * @brief Moves active element to the right
 * 
 * @param dll 
 */
void DLL_move_active_right(DLL *dll);

/**
 * @brief Frees the list
 * 
 * @param dll 
 */
void DLL_free(DLL *dll);

#endif
