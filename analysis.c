#include "analysis.h"
#include "expressions.h"

extern error ERROR;

bool accept(token *token, token_ID acceptedID) {
    if (acceptedID == token->ID) {
        return true;
    }
    return false;
}

bool expect(token *token, token_ID acceptedID) {
    if (acceptedID == token->ID) {
        return true;
    }
    // printf("Unexpected token parsed in syntax analisys\n");
    // printf("Expected %d", acceptedID);
    // printf("got %d\n", token->ID);
    return false;
}

bool parse_arguments_prime(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;
    if (accept(token, TOKEN_ID_COMMA)) {
        next_tok;
        if (expect(token, TOKEN_ID_VARIABLE)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return false;
            }
        }
        else if (expect(token, TOKEN_ID_INTEGER)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return false;
            }
        }
        else if (expect(token, TOKEN_ID_STRING)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return false;
            }
        }
        else if (expect(token, TOKEN_ID_DOUBLE)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return false;
            }
        }

        else {
            DLL_move_active_left(dll);
            return false;
        }
    }
    else {
        DLL_move_active_left(dll);
        return true;
    }
    return true;
}

bool parse_arguments(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;

    // case Function
    if (accept(token, TOKEN_ID_VARIABLE)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return false;
        }
    }
    else if (accept(token, TOKEN_ID_INTEGER)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return false;
        }
    }
    else if (accept(token, TOKEN_ID_STRING)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return false;
        }
    }
    else if (accept(token, TOKEN_ID_DOUBLE)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return false;
        }
    }

    else {
        DLL_move_active_left(dll);
        return true;
    }
    return true;
}

bool parse_type(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;
    if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_VOID) {
        return true;
    }
    else if (accept(token, TOKEN_ID_KEYWORD) && ((token->VAL.keyword == KW_INT) || (token->VAL.keyword == KW_INT_NULL))) {
        return true;
    }
    else if (accept(token, TOKEN_ID_KEYWORD) && ((token->VAL.keyword == KW_FLOAT) || (token->VAL.keyword == KW_FLOAT_NULL))) {
        return true;
    }
    else if (accept(token, TOKEN_ID_KEYWORD) && ((token->VAL.keyword == KW_STRING) || (token->VAL.keyword == KW_STRING_NULL))) {
        return true;
    }
    else {
        DLL_move_active_left(dll);
        return false;
    }
}

bool parse_parameters_prime(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;
    if (accept(token, TOKEN_ID_COMMA)) {
        if (!parse_type(lexer, dll)) {
            return_error;
        }
        next_tok;
        if (expect(token, TOKEN_ID_VARIABLE)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return_error;
            }
        }
        else if (expect(token, TOKEN_ID_INTEGER)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return_error;
            }
        }
        else if (expect(token, TOKEN_ID_STRING)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return_error;
            }
        }
        else if (expect(token, TOKEN_ID_DOUBLE)) {
            if (!parse_arguments_prime(lexer, dll)) {
                return_error;
            }
        }

        else {
            DLL_move_active_left(dll);
            return_error;
        }
    }
    else {
        DLL_move_active_left(dll);
        return true;
    }
    return true;
}

bool parse_parameters(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));
    if (!parse_type(lexer, dll)) {
        next_tok;
        if (!expect(token, TOKEN_ID_RBRACKET)) {

            return_error;
        }
        else {
            DLL_move_active_left(dll);
            return true;
        }
    }
    next_tok;

    // case Function
    if (accept(token, TOKEN_ID_VARIABLE)) {
        if (!parse_parameters_prime(lexer, dll)) {
            return_error;
        }
    }
    else if (accept(token, TOKEN_ID_INTEGER)) {
        if (!parse_parameters_prime(lexer, dll)) {
            return_error;
        }
    }
    else if (accept(token, TOKEN_ID_STRING)) {
        if (!parse_parameters_prime(lexer, dll)) {
            return_error;
        }
    }
    else if (accept(token, TOKEN_ID_DOUBLE)) {
        if (!parse_parameters_prime(lexer, dll)) {
            return_error;
        }
    }

    else {
        DLL_move_active_left(dll);
        return true;
    }
    return true;
}

bool parse_assignment_prime(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    // TODO add FUNCALL case
    bool r = parse_expression(lexer, dll, false);
    return r;
}

bool parse_assignment(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;
    if (!accept(token, TOKEN_ID_VARIABLE)) {
        DLL_move_active_left(dll);
        return false;
    }

    // TODO add variable info to symtable

    next_tok;
    if (!accept(token, TOKEN_ID_EQUALS) && !accept(token, TOKEN_ID_NOT_EQUALS) && !accept(token, TOKEN_ID_TRIPLE_EQUALS)) {
        DLL_move_active_left(dll);
        DLL_move_active_left(dll);
        return false;
    }
    next_tok;
    if (accept(token, TOKEN_ID_IDENTIFIER)) {
        next_tok;
        if (!expect(token, TOKEN_ID_LBRACKET)) {
            return_error;
        }
        if (!parse_arguments(lexer, dll)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_RBRACKET)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_SEMICOLLON)) {
            return_error;
        }
        if (!parse_body(lexer, dll)) {
            return_error;
        }
    }
    else {
        DLL_move_active_left(dll);
        if (!parse_assignment_prime(lexer, dll)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_SEMICOLLON)) {
            return_error;
        }
    }

    return true;
}

