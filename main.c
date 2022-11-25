
#include "DLL.h"
#include "expressions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define return_error;       \
    if (ERROR == SUCCESS) { \
        ERROR = SYNTAX_ERR; \
    }                       \
    return false;

bool parse_body(lexer_T *lexer, DLL *dll);

#define next_tok                                  \
    ;                                             \
    if (dll->activeElement == dll->lastElement) { \
        ERROR = lexer_next_token(lexer, token);   \
        DLL_push(dll, token);                     \
        if (ERROR != SUCCESS) {                   \
            return false;                         \
        }                                         \
    }                                             \
    else {                                        \
        DLL_move_active_right(dll);               \
        token = DLL_get_active(dll);              \
    }



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
    printf("Unexpected token parsed in syntax analisys\n");
    printf("Expected %d", acceptedID);
    printf("got %d\n", token->ID);
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
    else if (accept(token, TOKEN_ID_KEYWORD) && ((token->VAL.keyword == KW_INT) || (token->VAL.keyword ==KW_INT_NULL))) {
        return true;
    }
    else if (accept(token, TOKEN_ID_KEYWORD) && ((token->VAL.keyword == KW_FLOAT) || (token->VAL.keyword ==KW_FLOAT_NULL))) {
        return true;
    }
    else if (accept(token, TOKEN_ID_KEYWORD) && ((token->VAL.keyword == KW_STRING) || (token->VAL.keyword ==KW_STRING_NULL))) {
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
    if(!parse_type(lexer, dll)){
        next_tok;
        if(!expect(token, TOKEN_ID_RBRACKET)){

            return_error;
        }
        else{
            DLL_move_active_left(dll);
            return true;
        }
    }
    next_tok;

    // case Function
    if (accept(token, TOKEN_ID_VARIABLE)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return_error;
        }
    }
    else if (accept(token, TOKEN_ID_INTEGER)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return_error;
        }
    }
    else if (accept(token, TOKEN_ID_STRING)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return_error;
        }
    }
    else if (accept(token, TOKEN_ID_DOUBLE)) {
        if (!parse_arguments_prime(lexer, dll)) {
            return_error;
        }
    }

    else {
        DLL_move_active_left(dll);
        return true;
    }
    return true;
}



bool parse_local_scope(lexer_T *lexer, DLL *dll) {
    return true;
}

bool parse_assignment_prime(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    // TODO add FUNCALL case

    error r = parse_expresion(lexer, dll, false);
    return r;
}

bool parse_assignment(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;
    if (!accept(token, TOKEN_ID_VARIABLE)) {
        DLL_move_active_left(dll);
        return false;
    }
    next_tok;
    if (!accept(token, TOKEN_ID_EQUALS)) {
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
        if (!parse_assignment_prime(lexer, dll)) {
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
        if (!parse_expresion(lexer, dll, true)) {
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
        if (!parse_expresion(lexer, dll, true)) {
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
    }

    // TODO case Function call
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
        if (!parse_body(lexer, dll)) {
            return_error;
        }
    }
    else if (accept(token, TOKEN_ID_EOF)) {
        return true;
    }

    else {
        //DLL_move_active_left(dll);
        // case assignment
        if (parse_assignment(lexer, dll)) {
        }
        else {
            DLL_move_active_left(dll);
            return true;
        }
    }

    return true;
}

int main() {

    lexer_T *lexer = lexer_init();

    DLL *dll = createDLL();
    if (parse_body(lexer, dll)) {
        printf("PARSED\n");
    }
    else {
        printf("FAILED TO PARSE\n");
        printf("error : %d", ERROR);
    }

    return ERROR;
}