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
    STATE_INTEGER_EXPONENT_START,
    STATE_INTEGER_EXPONENT_E,
    STATE_DOUBLE_EXPONENT_E,
    STATE_QUOTATION_START,
    STATE_QUOTATION_CENTER,
    STATE_QUOTATION_END_E,
    


}lexer_state;

typedef struct lexer_struct
{
    char* src;
    size_t src_size;
    char c;
    unsigned int i;
    lexer_state state;

} lexer_T;

lexer_T* init_lexer(char* src);

void lexer_advance(lexer_T* lexer);

void lexer_skip_whitespace(lexer_T* lexer);

token lexer_next_token(lexer_T* lexer);