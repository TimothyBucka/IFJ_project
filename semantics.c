/**
 * Project: IFJ22 Compiler
 * 
 * @file semantics.c
 * @brief Implementation of function that converts keyword to data type
 * 
 * @authors xbucka00
*/

#include "semantics.h"
/**
 * @brief Converts keyword to data type
 * 
 * @param kw 
 * @return data_type 
 */
data_type kw_to_data_type(keyword kw) {
    switch (kw) {
    case KW_VOID:
        return NULL_TYPE;
    case KW_INT:
        return INT;
    case KW_INT_NULL:
        return INT_NULL;
    case KW_FLOAT:
        return FLOAT;
    case KW_FLOAT_NULL:
        return FLOAT_NULL;
    case KW_STRING:
        return STRING;
    case KW_STRING_NULL:
        return STRING_NULL;
    default:
        return UNDEFINED;
    }
}
