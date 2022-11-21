
#include "stack.h"
#include "expressions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define next_tok;     if(stack->size ==0){ ERROR = lexer_next_token(lexer, token, &ended);} else{token = stack_pop(stack);}

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
    printf("Expected %s, got %s\n", acceptedID, token->ID);
    return false;
}

bool parse_paremeters(lexer_T *lexer) {
    return true;
}

bool parse_type(lexer_T *lexer) {
    return true;
}

bool parse_local_scope(lexer_T *lexer) {
    return true;
}


bool parse_body(lexer_T *lexer, stack *stack) {
    int ended = 0;
    token *tmp = calloc(1, sizeof(token));
    token *token = calloc(1, sizeof(token));

    error ERROR;

    next_tok;

    if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_FUNCTION) {
        next_tok;
        if (expect(token, TOKEN_ID_IDENTIFIER)) {
            next_tok;
            if (expect(token, TOKEN_ID_LBRACKET)) {
                if (parse_paremeters(lexer)) {
                    next_tok;
                    if (expect(token, TOKEN_ID_RBRACKET)) {
                        next_tok;
                        if (expect(token, TOKEN_ID_COLON)) {
                            if (parse_type(lexer)) {
                                next_tok;
                                if (expect(token, TOKEN_ID_LCURLYBRACKET)) {
                                    if (parse_body(lexer, stack)) {
                                        next_tok;
                                        if (expect(token, TOKEN_ID_RCURLYBRACKET)) {
                                            parse_body(lexer, stack);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_WHILE) {
        next_tok;
        if (expect(token, TOKEN_ID_LBRACKET)) {
            if (parse_expresion(lexer, stack)) {
                next_tok;
                if (expect(token, TOKEN_ID_RBRACKET)) {
                    next_tok;
                    if (expect(token, TOKEN_ID_LCURLYBRACKET)) {
                        if (parse_local_scope(lexer)) {
                            next_tok;
                            if (expect(token, TOKEN_ID_RCURLYBRACKET)) {
                                parse_body(lexer, stack);
                            }
                        }
                    }
                }
            }
        }
    }
    else if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_RETURN)
    {
        //TODO add return
        next_tok;
        if(expect(token, TOKEN_ID_SEMICOLLON)){
            parse_body(lexer, stack);
        }
    }
    
    else {
        stack_push(stack, token);
        return 1;
    }
}

int main() {
    int ended = 0;

    lexer_T *lexer = lexer_init();
    
    stack *stack = stack_new();

    if (parse_body(lexer, stack)){
        printf("PARSED\n");
    }
    else{
        printf("FAILED TO PARSE\n");
    }

    return 0;
}