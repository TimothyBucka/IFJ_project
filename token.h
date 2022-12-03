#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_ID_IDENTIFIER, // functions
    TOKEN_ID_VARIABLE,   // variables
    TOKEN_ID_INTEGER,
    TOKEN_ID_FLOAT,
    TOKEN_ID_STRING,
    TOKEN_ID_KEYWORD,

    TOKEN_ID_PLUS,
    TOKEN_ID_MINUS,
    TOKEN_ID_EQUALS,        // =
    TOKEN_ID_TRIPLE_EQUALS, // ===
    TOKEN_ID_NOT_EQUALS,    // !==
    TOKEN_ID_GT,            //>
    TOKEN_ID_LT,            //<
    TOKEN_ID_GTE,           //>=
    TOKEN_ID_LTE,           //<=
    TOKEN_ID_CONCAT,
    TOKEN_ID_DIVISION,
    TOKEN_ID_MULTIPLICATION,
    TOKEN_ID_COMMA,
    TOKEN_ID_SEMICOLLON,
    TOKEN_ID_COLON,

    TOKEN_ID_LBRACKET,
    TOKEN_ID_RBRACKET,
    TOKEN_ID_LCURLYBRACKET,
    TOKEN_ID_RCURLYBRACKET,

    TOKEN_ID_EOL,
    TOKEN_ID_EOF,

} token_ID;

typedef enum {
    KW_NOKW,
    KW_BOOLEAN,  // rozsirenie bool
    KW_BREAK,    // rozsirenie cykly
    KW_CONTINUE, // rozsirenie cykly
    KW_ELSE,
    KW_ELSEIF, // rozsirenie bool
    KW_FALSE,  // rozsirenie bool
    KW_FLOAT,
    KW_FLOAT_NULL,
    KW_FOR, // rozsirenie cykly
    KW_FUNCTION,
    KW_GLOBAL, // rozsirenie glob
    KW_IF,
    KW_INT,
    KW_INT_NULL,
    KW_NULL,
    KW_RESULT,
    KW_RETURN,
    KW_STRING,
    KW_STRING_NULL,
    KW_TRUE, // rozsirenie bool
    KW_VOID,
    KW_WHILE,

} keyword;

typedef union {
    int integer;            //0
    double decimal;         //1
    char *string;           //2
    keyword keyword;        //3
} token_VAL;

typedef struct
{
    token_ID ID;
    token_VAL VAL;
    
} token;

// typedef enum {
//     STRING,
//     DECIMAL,
//     INTEGER,
//     VOID,
//     STRING_NULL,
//     DECIMAL_NULL,
//     INTEGER_NULL,
//     NULL_VAL,
//     UNDEFINED
// } data_type;

#endif
