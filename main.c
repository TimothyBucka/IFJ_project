
#include "DLL.h"
#include "expressions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analysis.h"

error ERROR = SUCCESS;
int ERRORFROMLEXER = 0;
int BODYRECURSIONCOUNT = 0;

int main() {

    lexer_T *lexer = lexer_init();

    DLL *dll = createDLL();
    if (run_analysis(lexer, dll)) {
        // //printf("PARSED\n");
    }
    else {
        // //printf("error : %d", ERROR);
        // //printf("FAILED TO PARSE\n");
    }
    
    DLL_free(dll);

    return ERROR;
}