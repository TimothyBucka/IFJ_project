#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "generator.h"
#include "chararray.h"



    void start_of_generator(){
        printf(START);

        generate_buildin_functions();

        printf("LABEL $main\n");
    }

    void generate_buildin_functions(){
        printf(FUNCTION_READS);
        printf(FUNCTION_READI);
        printf(FUNCTION_READF);
        printf(FUNCTION_WRITE);
        printf(FUNCTION_FLOAT_VALUE);
        printf(FUNCTION_INT_VALUE);
        printf(FUNCTION_STRING_VALUE);
        printf(FUNCTION_STRING_LENGTH);
        printf(FUNCTION_SUBSTRING);
        printf(FUNCTION_ORD);
        printf(FUNCTION_CHR);
    }

// TODO zavolame write zakazdym z danym termom a mame hotovy write