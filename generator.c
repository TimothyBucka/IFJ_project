/**
 * Project: IFJ22 Compiler
 * 
 * @file generator.c
 * @brief Implementation of generator
 * 
 * @authors xstect00
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "generator.h"
#include "chararray.h"
#include "expressions.h"
#include "analysis.h"
#include "DLL.h"




    void start_of_generator(){

        printf(START);

        generate_buildin_functions();

        generate_main();
    }

    void end_of_generator(){

        printf("LABEL $end\n");
        printf("EXIT int@0\n");
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
// TODO zavolame write zakazdym z danym termom a mame hotovy write
//MBY can be fixed with danny a urobit cez while loop podla poctu termov
    }

bool generate_main () {
    printf("LABEL $main\n");
    printf("PUSHFRAME\n");
    return true;
}

bool generate_end_of_main () {
    printf("POPFRAME\n");
    printf("CLEARS\n");

    return true;
}

bool generate_function (char *function_id) {
    printf("LABEL $");
    printf("%s", function_id); //FIXME
    printf("\n");
    printf("PUSHFRAME\n");

    return true;
}

bool generate_end_of_function () { //FIXME mby
    printf("POPFRAME\n");
    printf("CLEARS\n");

    return true;
} 

bool data_type_to (data_type type) {
    if (type == INT) {
        printf("int@");
    }
    else if (type == FLOAT) {
        printf("float@");
    }
    else if (type == STRING) {
        printf("string@");
    }
    else if (type == NULL_TYPE) {
        printf("nil@");
    }
    else {
        return false;
    }

    return true;
}

bool generate_function_return (data_type type) {
    printf("MOVE LF@&retval ");
    if(data_type_to(type)==0){
        return false;
    }
    else {
        printf("\n");
    }
    return true;
}

bool generate_function_call (char *function_id) {
    printf("CALL $");
    printf("%s", function_id);
    printf("\n");

    return true;
}

bool implicit_conversion (data_type type, data_type converted_type, char *var1) {
    if (type == FLOAT && converted_type == INT) {
        printf("INT2FLOAT TF@&conval TF@&conval\n");
    }
    else if (type == INT && converted_type == FLOAT) {
        printf("FLOAT2INT TF@&conval TF@&conval\n");
    }
    else if (type == NULL_TYPE && converted_type == STRING) {
        printf("MOVE TF@&conval string@""\n");
    }
    else if (type == NULL_TYPE && converted_type == INT) {
        printf("MOVE TF@&conval int@0\n");
    }
    else if (type == NULL_TYPE && converted_type == FLOAT) {
        printf("MOVE TF@&conval float@0.0\n");
    }
    else {
        return false;    
    }
    printf("MOVE LF@");
    printf("%s", var1);
    printf(" TF@&conval\n");

    return true;
}


