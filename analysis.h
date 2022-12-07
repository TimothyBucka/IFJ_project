/**
 * Project: IFJ22 Compiler
 * 
 * @file analysis.h
 * @brief Interface of syntax analysis
 * 
 * @authors xmacur09, xstect00, xbucka00
*/

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "DLL.h"
#include "lexer.h"
#include "semantics.h"
#include "symtable.h"
#include "token.h"

#define return_error(error_type) \
    if (ERROR == SUCCESS) {      \
        ERROR = error_type;      \
    }                            \
    return false;

#define compare_params(type_of_param)                                         \
    if (data->f_or_v.function->parameters[ARGSCOUNT].type != type_of_param && \
        data->f_or_v.function->parameters[ARGSCOUNT].type != TERM_TYPE) {     \
        ERROR = PARAMETERS_ERR;                                               \
        return false;                                                         \
    }

#define next_tok                                    \
    ;                                               \
    if (dll->activeElement == dll->lastElement) {   \
        token_ptr = calloc(1, sizeof(*token_ptr));  \
        ERROR = lexer_next_token(lexer, token_ptr); \
        DLL_push(dll, token_ptr);                   \
        if (ERROR != SUCCESS) {                     \
            ERRORFROMLEXER = 1;                     \
            return false;                           \
        }                                           \
    }                                               \
    else {                                          \
        DLL_move_active_right(dll);                 \
        token_ptr = DLL_get_active(dll);            \
    }

#define return_tok \
    ;              \
    DLL_move_active_left(dll);

bool accept(token *token_ptr, token_ID acceptedID);

bool expect(token *token_ptr, token_ID acceptedID);

bool parse_arguments_prime(lexer_T *lexer, DLL *dll, symtables tables, table_item_data *data);

bool parse_arguments(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_function_call(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_type(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_parameters_prime(lexer_T *lexer, DLL *dll, symtables tables, function *func);

bool parse_parameters(lexer_T *lexer, DLL *dll, symtables tables, function *func);

bool parse_assignment_prime(lexer_T *lexer, DLL *dll, symtables tables, data_type *type);

bool parse_assignment(lexer_T *lexer, DLL *dll, symtables tables);

bool parse_body(lexer_T *lexer, DLL *dll, symtables tables);

bool run_analysis(lexer_T *lexer, DLL *dll);

void preload_hash_table(hash_table table);

#endif