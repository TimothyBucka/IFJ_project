#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "lexer.h"
#include "token.h"
#include "DLL.h"


#define return_error        \
    if (ERROR == SUCCESS) { \
        ERROR = SYNTAX_ERR; \
    }                       \
    return false;



#define next_tok                                  \
    ;                                             \
    if (dll->activeElement == dll->lastElement) { \
        ERROR = lexer_next_token(lexer, token);   \
        DLL_push(dll, token);                     \
        if (ERROR != SUCCESS) {                   \
            return false;                         \
        }                                         \
    }                                             \
    else {                                        \
        DLL_move_active_right(dll);               \
        *token = DLL_get_active(dll);             \
    }

bool accept(token *token, token_ID acceptedID);

bool expect(token *token, token_ID acceptedID);

bool parse_arguments_prime(lexer_T *lexer, DLL *dll);

bool parse_arguments(lexer_T *lexer, DLL *dll);

bool parse_function_call(lexer_T *lexer, DLL *dll);

bool parse_function_call_prime(lexer_T *lexer, DLL *dll);

bool parse_type(lexer_T *lexer, DLL *dll);

bool parse_parameters_prime(lexer_T *lexer, DLL *dll);

bool parse_parameters(lexer_T *lexer, DLL *dll);

bool parse_assignment_prime(lexer_T *lexer, DLL *dll);

bool parse_assignment(lexer_T *lexer, DLL *dll);

bool parse_body(lexer_T *lexer, DLL *dll) ;


#endif