
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int ended = 0;

    lexer_T *lexer = lexer_init();

    printf("\n------------------------------\n");

    while (ended == 0) {
        token Token;
        lexer_next_token(lexer, &Token, &ended);
        if (Token.ID == 5) {
            printf("TOKEN --- type:%d   value:%d\n", Token.ID, Token.VAL);
        }
        else if (Token.ID < 5) {
            printf("TOKEN --- type:%d   value:%s\n", Token.ID, Token.VAL);
        }
        else {
            printf("TOKEN --- type:%d   value:---\n", Token.ID);
        }
    }

    return 0;
}