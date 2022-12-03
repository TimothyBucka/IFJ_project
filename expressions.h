#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "DLL.h"
#include "lexer.h"
#include "semantics.h"
#include "stack.h"
#include "symtable.h"
#include "token.h"

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

typedef enum {
    PAR_E_PAR,  // E -> (E)
    E_PLUS_E,   // E -> E + E
    E_MINUS_E,  // E -> E - E
    E_TIMES_E,  // E -> E * E
    E_DIVIDE_E, // E -> E / E
    E_CONCAT_E, // E -> E . E
    E_EQ_E,     // E -> E === E
    E_NEQ_E,    // E -> E !== E
    E_LT_E,     // E -> E < E
    E_GT_E,     // E -> E > E
    E_LEQ_E,    // E -> E <= E
    E_GEQ_E,    // E -> E >= E
    ID,         // E -> id
    NONE        // No rule
} rules;


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

data_type get_data_type_from_item(expr_item *, expr_item *, expr_item *);

bool apply_rule(expr_stack *);

expr_item *get_term_or_dollar(expr_stack *);

bool parse_expression(lexer_T *, DLL *,symtables, bool); // TODO symtable argument

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
