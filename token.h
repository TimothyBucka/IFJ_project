

typedef enum
{
    TOKEN_ID_IDENTIFIER,
    TOKEN_ID_VARIABLE,
    TOKEN_ID_INTEGER,
    TOKEN_ID_DOUBLE,
    TOKEN_ID_STRING,
    TOKEN_ID_KEYWORD,


    TOKEN_ID_PLUS,
    TOKEN_ID_MINUS,
    TOKEN_ID_EQUALS,
    TOKEN_ID_TRIPLE_EQUALS,
    TOKEN_ID_GT,        //>
    TOKEN_ID_ST,        //<
    TOKEN_ID_GTE,       //>=
    TOKEN_ID_STE,       //<=
    TOKEN_ID_CONCAT,
    TOKEN_ID_DIVISION,
    TOKEN_ID_MULTIPLICATION,
    TOKEN_ID_COMMA,
    TOKEN_ID_SEMICOLLON,


    TOKEN_ID_LBRACKET,
    TOKEN_ID_RBRACKET,
    TOKEN_ID_LCURLYBRACKET,
    TOKEN_ID_RCURLYBRACKET,

    TOKEN_ID_EOL,
    TOKEN_ID_EOF,


}token_ID;



typedef enum
{
    KW_ELSE,
    KW_FLOAT,
    KW_FUNCTION,
    KW_IF,
    KW_INT,
    KW_NULL,
    KW_RESULT,
    KW_STRING,
    KW_VOID,
    KW_WHILE,

}keyword;


typedef union
{
    int integer;
    double decimal;
    char* string;
    keyword keyword;
}token_VAL;

typedef struct
{
    token_ID ID;
    token_VAL VAL;
}token;


