#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"
#include "chararray.h"

lexer_T *init_lexer(char *src)
{
    lexer_T *lexer = calloc(1, sizeof(struct lexer_struct));
    lexer->src = src;
    lexer->src_size = strlen(src);
    lexer->i = 0;
    lexer->c = src[lexer->i];
    lexer->state = STATE_START;

    return lexer;
}

void lexer_advance(lexer_T *lexer)
{
    if (lexer->i < lexer->src_size && lexer->c != '\0')
    {
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T *lexer)
{
    while (lexer->c == ASCII_CR || lexer->c == ASCII_LF || lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n')
    {
        lexer_advance(lexer);
    }
}

token lexer_next_token(lexer_T *lexer)
{
    char* value = chararray_init();
    int one_more=1;
    while (one_more)
    {
        if (lexer->c == '\0'){
            one_more =0;
        }
        printf("State %d, character '%c'\n", lexer->state, lexer->c);
        switch (lexer->state)
        {
        case STATE_START:
            lexer_skip_whitespace(lexer);
            if (lexer->c == '$')
            {
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_START;
                break;
            }
            else if (isalpha(lexer->c) || lexer->c == '_'){
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_IDENTIFIER_OR_KEYWORD_E;
                break;
            }
            else if (isdigit(lexer->c))
            {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_E;
            }
            
            else{
                printf("Lexer error in %d\n", lexer->state);
                lexer_advance(lexer);
            }
            break;

        case STATE_VARIABLE_START:
            if (isalpha(lexer->c) || lexer->c == '_')
            {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_E;
                break;
            }
            else{
                printf("Lexer error in %d\n", lexer->state);
                lexer->state = STATE_START;
            }
            break;

        case STATE_VARIABLE_E:
            if (isalnum(lexer->c) || lexer->c == '_')
            {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                break;
            }
            else{
                printf("Token is variable\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                token Token = {.ID=TOKEN_ID_VARIABLE, .VAL =tok_val};
                return Token ;
            }
            break;

        case STATE_IDENTIFIER_OR_KEYWORD_E:
            if (isalnum(lexer->c) || lexer->c == '_')
            {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                break;
            }
            else{
                printf("Token is identifier or keyword\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                token Token = {.ID=TOKEN_ID_IDENTIFIER, .VAL =tok_val};
                return Token ;
            }
            break;
        case STATE_INTEGER_E:
            if (isdigit(lexer->c)){
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                break;
            }
            else if (lexer ->c =='.'){
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_AND_SEPARATOR;
                break;
            }
            else if (tolower(lexer ->c)=='e'){
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_START;
                break;
            }
            else{
                printf("Token is integer\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                token Token = {.ID=TOKEN_ID_INTEGER, .VAL =tok_val};
                return Token ;
            }
        case STATE_INTEGER_AND_SEPARATOR:
            if (isdigit(lexer->c)){
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state=STATE_DOUBLE_E;
                break;
            }
            else{
                printf("Lexer error in %d\n", lexer->state);
                lexer_advance(lexer);
            }
        
        case STATE_DOUBLE_E:
            if (isdigit(lexer->c)){
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                break;
            }
            else if (tolower(lexer ->c)=='e'){
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_START;
                break;
            }
            else{
                printf("Token is double\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                token Token = {.ID=TOKEN_ID_DOUBLE, .VAL =tok_val};
                return Token ;
            }
        }
    }
}
