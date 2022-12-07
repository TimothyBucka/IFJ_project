/**
 * Project: IFJ22 Compiler
 * 
 * @file expressions.c
 * @brief Implementation of expression parser
 * 
 * @authors xmacur09, xbucka00
*/

#include "expressions.h"

extern error ERROR;
extern int ERRORFROMLEXER;
extern int BODYRECURSIONCOUNT;

size_t COUNTER = 0;

expr_item *expr_item_new(token *token_ptr, expr_item_type type) { // creates new item
    expr_item *item = malloc(sizeof(*item));
    item->token_ptr = token_ptr;
    item->type = type;
    item->data_type = UNDEFINED;
    item->breakpoint = false;
    item->next_item = NULL;
    return item;
}

expr_stack *expr_stack_new() { // creates new stack
    expr_stack *stack = malloc(sizeof(*stack));
    if (!stack)
        return NULL;
    stack->size = 0;
    stack->top_item = NULL;
    return stack;
}

void expr_stack_push(expr_stack *stack, expr_item *item) { // pushes item to top of stack
    if (!stack->top_item) { 
        stack->top_item = item;
    }
    else {
        item->next_item = stack->top_item;
        stack->top_item = item;
    }
    stack->size++;
}

expr_item *expr_stack_pop(expr_stack *stack) { // returns top item from stack
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

void expr_stack_free(expr_stack *stack) { // frees all items in stack
    expr_item *item = stack->top_item;
    while (item) {
        expr_item *next_item = item->next_item;
        free(item);
        item = next_item;
    }
    free(stack);
}

int get_index_token(token *token_ptr) { // returns index of token in precedence table
    if (!token_ptr) { // dollar
        return 0;
    }

    switch (token_ptr->ID) {
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
    case TOKEN_ID_FLOAT:
    case TOKEN_ID_STRING:
        return 7;
    default:
        return 0;
    }
}

data_type get_data_type(token *token_ptr) { // returns data type of token
    switch (token_ptr->ID) {
    case TOKEN_ID_INTEGER:
        return INT;
    case TOKEN_ID_FLOAT:
        return FLOAT;
    case TOKEN_ID_STRING:
        return STRING;
    default:
        return UNDEFINED;
    }
}

char get_precedence(token *token_on_stack, token *token_ptr) { // returns precedence of two tokens
    int index[2] = {0, 0};

    index[0] = get_index_token(token_on_stack);
    index[1] = get_index_token(token_ptr);

    if (index[0] == -1 || index[1] == -1) {
        return '\0';
    }

    return prec_table[index[0]][index[1]];
}

int count_breakpoint(expr_stack *expr_stack) {// counts number of items in stack before breakpoint
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

data_type get_data_type_from_item(expr_item *item_right, expr_item *item_middle, expr_item *item_left, symtables tables) { // returns data type of expression
    if (item_right->type == TERM && item_right->token_ptr->ID != TOKEN_ID_RBRACKET) { // id
        if (item_right->token_ptr->ID == TOKEN_ID_VARIABLE) {
            table_item_data *item = NULL;

            if (BODYRECURSIONCOUNT == 1) {
                item = hash_table_lookup(tables.global, item_right->token_ptr->VAL.string);
            } else {
                item = hash_table_lookup(tables.local, item_right->token_ptr->VAL.string);
            }

            if (!item) {
                item = hash_table_lookup(tables.global, item_right->token_ptr->VAL.string);
            }
            if (!item) {
                ERROR = UNDEFINED_VAR_ERR;
                return UNDEFINED;
            }
            else {
                if (item->is_var) {
                    return item->f_or_v.variable->type;
                }
                else {
                    ERROR = SEM_OTHER_ERR;
                    return UNDEFINED;
                }
            }
            return INT;
        }
        else if (item_right->token_ptr->ID == TOKEN_ID_INTEGER) {
            return INT;
        }
        else if (item_right->token_ptr->ID == TOKEN_ID_FLOAT) {
            return FLOAT;
        }
        else if (item_right->token_ptr->ID == TOKEN_ID_STRING) {
            return STRING;
        }
    }
    else if (item_middle->type == NONTERM) { // (E)
        return item_middle->data_type;
    }
    else { // E something E
        data_type right = item_right->data_type;
        data_type left = item_left->data_type;

        switch (item_middle->token_ptr->ID) {
        case TOKEN_ID_PLUS:
        case TOKEN_ID_MINUS:
        case TOKEN_ID_MULTIPLICATION:
            if (left == INT) {
                if (right == INT || right == NULL_TYPE) {
                    return INT;
                }
                else if (right == FLOAT) {
                    return FLOAT;
                }
                else {
                    ERROR = EXPR_ERR;
                    return UNDEFINED;
                }
            }
            else if (left == FLOAT) {
                if (right == INT || right == FLOAT || right == NULL_TYPE) {
                    return FLOAT;
                }
                else {
                    ERROR = EXPR_ERR;
                    return UNDEFINED;
                }
            }
            else if (left == NULL_TYPE) {
                if (right == INT) {
                    return INT;
                }
                else if (right == FLOAT) {
                    return FLOAT;
                }
                else if (right == NULL_TYPE) {
                    return NULL_TYPE;
                }
                else {
                    ERROR = EXPR_ERR;
                    return UNDEFINED;
                }
            }
            else {
                ERROR = EXPR_ERR;
                return UNDEFINED;
            }
            break;
        case TOKEN_ID_DIVISION:
            if (left == NULL_TYPE && right == NULL_TYPE) {
                ERROR = EXPR_ERR;
                return UNDEFINED;
            } else if ((left == INT || left == FLOAT || left == NULL_TYPE) && (right == INT || right == FLOAT || right == NULL_TYPE)) { 
                return FLOAT;
            }
            else {
                ERROR = EXPR_ERR;
                return UNDEFINED;
            }
            break;
        case TOKEN_ID_CONCAT:
            if ((right == STRING || right == NULL_TYPE) && (left == STRING || left == NULL_TYPE)) {
                return STRING;
            }
            else {
                ERROR = EXPR_ERR;
                return UNDEFINED;
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
            return UNDEFINED;
        }
    }
    return UNDEFINED;
}

bool apply_rule(expr_stack *expr_stack, symtables tables) { // applies rule to stack
    int number_of_items = count_breakpoint(expr_stack);

    bool ret_val = false;

    expr_item *item_right = NULL;
    expr_item *item_middle = NULL;
    expr_item *item_left = NULL;


    rules rule;

    if (number_of_items == 1) {
        item_right = expr_stack_pop(expr_stack);
        if (item_right->type == TERM && get_index_token(item_right->token_ptr) == 7) { // E -> id
            rule = ID;
            operation_rule(rule, item_right->token_ptr);
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
            if (item_middle->token_ptr->ID == TOKEN_ID_PLUS) { // E -> E + E
                rule =  E_PLUS_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_MINUS) { // E -> E - E
                rule = E_MINUS_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_MULTIPLICATION) { // E -> E * E
                rule = E_TIMES_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_DIVISION) { // E -> E / E
                rule = E_DIVIDE_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_CONCAT) { // E -> E . E
                rule = E_CONCAT_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_TRIPLE_EQUALS) { // E -> E == E
                rule = E_EQ_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_NOT_EQUALS) { // E -> E !== E
                rule = E_NEQ_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_LT) { // E -> E < E
                rule = E_LT_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_GT) { // E -> E > E
                rule = E_GT_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_LTE) { // E -> E <= E
                rule = E_LEQ_E;
                ret_val = true;
            }
            else if (item_middle->token_ptr->ID == TOKEN_ID_GTE) { // E -> E >= E
                rule = E_GEQ_E;
                ret_val = true;
            }
            else {
                
                ret_val = false;
            }
            operation_rule(rule, NULL);
        }
        else if ((item_right->type == TERM && get_index_token(item_right->token_ptr) == 2) &&
                 (item_left->type == TERM && get_index_token(item_left->token_ptr) == 1) &&
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
        item_right->data_type = get_data_type_from_item(item_right, item_middle, item_left, tables);
        item_right->type = NONTERM;
        item_right->token_ptr = NULL;
        expr_stack_push(expr_stack, item_right);
    }

    if (item_middle) {
        free(item_middle);
        free(item_left);
    }

    return ret_val;
}

expr_item *get_term_or_dollar(expr_stack *expr_stack) { // returns pointer to top term or dollar
    expr_item *item = expr_stack->top_item;
    while (item->type != TERM && item->type != DOLLAR) {
        item = item->next_item;
    }
    return item;
}

bool parse_expression(lexer_T *lexer, DLL *dll, symtables tables, data_type *final_type, bool exp_brack) { // parses expression
    if (ERROR == LEXICAL_ERR) {
        return false;
    }
    token *token_ptr;
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
        switch (get_precedence(stack_term->token_ptr, token_ptr)) { // precedence table
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
            if (!apply_rule(expr_stack, tables)) {
                ERROR = SYNTAX_ERR;
                // printf("BAD expresoizn\n");
                expr_stack_free(expr_stack);
                UNDO_DLL_ACTIVE;
                return false;
            }
            if (ERROR > 2) { // semantic erro
                expr_stack_free(expr_stack);
                UNDO_DLL_ACTIVE;
                return false;
            }
            if (expr_stack->top_item->type == NONTERM && expr_stack->top_item->next_item->type == DOLLAR && get_index_token(token_ptr) == 0) { 
                if (exp_brack) {
                    ERROR = SYNTAX_ERR;
                    expr_stack_free(expr_stack);
                    UNDO_DLL_ACTIVE;
                    return false;
                }
                *final_type = expr_stack->top_item->data_type;
                expr_stack_free(expr_stack);
                DLL_move_active_left(dll); 
                COUNTER = 0;
                return true; 
            }
            break;
        case '\0':
            if (exp_brack && token_ptr->ID == TOKEN_ID_RBRACKET) {
                NEXT_TOKEN;
                if (ERROR) {
                    expr_stack_free(expr_stack);
                    UNDO_DLL_ACTIVE;
                    return false;
                }
                if (get_index_token(token_ptr) == 0) { // not an expression
                    *final_type = expr_stack->top_item->data_type;
                    expr_stack_free(expr_stack);
                    DLL_move_active_left(dll);
                    DLL_move_active_left(dll);
                    COUNTER = 0;

                    return true;
                }
                else {
                    ERROR = SYNTAX_ERR;
                    expr_stack_free(expr_stack);
                    UNDO_DLL_ACTIVE;
                    return false;
                }
            }
            expr_stack_free(expr_stack);
            ERROR = SYNTAX_ERR;
            UNDO_DLL_ACTIVE;
            return false;
        }
    } while (1);
}