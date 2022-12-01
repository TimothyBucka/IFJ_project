# ifndef SEMANTICS_H
# define SEMANTICS_H

typedef enum {
    NONE,        // 0
    NULL_VAL,    // 1
    INT,         // 2
    INT_NULL,    // 3
    DOUBLE,      // 5
    DOUBLE_NULL, // 6
    STRING,      // 7
    STRING_NULL  // 8
} data_type;

# endif