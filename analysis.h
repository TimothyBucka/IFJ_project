#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "lexer.h"
#include "token.h"
#include "DLL.h"
#include "symtable.h"
#include "semantics.h"


#define return_error        \
    if (ERROR == SUCCESS) { \
        ERROR = SYNTAX_ERR; \
    }                       \
    return false;



#define next_tok                                  \
    ;                                             \
    if (dll->activeElement == dll->lastElement) { \
        token_ptr = calloc(1, sizeof(*token_ptr));        \
        ERROR = lexer_next_token(lexer, token_ptr);   \
        DLL_push(dll, token_ptr);                     \
        if (ERROR != SUCCESS) {                   \
            return false;                         \
        }                                         \
    }                                             \
    else {                                        \
        DLL_move_active_right(dll);               \
        token_ptr = DLL_get_active(dll);              \
    }

#define return_tok\
    ;\
    DLL_move_active_left(dll);\


bool accept(token *token_ptr, token_ID acceptedID);

bool expect(token *token_ptr, token_ID acceptedID);

bool parse_arguments_prime(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_arguments(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_function_call(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_function_call_prime(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_type(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_parameters_prime(lexer_T *lexer, DLL *dll, symtables tables, function* func);

bool parse_parameters(lexer_T *lexer, DLL *dll, symtables tables, function* func);

bool parse_assignment_prime(lexer_T *lexer, DLL *dll, symtables tables, data_type* type);

bool parse_assignment(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_body(lexer_T *lexer, DLL *dll, symtables tables) ;

bool run_analysis(lexer_T *lexer, DLL *dll);


#endif