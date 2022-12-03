#include "analysis.h"
#include "expressions.h"

extern error ERROR;

bool accept(token *token_ptr, token_ID acceptedID) {
    if (acceptedID == token_ptr->ID) {
        return true;
    }
    return false;
}

bool expect(token *token_ptr, token_ID acceptedID) {
    if (acceptedID == token_ptr->ID) {
        return true;
    }
    // printf("Unexpected token_ptr parsed in syntax analisys\n");
    // printf("Expected %d", acceptedID);
    // printf("got %d\n", token_ptr->ID);
    return false;
}

bool run_analysis(lexer_T *lexer, DLL *dll) {

    hash_table global = init_hash_table();
    hash_table local = init_hash_table();

    symtables tables = {global, local};

    bool var = parse_body(lexer, dll, tables);

    debug_print_table(global);

    hash_table_free(tables.global);

    hash_table_free(tables.local);




    return var;
}

bool parse_arguments_prime(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;
    if (accept(token_ptr, TOKEN_ID_COMMA)) {
        next_tok;
        if (expect(token_ptr, TOKEN_ID_VARIABLE)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return false;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_INTEGER)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return false;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_STRING)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return false;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_DOUBLE)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return false;
            }
        }

        else {
            return_tok;
            return false;
        }
    }
    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_arguments(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;

    // case Function
    if (accept(token_ptr, TOKEN_ID_VARIABLE)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_INTEGER)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_STRING)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_DOUBLE)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }

    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_type(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;
    if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_VOID) {
        return true;
    }
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && ((token_ptr->VAL.keyword == KW_INT) || (token_ptr->VAL.keyword == KW_INT_NULL))) {
        return true;
    }
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && ((token_ptr->VAL.keyword == KW_FLOAT) || (token_ptr->VAL.keyword == KW_FLOAT_NULL))) {
        return true;
    }
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && ((token_ptr->VAL.keyword == KW_STRING) || (token_ptr->VAL.keyword == KW_STRING_NULL))) {
        return true;
    }
    else {
        return_tok;
        return false;
    }
}

bool parse_parameters_prime(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;
    if (accept(token_ptr, TOKEN_ID_COMMA)) {
        if (!parse_type(lexer, dll, tables)) {
            return_error;
        }
        next_tok;
        if (expect(token_ptr, TOKEN_ID_VARIABLE)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return_error;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_INTEGER)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return_error;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_STRING)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return_error;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_DOUBLE)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return_error;
            }
        }

        else {
            return_tok;
            return_error;
        }
    }
    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_parameters(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;
    if (!parse_type(lexer, dll, tables)) {
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {

            return_error;
        }
        else {
            return_tok;
            return true;
        }
    }
    next_tok;

    // case Function
    if (accept(token_ptr, TOKEN_ID_VARIABLE)) {
        if (!parse_parameters_prime(lexer, dll, tables)) {
            return_error;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_INTEGER)) {
        if (!parse_parameters_prime(lexer, dll, tables)) {
            return_error;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_STRING)) {
        if (!parse_parameters_prime(lexer, dll, tables)) {
            return_error;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_DOUBLE)) {
        if (!parse_parameters_prime(lexer, dll, tables)) {
            return_error;
        }
    }

    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_assignment_prime(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    // TODO add FUNCALL case
    bool r = parse_expression(lexer, dll, tables, false);
    return r;
}

bool parse_assignment(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;
    if (!accept(token_ptr, TOKEN_ID_VARIABLE)) {
        return_tok;
        return false;
    }

    // //TODO add variable info to symtable
    if (!hash_table_has_item(tables.global, token_ptr->VAL.string))
    {
            
        table_item_data *data = malloc(sizeof(table_item_data));
        data->name = token_ptr->VAL.string;
        variable *var = malloc(sizeof(variable));
        var->type = NO_TYPE;
        data->f_or_v = var;
        data->is_var=true;
        hash_table_insert(tables.global, data);
    }





    next_tok;
    if (!accept(token_ptr, TOKEN_ID_EQUALS) && !accept(token_ptr, TOKEN_ID_NOT_EQUALS) && !accept(token_ptr, TOKEN_ID_TRIPLE_EQUALS)) {
        return_tok;
        return_tok;
        return false;
    }
    next_tok;
    if (accept(token_ptr, TOKEN_ID_IDENTIFIER)) {
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error;
        }
        if (!parse_arguments(lexer, dll, tables)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            return_error;
        }
        if (!parse_body(lexer, dll, tables)) {
            return_error;
        }
    }
    else {
        return_tok;
        if (!parse_assignment_prime(lexer, dll, tables)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            return_error;
        }
    }

    return true;
}

