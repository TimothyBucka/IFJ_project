

typedef enum
{
    TOKEN_ID_IDENTIFIER,
    TOKEN_ID_VARIABLE,
    TOKEN_ID_PLUS,
    TOKEN_ID_MINUS,
    TOKEN_ID_INTEGER,
    TOKEN_ID_DOUBLE

}token_ID;


typedef union
{
    int integer;
    double decimal;
    char* string;
}token_VAL;

typedef struct
{
    token_ID ID;
    token_VAL VAL;
}token;


