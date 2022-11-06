#include <stdbool.h>
#include "symtable.h"
#include <stdlib.h>



int get_hash(char *key)
{
    int   h = 0, high;
    while (*key)
    {
        h = (h << 4) + *key++;
        if (high = h & 0xF0000000)
            h ^= high >> 24;
        h &= ~high;
    }
    return h;
}

hash_table* init_hash_table(){
    return malloc(sizeof(hash_table));
}

token* hash_table_lookup (hash_table* table, char* key){
    table_item* item = table[get_hash(key)];
    if ( item != NULL){
        while (item != NULL)
        {
            if (item)
            {
                /* code */
            }
            //get
        }
        
    }
    else{
        return NULL;
    }
}
