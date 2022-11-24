#include "expressions.h"

expr_item *expr_item_new(token *token, expr_item_type type) {
    expr_item *item = malloc(sizeof(*item));
    item->token = token;
    item->type = type;
    item->breakpoint = false;
    item->next_item = NULL;
    return item;
}

expr_stack *expr_stack_new() {
    expr_stack *stack = malloc(sizeof(*stack));
    if (!stack)
        return NULL;
    stack->size = 0;
    stack->top_item = NULL;
    return stack;
}

void expr_stack_push(expr_stack *stack, expr_item *item) {
    if (stack->top_item == NULL) {
        stack->top_item = item;
    }
    else {
        item->next_item = stack->top_item;
        stack->top_item = item;
    }
    stack->size++;
}

expr_item *expr_stack_pop(expr_stack *stack) {
    if (stack->top_item == NULL) {
        return NULL;
    }
    else {
        expr_item *item = stack->top_item;
        stack->top_item = item->next_item;
        stack->size--;
        return item;
    }
}

void expr_stack_free(expr_stack *stack) {
    expr_item *item = stack->top_item;
    while (item != NULL) {
        expr_item *next_item = item->next_item;
        free(item);
        item = next_item;
    }
    free(stack);
}

int get_index_token(token *token) {
    if (!token) { // dollar
        return 0;
    }

    switch (token->ID) {
    case TOKEN_ID_LBRACKET:
        return 1;
    case TOKEN_ID_RBRACKET:
        return 2;
    case TOKEN_ID_PLUS:
    case TOKEN_ID_MINUS:
        return 3;
    case TOKEN_ID_MULTIPLICATION:
    case TOKEN_ID_DIVISION:
        return 4;
    case TOKEN_ID_CONCAT:
        return 5;
    case TOKEN_ID_TRIPLE_EQUALS:
    case TOKEN_ID_NOT_EQUALS:
    case TOKEN_ID_LT:
    case TOKEN_ID_GT:
    case TOKEN_ID_LTE:
    case TOKEN_ID_GTE:
        return 6;
    case TOKEN_ID_IDENTIFIER:
    case TOKEN_ID_VARIABLE:
    case TOKEN_ID_INTEGER:
    case TOKEN_ID_DOUBLE:
    case TOKEN_ID_STRING:
        return 7;
    default:
        return 0;
    }
}

char get_precedence(token *token_on_stack, token *token) {
    int index[2] = {0, 0};

    index[0] = get_index_token(token_on_stack);
    index[1] = get_index_token(token);

    if (index[0] == -1 || index[1] == -1) {
        return '\0';
    }

    return prec_table[index[0]][index[1]];
}

expr_item *get_term_or_dollar(expr_stack *expr_stack) {
    expr_item *item = expr_stack->top_item;
    while (item->type != TERM && item->type != DOLLAR) {
        item = item->next_item;
    }
    return item;
}

error parse_expresion(lexer_T *lexer, DLL *dll, bool exp_brack) {
    token *token = calloc(1, sizeof(token));
    expr_item *new_item;
    expr_stack *expr_stack = expr_stack_new();
    if (!expr_stack)
        return INTERNAL_ERR;

    NEW_ITEM(new_item, NULL, DOLLAR);
    expr_stack_push(expr_stack, new_item);

    next_tok;
    if (ERROR) {
        expr_stack_free(expr_stack);
        return ERROR;
    }

    do {
        expr_item *stack_term = get_term_or_dollar(expr_stack);
        switch (get_precedence(stack_term->token, token)) {
        case '=':
            NEW_ITEM(new_item, token, TERM);
            expr_stack_push(expr_stack, new_item);
            next_tok;
            if (ERROR) {
                expr_stack_free(expr_stack);
                return ERROR;
            }
            break;
        case '<':
            stack_term->breakpoint = true;
            NEW_ITEM(new_item, token, TERM);
            expr_stack_push(expr_stack, new_item);
            next_tok;
            if (ERROR) {
                expr_stack_free(expr_stack);
                return ERROR;
            }
            break;
        case '>':
            // TODO now
            break;
        case '\0':
            expr_stack_free(expr_stack);
            return SYNTAX_ERR;
        }
    } while (1);


    return SUCCESS;
}