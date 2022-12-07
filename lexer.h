/**
 * Project: IFJ22 Compiler
 * 
 * @file lexer.h
 * @brief Interface for lexer
 * 
 * @authors xmacur09, xfrust00, xstect00, xbucka00
*/

#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chararray.h"
#include "errors.h"
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
    STATE_QUOTATION_CENTER_E,
    STATE_QUOTATION_ESCAPE_CHAR,
    STATE_SLASH,
    STATE_QUESTIONMARK,
    STATE_CLOSING_TAG,

    STATE_GT_E,      //greater than
    STATE_ST_E,      //smaller than
    STATE_EQ_E,      //first eq
    STATE_EQEQ,      //second eq
    STATE_NOT,       // !
    STATE_NOT_EQ,    // !=
} lexer_state;

typedef enum {
    STATE_NONE,
    STATE_
} lexer_comment_state;

typedef struct lexer_struct {
    char c;
    unsigned int i;
    lexer_state state;
    bool prologParsed;

} lexer_T;

enum stav {CEL, DES, EXP};

/**
 * @brief Initializes the lexer
 * 
 * @return lexer_T* 
 */
lexer_T *lexer_init();

/**
 * @brief Frees the lexer
 * 
 * @param lexer 
 */
void lexer_free(lexer_T *lexer);

/**
 * @brief Advances the lexer
 * 
 * @param lexer 
 */
void lexer_advance(lexer_T *lexer);

/**
 * @brief Skips the comments
 * 
 * @param lexer 
 */
void lexer_skip_comment(lexer_T *lexer);

/**
 * @brief Skips the whitespace
 * 
 * @param lexer 
 */
void lexer_skip_whitespace(lexer_T *lexer);

/**
 * @brief Returns the next token from the lexer
 * 
 * @param lexer 
 * @param Token 
 * @return error 
 */
error lexer_next_token(lexer_T *lexer, token *Token);

int isoctdigit(char c);

/**
 * @brief Checks if the identifier is a keyword
 * 
 * @param src 
 * @return int 
 */
int is_keyword(char *src);

int str_to_int(char *src); 

double str_to_doule(char *src); 

/**
 * @brief Sanitizes the string
 * 
 * @param str 
 */
void clean_string(char **src); 

#endif
