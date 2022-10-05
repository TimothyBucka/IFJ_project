#include <stdlib.h>
#include <string.h>
char* chararray_init (){
    char* value = calloc(0, sizeof(char));

    /*
    
    ADD calloc CHECK 
    
    */
   
    return value;
}
char* chararray_append (char* chararray, char c){
    char* value = realloc(chararray, (strlen(chararray) + 1) * sizeof(char));

    /*
    
    ADD realloc CHECK 
    
    */

    strcat(value, (char[]){c, 0});
    return value;
}

void chararray_free (char* chararray){
    free(chararray);
}