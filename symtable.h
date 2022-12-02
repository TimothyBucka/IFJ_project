#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "semantics.h"
#include "token.h"

#define TABLESIZE 13

typedef struct
{
    data_type type;
} variable;

//make parameter struct
typedef struct
{
    char* name;
    data_type type;
} parameter;

typedef struct
{
    data_type return_type;
    parameter *parameters;
} function;

typedef union {
    variable variable;
    function function;
}function_or_variable;

typedef struct {
    char *name;
    union function_or_variable* f_or_v;

} table_item_data;
typedef struct table_item {
    table_item_data value;
    struct table_item *next_item;
} table_item_t;

//typedef a hashtable with a fixed array of pointers to table_item
typedef table_item_t** hash_table;

typedef struct{
    hash_table * global;
    hash_table * local;
}symtables;

int get_hash(char *key);

hash_table init_hash_table();

table_item_data *hash_table_lookup(hash_table table, char *key);

void hash_table_insert(hash_table table, table_item_data *item);

void hast_table_remove(hash_table table, char *key);

void debug_print_table(hash_table table);

#endif
