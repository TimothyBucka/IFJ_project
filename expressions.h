/**
 * Project: IFJ22 Compiler
 * 
 * @file expressions.h
 * @brief Interface for expressions
 * 
 * @authors xmacur09, xbucka00
*/

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "DLL.h"
#include "lexer.h"
#include "semantics.h"
#include "symtable.h"
#include "token.h"
#include "generator.h"


#define NEW_ITEM(new_item, token_ptr, type)    \
    new_item = expr_item_new(token_ptr, type); \
    if (!new_item) {                       \
        expr_stack_free(expr_stack);       \
        return INTERNAL_ERR;               \
    }



#define NEXT_TOKEN                                  \
    ;                                             \
    if (dll->activeElement == dll->lastElement) { \
        token_ptr = calloc(1, sizeof(*token_ptr));        \
        ERROR = lexer_next_token(lexer, token_ptr);   \
        DLL_push(dll, token_ptr);                     \
        COUNTER++;                                         \
        if (ERROR != SUCCESS) {                   \
            return false;                         \
        }                                         \
    }                                             \
    else {                                        \
        DLL_move_active_right(dll);               \
        token_ptr = DLL_get_active(dll);              \
        COUNTER++; \
    }

#define return_tok\
    ;\
    DLL_move_active_left(dll);




#define UNDO_DLL_ACTIVE                    \
    for (size_t i = 0; i < COUNTER; i++) { \
        DLL_move_active_left(dll);         \
    }                                      \
    COUNTER = 0;

typedef enum {
    TERM,
    NONTERM,
    DOLLAR
} expr_item_type;




typedef struct expr_item {
    token *token_ptr;
    expr_item_type type;
    data_type data_type; // non terminal only
    bool breakpoint;
    struct expr_item *next_item;
} expr_item;

typedef struct {
    int size;
    expr_item *top_item;
} expr_stack;

expr_item *expr_item_new(token *, expr_item_type);

expr_stack *expr_stack_new();
void expr_stack_push(expr_stack *, expr_item *);
expr_item *expr_stack_pop(expr_stack *);
void expr_stack_free(expr_stack *);

char get_precedence(token *, token *);

int count_breakpoint(expr_stack *);

data_type get_data_type_from_item(expr_item *, expr_item *, expr_item *, symtables);

bool apply_rule(expr_stack *, symtables);

expr_item *get_term_or_dollar(expr_stack *);

bool parse_expression(lexer_T *, DLL *,symtables, data_type *, bool);

// gloabal table
static const char prec_table[8][8] = {
    //    0     1     2    3    4    5    6    7
    //    $     (     )    +    *    .    r    i
    {'\0', '<', '\0', '<', '<', '<', '<', '<'}, // $ - 0
    {'\0', '<', '=', '<', '<', '<', '<', '<'},  // ( - 1
    {'>', '\0', '>', '>', '>', '>', '>', '\0'}, // ) - 2
    {'>', '<', '>', '>', '<', '\0', '>', '<'},  // + - 3
    {'>', '<', '>', '>', '>', '\0', '>', '<'},  // * - 4
    {'>', '<', '>', '\0', '\0', '>', '>', '<'}, // . - 5
    {'>', '<', '>', '<', '<', '<', '\0', '<'},  // r - 6
    {'>', '\0', '>', '>', '>', '>', '>', '\0'}  // i - 7
};

#endif
