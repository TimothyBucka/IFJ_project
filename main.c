
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"


int main(){
    char* src = "fd 1.2 /*l**/ abc";
    int ended = 0;
    lexer_T* lexer = lexer_init(src);

    while (ended == 0)
    {
        token Token;
        lexer_next_token(lexer, &Token, &ended);
        if (Token.ID == 5){
            printf("TOKEN --- type:%d   value:%d\n",Token.ID, Token.VAL);
        }
        else if (Token.ID <5){
            printf("TOKEN --- type:%d   value:%s\n",Token.ID, Token.VAL);
        }
        else{
            printf("TOKEN --- type:%d   value:---\n",Token.ID);
        }
    }
    

    
    return 0;
}