#include "symtable.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int get_hash(char *key) {
    int h = 0, high;
    while (*key) {
        h = (h << 4) + *key++;
        if ((high = h & 0xF0000000))
            h ^= high >> 24;
        h &= ~high;
    }
    return h%TABLESIZE;
}


hash_table init_hash_table() {
    hash_table table = calloc(TABLESIZE,sizeof(table_item_t*));
    for (int i = 0; i < TABLESIZE; i++) {
        table[i] = NULL;
    }
    return table;
}

table_item_data *hash_table_lookup(hash_table table, char *key) {
    table_item_t *item = table[get_hash(key)];
    if (item != NULL) {
        while (item != NULL) {
            if (!strcmp(item->value.name,key)) // TODO neviem s cim mam porovnavat, co bude kluc v tokene?
            {                                  // mozno bude treba pridat nieco do token struktury, ak ideme teda vkladat tokeny
                return &item->value;
            }
            else {
                item = item->next_item;
            }
        }
        return NULL;
    }
    else {
        return NULL;
    }
}

bool hash_table_has_item(hash_table table, char *key){
        table_item_t *item = table[get_hash(key)];
    if (item != NULL) {
        while (item != NULL) {
            if (!strcmp(item->value.name, key)) // TODO neviem s cim mam porovnavat, co bude kluc v tokene?
            {                                  // mozno bude treba pridat nieco do token struktury, ak ideme teda vkladat tokeny
                return true;
            }
            else {
                item = item->next_item;
            }
        }
        return false;
    }
    else {
        return false;
    }
}


void hash_table_insert(hash_table table, table_item_data *item) {
    table_item_t *new_item = calloc(1,sizeof(table_item_t));
    new_item->next_item = table[get_hash(item->name)];
    new_item->value = *item;
    table[get_hash(item->name)] = new_item;
}

void hash_table_remove(hash_table table, char *key) {
    table_item_t *item = table[get_hash(key)];
    while (item != NULL) {

        if (!strcmp(item->value.name, key)) {
            table[get_hash(key)] = item->next_item;
            free(item);
            return;
        }
        else if (item->next_item->value.name == key) {
            table_item_t *tmp = item->next_item;
            item->next_item = item->next_item->next_item;
            free(tmp);
            return;
        }
        else {
            item = item->next_item;
        }
    }
}

void hash_table_free(hash_table table){
    for (int i = 0; i < TABLESIZE; i++) {
        
        while (table[i] != NULL ) {
            table_item_t *item = table[i];
            table[i] = (*table[i]).next_item;
            if (item->value.is_var) 
            {
                free(item->value.f_or_v.variable);
            } else {
                //free(item->value.f_or_v.function->parameters);
                free(item->value.f_or_v.function);
            }
            free(item);
            item = &(*table[i]);
        }
    }
    free(table);
}

void debug_print_table(hash_table table){
    printf("\n\n--------------------------------------\n");
    for (int i = 0; i < TABLESIZE; i++) {
        printf("%d: ", i);
        table_item_t *item = table[i];
        while (item != NULL && item->value.name != NULL) {
            printf("[name:%s, type:%s] ", item->value.name, item->value.is_var==true ? "var" : "func" );
            item = item->next_item;
        }
        printf("\n");
    }
    printf("--------------------------------------\n");
}