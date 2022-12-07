/**
 * Project: IFJ22 Compiler
 * 
 * @file chararray.c
 * @brief Implementation of dynamic char array
 * 
 * @authors xmacur09
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *chararray_init(unsigned int size) {
    char *value = (char *)calloc(size + 1, sizeof(char));

    value[0] = '\0';

    return value;
}


void chararray_append(char **chararray, char c) {
    char *new_str;
    char* str2 = malloc(sizeof(char) * 2);
    str2[0] = c;
    str2[1] = '\0';
    if ((new_str = malloc(strlen(*chararray) + strlen(str2) + 1)) != NULL) {
        new_str[0] = '\0'; // ensures the memory is an empty string
        strcat(new_str, *chararray);
        strcat(new_str, str2);
    }

    free(*chararray);
    *chararray = new_str;
    free(str2);
}


void chararray_append_string(char **chararray, char* str) {
    char *new_str;
    if ((new_str = malloc(strlen(*chararray) + strlen(str) + 1)) != NULL) {
        new_str[0] = '\0'; // ensures the memory is an empty string
        strcat(new_str, *chararray);
        strcat(new_str, str);
    }


    free(*chararray);
    *chararray = new_str;
}


void chararray_free(char *chararray) { free(chararray); }
