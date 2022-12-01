#include "symtable.h"
#include <stdbool.h>
#include <stdlib.h>

int get_hash(char *key) {
    int h = 0, high;
    while (*key) {
        h = (h << 4) + *key++;
        if ((high = h & 0xF0000000))
            h ^= high >> 24;
        h &= ~high;
    }
    return h;
}

hash_table *init_hash_table() {
    return malloc(sizeof(hash_table));
}

token *hash_table_lookup(hash_table *table, char *key) {
    table_item *item = *table[get_hash(key)];
    if (item != NULL) {
        while (item != NULL) {
            if (item->value.VAL.string == key) // TODO neviem s cim mam porovnavat, co bude kluc v tokene?
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

void hash_table_insert(hash_table *table, token *item) {
    table_item *new_item = malloc(sizeof(table_item));
    new_item->next_item = *table[get_hash(item->VAL.string)];
    new_item->value = *item;
    *table[get_hash(item->VAL.string)] = new_item;
}

void hash_table_remove(hash_table *table, char *key) {
    table_item *item = *table[get_hash(key)];
    while (item != NULL) {

        if (item->value.VAL.string == key) {
            *table[get_hash(key)] = item->next_item;
            free(item);
            return;
        }
        else if (item->next_item->value.VAL.string == key) {
            table_item *tmp = item->next_item;
            item->next_item = item->next_item->next_item;
            free(tmp);
            return;
        }
        else {
            item = item->next_item;
        }
    }
}