# ifndef SEMANTICS_H
# define SEMANTICS_H

typedef enum {
    NO_TYPE,        // 0
    NULL_TYPE,    // 1
    INT,         // 2
    INT_NULL,    // 3
    DOUBLE,      // 4
    DOUBLE_NULL, // 5
    STRING,      // 6
    STRING_NULL, // 7
    BOOL_TYPE    // 8
    
} data_type;

# endif