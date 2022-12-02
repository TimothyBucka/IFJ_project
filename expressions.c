#include "expressions.h"

extern error ERROR;

size_t COUNTER = 0;

expr_item *expr_item_new(token *token, expr_item_type type) {
    expr_item *item = malloc(sizeof(*item));
    item->token = token;
    item->type = type;
    item->data_type = NONE;
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
    if (!stack->top_item) {
        stack->top_item = item;
    }
    else {
        item->next_item = stack->top_item;
        stack->top_item = item;
    }
    stack->size++;
}

expr_item *expr_stack_pop(expr_stack *stack) {
    if (!stack->top_item) {
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
    while (item) {
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

int count_breakpoint(expr_stack *expr_stack) {
    int count = 0;
    expr_item *item = expr_stack->top_item;
    while (item) {
        if (!item->breakpoint) {
            count++;
        }
        else {
            item->breakpoint = false;
            break;
        }
        item = item->next_item;
    }
    return count;
}

data_type get_data_type_from_item(expr_item *item_right, expr_item *item_middle, expr_item *item_left) {
    if (item_right->type == TERM) {
        if (item_right->token->ID == TOKEN_ID_VARIABLE) {
            return INT; // TODO search symtable if not found return NONE -- error 5
        }
        else if (item_right->token->ID == TOKEN_ID_INTEGER) {
            return INT;
        }
        else if (item_right->token->ID == TOKEN_ID_DOUBLE) {
            return DOUBLE;
        }
        else if (item_right->token->ID == TOKEN_ID_STRING) {
            return STRING;
        }
    }
    else if (item_middle->type == NONTERM) {
        return item_middle->data_type;
    }
    else {
        data_type right = item_right->data_type;
        data_type left = item_left->data_type;

        switch (item_middle->token->ID) {
        case TOKEN_ID_PLUS:
        case TOKEN_ID_MINUS:
        case TOKEN_ID_MULTIPLICATION:
            if (right == INT && left == INT) {
                return INT;
            }
            else if (right == DOUBLE || left == DOUBLE) {
                return DOUBLE;
            }
            else {
                ERROR = EXPR_ERR;
                return NONE;
            }
            break;
        case TOKEN_ID_DIVISION:
            if ((right != INT && right != DOUBLE) || (left != INT && left != DOUBLE)) {
                ERROR = EXPR_ERR;
                return NONE;
            }
            else {
                return DOUBLE;
            }
            break;
        case TOKEN_ID_CONCAT:
            if (right == STRING && left == STRING) {
                return STRING;
            }
            else {
                ERROR = EXPR_ERR;
                return NONE;
            }
            break;
        case TOKEN_ID_TRIPLE_EQUALS:
        case TOKEN_ID_NOT_EQUALS:
        case TOKEN_ID_LT:
        case TOKEN_ID_GT:
        case TOKEN_ID_LTE:
        case TOKEN_ID_GTE:
            return BOOL_TYPE;
            break;
        default:
            return NONE;
        }
    }
    return NONE;
}

bool apply_rule(expr_stack *expr_stack) {
    int number_of_items = count_breakpoint(expr_stack);

    bool ret_val = false;

    expr_item *item_right = NULL;
    expr_item *item_middle = NULL;
    expr_item *item_left = NULL;

    if (number_of_items == 1) {
        item_right = expr_stack_pop(expr_stack);
        if (item_right->type == TERM && get_index_token(item_right->token) == 7) { // E -> id
            ret_val = true;
        }
        else {
            ret_val = false;
        }
    }
    else if (number_of_items == 3) {
        item_right = expr_stack_pop(expr_stack);
        item_middle = expr_stack_pop(expr_stack);
        item_left = expr_stack_pop(expr_stack);

        if (item_left->type == NONTERM && item_middle->type == TERM && item_right->type == NONTERM) {
            if (item_middle->token->ID == TOKEN_ID_PLUS) { // E -> E + E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_MINUS) { // E -> E - E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_MULTIPLICATION) { // E -> E * E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_DIVISION) { // E -> E / E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_CONCAT) { // E -> E . E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_TRIPLE_EQUALS) { // E -> E == E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_NOT_EQUALS) { // E -> E !== E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_LT) { // E -> E < E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_GT) { // E -> E > E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_LTE) { // E -> E <= E
                ret_val = true;
            }
            else if (item_middle->token->ID == TOKEN_ID_GTE) { // E -> E >= E
                ret_val = true;
            }
            else {
                ret_val = false;
            }
        }
        else if ((item_right->type == TERM && get_index_token(item_right->token) == 2) &&
                 (item_left->type == TERM && get_index_token(item_left->token) == 1) &&
                 (item_middle->type == NONTERM)) { // E -> (E)
            ret_val = true;
        }
        else {
            ret_val = false;
        }
    }
    else {
        return false;
    }

    if (ret_val) {
        item_right->data_type = get_data_type_from_item(item_right, item_middle, item_left);
        item_right->type = NONTERM;
        item_right->token = NULL;
        expr_stack_push(expr_stack, item_right);
    }

    if (item_middle) {
        free(item_middle);
        free(item_left);
    }

    return ret_val;
}

expr_item *get_term_or_dollar(expr_stack *expr_stack) {
    expr_item *item = expr_stack->top_item;
    while (item->type != TERM && item->type != DOLLAR) {
        item = item->next_item;
    }
    return item;
}

bool parse_expression(lexer_T *lexer, DLL *dll, bool exp_brack) {
    // printf("Parsing expression...\n");
    token *token = calloc(1, sizeof(token));
    expr_item *new_item;
    expr_stack *expr_stack = expr_stack_new();
    if (!expr_stack) {
        ERROR = INTERNAL_ERR;
        return false;
    }

    NEW_ITEM(new_item, NULL, DOLLAR);
    expr_stack_push(expr_stack, new_item);

    NEXT_TOKEN;
    if (ERROR) {
        expr_stack_free(expr_stack);
        UNDO_DLL_ACTIVE;
        return false;
    }

    do {
        expr_item *stack_term = get_term_or_dollar(expr_stack);
        switch (get_precedence(stack_term->token, token)) {
        case '=':
            NEXT_TOKEN;
            NEW_ITEM(new_item, &dll->activeElement->previousElement->data, TERM);
            expr_stack_push(expr_stack, new_item);
            if (ERROR) {
                expr_stack_free(expr_stack);
                UNDO_DLL_ACTIVE;
                return false;
            }
            break;
        case '<':
            stack_term->breakpoint = true;
            NEXT_TOKEN;

            NEW_ITEM(new_item, &dll->activeElement->previousElement->data, TERM);
            expr_stack_push(expr_stack, new_item);
            if (ERROR) {
                expr_stack_free(expr_stack);
                UNDO_DLL_ACTIVE;
                return false;
            }
            break;
        case '>':
            if (!apply_rule(expr_stack)) {
                ERROR = SYNTAX_ERR;
                // printf("BAD expresoizn\n");
                expr_stack_free(expr_stack);
                UNDO_DLL_ACTIVE;
                return false;
            }
            if (ERROR == EXPR_ERR) {
                expr_stack_free(expr_stack);
                UNDO_DLL_ACTIVE;
                return false;
            }
            if (expr_stack->top_item->type == NONTERM && expr_stack->top_item->next_item->type == DOLLAR && get_index_token(token) == 0) {
                if (exp_brack) {
                    ERROR = SYNTAX_ERR;
                    // printf("BAD expresoizn\n");
                    expr_stack_free(expr_stack);
                    UNDO_DLL_ACTIVE;
                    return false;
                }
                printf("Type of expression: %d\n", expr_stack->top_item->data_type);
                expr_stack_free(expr_stack);
                // printf(":D GOOOT expresoizn\n");
                DLL_move_active_left(dll); // FIXME asi si zabudol mi posunut aktivny spat o jeden ak to dobre chapem, tak som to snad opravil
                COUNTER = 0;
                return true; // TODO @Timo
            }
            break;
        case '\0':
            if (exp_brack && token->ID == TOKEN_ID_RBRACKET) {
                NEXT_TOKEN;
                if (ERROR) {
                    expr_stack_free(expr_stack);
                    UNDO_DLL_ACTIVE;
                    return false;
                }
                if (get_index_token(token) == 0) { // not an expression
                    printf("Type of expression: %d\n", expr_stack->top_item->data_type);
                    expr_stack_free(expr_stack);
                    DLL_move_active_left(dll);
                    DLL_move_active_left(dll);
                    // printf(":D GOOOT expresoizn\n");
                    COUNTER = 0;

                    return true;
                }
                else {
                    ERROR = SYNTAX_ERR;
                    // printf("BAD expresoizn\n");
                    expr_stack_free(expr_stack);
                    UNDO_DLL_ACTIVE;
                    return false;
                }
            }
            expr_stack_free(expr_stack);
            ERROR = SYNTAX_ERR;
            // printf("BAD expresoizn\n");
            UNDO_DLL_ACTIVE;
            return false;
        }
    } while (1);
}