bool parse_body(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;

    // case Function
    if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_FUNCTION) { //  function
        next_tok;
        if (!expect(token, TOKEN_ID_IDENTIFIER)) {
            return_error;
        } //  func_id
        next_tok;
        if (!expect(token, TOKEN_ID_LBRACKET)) {
            return_error;
        } //  (
        if (!parse_parameters(lexer, dll)) {
            return_error;
        } //  parameters
        next_tok;
        if (!expect(token, TOKEN_ID_RBRACKET)) {
            return_error;
        } //  )
        next_tok;
        if (!expect(token, TOKEN_ID_COLON)) {
            return_error;
        } //  :
        if (!parse_type(lexer, dll)) {
            return_error;
        } //  type
        next_tok;
        if (!expect(token, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        } //  {
        if (!parse_body(lexer, dll)) {
            return_error;
        } //  body
        next_tok;
        if (!expect(token, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        } //  }

        // TODO add function info to symtable

        if (!parse_body(lexer, dll)) {
            return_error;
        } //  body
    }

    // case If
    else if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_IF) { //  if
        next_tok;
        if (!expect(token, TOKEN_ID_LBRACKET)) {
            return_error;
        } //  (
        if (!parse_expression(lexer, dll, true)) {
            return_error;
        } //  expresion
        next_tok;
        if (!expect(token, TOKEN_ID_RBRACKET)) {
            return_error;
        } //  )
        next_tok;
        if (!expect(token, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        } //  {
        if (!parse_body(lexer, dll)) {
            return_error;
        } //  body
        next_tok;
        if (!expect(token, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        } //  }
        next_tok;
        if (!accept(token, TOKEN_ID_KEYWORD) || token->VAL.keyword != KW_ELSE) {
            return_error;
        } //  else
        next_tok;
        if (!expect(token, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        } //  {
        if (!parse_body(lexer, dll)) {
            return_error;
        } //  body
        next_tok;
        if (!expect(token, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        } //  }
        if (!parse_body(lexer, dll)) {
            return_error;
        } //  body
    }

    // case While
    else if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_WHILE) { //  while
        next_tok;
        if (!expect(token, TOKEN_ID_LBRACKET)) {
            return_error;
        } //  (
        if (!parse_expression(lexer, dll, true)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_RBRACKET)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_LCURLYBRACKET)) {
            return_error;
        }
        if (!parse_body(lexer, dll)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_RCURLYBRACKET)) {
            return_error;
        }
        if (!parse_body(lexer, dll)) {
            return_error;
        }
    }

    // case Return
    else if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_RETURN) {
        // TODO add return
        next_tok;
        if (expect(token, TOKEN_ID_SEMICOLLON)) {
            if (!parse_body(lexer, dll)) {
                return_error;
            }
        }
        else {
            DLL_move_active_left(dll);
            if (!parse_expression(lexer, dll, false)) {
                return_error;
            }
            next_tok;
            if (!expect(token, TOKEN_ID_SEMICOLLON)) {
                return_error;
            }
            else {
                if (!parse_body(lexer, dll)) {
                    return_error;
                }
            }
        }
    }

    else if (accept(token, TOKEN_ID_IDENTIFIER)) {
        // TODO add return
        next_tok;
        if (!expect(token, TOKEN_ID_LBRACKET)) {
            return_error;
        }
        if (!parse_arguments(lexer, dll)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_RBRACKET)) {
            return_error;
        }
        next_tok;
        if (!expect(token, TOKEN_ID_SEMICOLLON)) {
            return_error;
        }

        // TODO add function info to symtable

        if (!parse_body(lexer, dll)) {
            return_error;
        }
    }

    else if (accept(token, TOKEN_ID_EOF)) {
        return true;
    }

    else {
        DLL_move_active_left(dll);
        //  case assignment
         if (parse_assignment(lexer, dll)) {
            if (!parse_body(lexer, dll)) {
                return_error;
            }
            return true;
        }
        else {
            if (ERROR != LEXICAL_ERR) {
                ERROR = SUCCESS;
            }
            else {
                return false;
            }
        }
        if (parse_expression(lexer, dll, false)) {
            next_tok;
            if (!expect(token, TOKEN_ID_SEMICOLLON)) {
                return_error;
            }
            if (!parse_body(lexer, dll)) {
                return_error;
            }

            return true;
        }
        else {
            if (dll->activeElement != NULL && expect(&dll->activeElement->data, TOKEN_ID_SEMICOLLON)) {
                if (ERROR != LEXICAL_ERR) {
                    ERROR = SUCCESS;
                    return true;
                } else {
                    return false;
                }
            } else {
                return_error;
            }
        }

        return true;
    }

    return true;
}