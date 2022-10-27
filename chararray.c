#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *chararray_init() {
    char *value = calloc(0, sizeof(char));

    /*

    TODO add calloc check

    */

    return value;
}

void chararray_append(char *chararray, char c) {


    size_t len = strlen(chararray);
    char* temp = realloc(chararray, len + 2); // 1 for the new char, 1 for the null terminator
    if (temp != NULL){
        chararray = temp;
    }
    else{
        printf("Big memory problem.\n");
    }

    /*

    TODO add realloc check

    */
    chararray[len] = c;
    chararray[len + 1] = '\0';
}

void chararray_free(char *chararray) {
    free(chararray);
}