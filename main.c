
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"


int main(){
    char* src = "$aB_c_ $_D88 $0 $ abc 111 1.2";
    lexer_T* lexer = init_lexer(src);
    for (size_t i = 0; i < 8; i++)
    {
        token Token = lexer_next_token(lexer);
        printf("TOKEN --- type:%d   value:%s\n",Token.ID, Token.VAL);
    }
    
    return 0;
}