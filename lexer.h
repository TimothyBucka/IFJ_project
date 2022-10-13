#include <ctype.h>
#include <stdlib.h>
#include "token.h"

typedef enum{
    STATE_START,
    STATE_VARIABLE_START,
    STATE_VARIABLE_E,
    STATE_IDENTIFIER_OR_KEYWORD_E,
    STATE_INTEGER_E,
    STATE_INTEGER_AND_SEPARATOR,
    STATE_DOUBLE_E,
    STATE_DOUBLE_EXPONENT_START,
    STATE_DOUBLE_EXPONENT_SIGN,
    STATE_INTEGER_EXPONENT_START,
    STATE_INTEGER_EXPONENT_SIGN,
    STATE_INTEGER_EXPONENT_E,
    STATE_DOUBLE_EXPONENT_E,
    STATE_QUOTATION_START,
    STATE_QUOTATION_CENTER,
    STATE_QUOTATION_END_E,
    STATE_SLASH,
    STATE_LINE_COMMENT_E,
    STATE_BLOCK_COMMENT_START,
    STATE_BLOCK_COMMENT_E,

    STATE_GT_E,   //greater than
    STATE_ST_E,   //smaller than
    STATE_EQ_E,   //first eq
    STATE_EQEQ, //second eq
}lexer_state;

typedef enum{
    STATE_NONE,
    STATE_
}lexer_comment_state;

typedef struct lexer_struct
{
    char* src;
    size_t src_size;
    char c;
    unsigned int i;
    lexer_state state;

} lexer_T;

lexer_T* lexer_init(char* src);

void lexer_free(lexer_T *lexer);

void lexer_advance(lexer_T* lexer);

void lexer_skip_whitespace(lexer_T* lexer);


void lexer_next_token(lexer_T* lexer, token *Token, int* ended);

int is_keyword(char* src);

int str_to_int(char* src);  //12 2431424343

double str_to_doule(char* src); //1e4 1E4 1.2e5  1.4E-/+6449

void clean_string(char* src);  // "\123 \xAF"