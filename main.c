/**
 * Project: IFJ22 Compiler
 * 
 * @file main.c
 * @brief Main file of the compiler
 * 
 * @authors xmacur09
*/

#include "DLL.h"
#include "expressions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analysis.h"
#include "generator.h"
#include "lexer.h"
#include "semantics.h"
#include "symtable.h"
#include "token.h"
#include "chararray.h"



error ERROR = SUCCESS;
int ERRORFROMLEXER = 0;
int BODYRECURSIONCOUNT = 0;

/**
 * @brief Main function of the compiler
 * 
 * @return int 
 */
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

    generate_end_of_main();
    end_of_generator();

    printf("==== ERROR CODE ==== %d\n", ERROR);

    return ERROR;
}