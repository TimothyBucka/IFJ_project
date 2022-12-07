/**
 * Project: IFJ22 Compiler
 * 
 * @file semantics.h
 * @brief Interface for semantics
 * 
 * @authors xmacur09, xbucka00
*/

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "token.h"

typedef enum {
    UNDEFINED,   // 0
    NULL_TYPE,   // 1 void
    INT,         // 2
    INT_NULL,    // 3
    FLOAT,       // 4
    FLOAT_NULL,  // 5
    STRING,      // 6
    STRING_NULL, // 7
    BOOL_TYPE,    // 8
    TERM_TYPE,   // 9           any of: INT, FLOAT, STRING
    
} data_type;

data_type kw_to_data_type(keyword);

#endif