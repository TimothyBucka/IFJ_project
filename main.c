
#include "DLL.h"
#include "expressions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analysis.h"

error ERROR = SUCCESS;

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

    return ERROR;
}