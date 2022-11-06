#include "token.h"

#define TABLESIZE 131

typedef struct
{
    token value;
    table_item* next_item;
}table_item;

typedef table_item *hash_table[TABLESIZE];

int get_hash(char* key);

hash_table* init_hash_table();

token* hash_table_lookup (hash_table* table, char* key);