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
#include "generator.h"

#define return_error(error_type) \
    if (ERROR == SUCCESS) {      \
        ERROR = error_type;      \
    }                            \
    return false;

#define compare_params(type_of_param)                                             \
    if (strcmp(data->name, "write")) {                                            \
        if (data->f_or_v.function->parameters[ARGSCOUNT].type != type_of_param && \
            data->f_or_v.function->parameters[ARGSCOUNT].type != TERM_TYPE) {     \
            ERROR = PARAMETERS_ERR;                                               \
            return false;                                                         \
        }                                                                         \
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

/**
 * @brief Shoud be called if an error wont be thrown
 * 
 * @param token_ptr 
 * @param acceptedID 
 * @return true 
 * @return false 
 */
bool accept(token *token_ptr, token_ID acceptedID);

/**
 * @brief Shoud be called if an error will be thrown
 * 
 * @param token_ptr 
 * @param acceptedID 
 * @return true 
 * @return false 
 */
bool expect(token *token_ptr, token_ID acceptedID);

/**
 * @brief Parses the remaining arguments of a function call
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @param data 
 * @return true 
 * @return false 
 */
bool parse_arguments_prime(lexer_T *lexer, DLL *dll, symtables tables, table_item_data *data, char *function_name);

/**
 * @brief Parses the first argument of a function call
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @return true 
 * @return false 
 */
bool parse_arguments(lexer_T *lexer, DLL *dll, symtables tables);

/**
 * @brief Parses a function call
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @return true 
 * @return false 
 */
bool parse_function_call(lexer_T *lexer, DLL *dll, symtables tables);

/**
 * @brief Parses the type tokens
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @return true 
 * @return false 
 */
bool parse_type(lexer_T *lexer, DLL *dll);

/**
 * @brief Parses the remaining parameters of a function
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @param func 
 * @return true 
 * @return false 
 */
bool parse_parameters_prime(lexer_T *lexer, DLL *dll, symtables tables, function *func);

/**
 * @brief Parses the first parameter of a function
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @param func 
 * @return true 
 * @return false 
 */
bool parse_parameters(lexer_T *lexer, DLL *dll, symtables tables, function *func);

/**
 * @brief Parses the assignment of a expression 
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @param type 
 * @return true 
 * @return false 
 */
bool parse_assignment_prime(lexer_T *lexer, DLL *dll, symtables tables, data_type *type);

/**
 * @brief Parses the assignment of a function call or expression
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @return true 
 * @return false 
 */
bool parse_assignment(lexer_T *lexer, DLL *dll, symtables tables);

/**
 * @brief Parses the main body component of a program and simulates local scope
 * 
 * @param lexer 
 * @param dll 
 * @param tables 
 * @return true 
 * @return false 
 */
bool parse_body(lexer_T *lexer, DLL *dll, symtables tables);

/**
 * @brief Inicializes the analysis
 * 
 * @param lexer 
 * @param dll 
 * @return true 
 * @return false 
 */
bool run_analysis(lexer_T *lexer, DLL *dll);

void preload_hash_table(hash_table table);


#endif