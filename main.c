
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"


int main(){
    char* src = "fd else fun ====* 1E5 2e-4 2.5e+23 3<4 5>=1.2";
    lexer_T* lexer = lexer_init(src);
    for (size_t i = 0; i < 15; i++)
    {
        token Token;
        lexer_next_token(lexer, &Token);
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