bool parse_body(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;

    // case Function
    if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_FUNCTION) { //  function
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_IDENTIFIER)) {
            return_error;
        } //  func_id
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error;
        } //  (
        if (!parse_parameters(lexer, dll, tables)) {
            return_error;
        } //  parameters
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error;
        } //  )
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_COLON)) {
            return_error;
        } //  :
        if (!parse_type(lexer, dll, tables)) {
            return_error;
        } //  type
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        } //  {
        if (!parse_body(lexer, dll, tables)) {
            return_error;
        } //  body
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        } //  }

        // TODO add function info to symtable

        if (!parse_body(lexer, dll, tables)) {
            return_error;
        } //  body
    }

    // case If
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_IF) { //  if
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error;
        } //  (
        if (!parse_expression(lexer, dll, tables, true)) {
            return_error;
        } //  expresion
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error;
        } //  )
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        } //  {
        if (!parse_body(lexer, dll, tables)) {
            return_error;
        } //  body
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        } //  }
        next_tok;
        if (!accept(token_ptr, TOKEN_ID_KEYWORD) || token_ptr->VAL.keyword != KW_ELSE) {
            return_error;
        } //  else
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        } //  {
        if (!parse_body(lexer, dll, tables)) {
            return_error;
        } //  body
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        } //  }
        if (!parse_body(lexer, dll, tables)) {
            return_error;
        } //  body
    }

    // case While
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_WHILE) { //  while
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error;
        } //  (
        if (!parse_expression(lexer, dll, tables, true)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        }
        if (!parse_body(lexer, dll, tables)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        }
        if (!parse_body(lexer, dll, tables)) {
            return_error;
        }
    }

    // case Return
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_RETURN) {
        // TODO add return
        next_tok;
        if (expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            if (!parse_body(lexer, dll, tables)) {
                return_error;
            }
        }
        else {
            return_tok;
            if (!parse_expression(lexer, dll, tables, false)) {
                return_error;
            }
            next_tok;
            if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
                return_error;
            }
            else {
                if (!parse_body(lexer, dll, tables)) {
                    return_error;
                }
            }
        }
    }

    else if (accept(token_ptr, TOKEN_ID_IDENTIFIER)) {
        // TODO add return
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error;
        }
        if (!parse_arguments(lexer, dll, tables)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error;
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            return_error;
        }

        // TODO add function info to symtable

        if (!parse_body(lexer, dll, tables)) {
            return_error;
        }
    }

    else if (accept(token_ptr, TOKEN_ID_EOF)) {
        return true;
    }

    else {
        return_tok;
        //  case assignment
        if (parse_assignment(lexer, dll, tables)) {
            if (!parse_body(lexer, dll, tables)) {
                return_error;
            }
            return true;
        }
        if (parse_expression(lexer, dll, tables, false)) {
            next_tok;
            if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
                return_error;
            }
            if (!parse_body(lexer, dll, tables)) {
                return_error;
            }

            return true;
        }
        else {
            if (dll->activeElement != NULL /*&& expect(&dll->activeElement->data, TOKEN_ID_SEMICOLLON)*/) {
                if (ERROR == SYNTAX_ERR) {
                    ERROR = SUCCESS;
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return_error;
            }
        }

        return true;
    }

    return true;
}