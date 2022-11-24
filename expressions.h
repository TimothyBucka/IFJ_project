#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "lexer.h"
#include "stack.h"
#include "symtable.h"
#include "DLL.h"

#define NEW_ITEM(new_item, token, type)    \
    new_item = expr_item_new(token, type); \
    if (!new_item) {                       \
        expr_stack_free(expr_stack);       \
        return INTERNAL_ERR;               \
    }

#define next_tok   if(dll->activeElement == dll->lastElement)\
                    {\
                        ERROR = lexer_next_token(lexer, token);\
                        DLL_push(dll, token);\
                    }\
                    else{DLL_move_active_right(dll);token = DLL_get_active(dll);}

// #define NEXT_TOKEN(lexer, token, ended, error)

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
    token *token;
    expr_item_type type;
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

expr_item *get_term_or_dollar(expr_stack *);

error parse_expresion(lexer_T *, DLL *, bool); // TODO symtable argument

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

static error ERROR = SUCCESS;

#endif
