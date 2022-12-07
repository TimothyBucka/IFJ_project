/**
 * Project: IFJ22 Compiler
 * 
 * @file symtable.h
 * @brief Interface for symbol table
 * 
 * @authors xmacur09, xbucka00
*/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "semantics.h"
#include "token.h"
#include <string.h>
#include <stdbool.h>

#define TABLESIZE 49157
#define PRIME 196613            //prime number larger than TABLESIZE   
#define P 37                    //empirical constant    http://www.cs.emory.edu/~cheung/Courses/253/Syllabus/Map/hash-func.html
#define A 83103                 
#define B 930721

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
    int num_of_params;
} function;

typedef union {
    variable *variable;
    function *function;
}function_or_variable;

typedef struct {
    char *name;
    function_or_variable f_or_v;
    bool is_var;
} table_item_data;

typedef struct table_item {
    table_item_data value;
    struct table_item *next_item;
} table_item_t;

//typedef a hashtable with a fixed array of pointers to table_item
typedef table_item_t** hash_table;

typedef struct{
    hash_table global;
    hash_table local;
}symtables;

/**
 * @brief Get the hash object
 * 
 * @param key 
 * @return int 
 */
int get_hash(char *key);

/**
 * @brief Creates a new hash table
 * 
 * @return hash_table 
 */
hash_table init_hash_table();

/**
 * @brief Inserts a new item into the hash table
 * 
 * @param table 
 * @param key 
 * @return table_item_data* 
 */
table_item_data *hash_table_lookup(hash_table table, char *key);

/**
 * @brief Inserts a new item into the hash table
 * 
 * @param table 
 * @param key 
 * @return true 
 * @return false 
 */
bool hash_table_has_item(hash_table table, char *key);

/**
 * @brief Inserts a new item into the hash table
 * 
 * @param table 
 * @param item 
 */
void hash_table_insert(hash_table table, table_item_data *item);

/**
 * @brief Deletes an item from the hash table
 * 
 * @param table 
 * @param key 
 */
void hash_table_remove(hash_table table, char *key);

/**
 * @brief Prints the hash table
 * 
 * @param table 
 */
void debug_print_table(hash_table table);

/**
 * @brief Deletes the hash table
 * 
 * @param table 
 */
void hash_table_free(hash_table table);

/**
 * @brief Preloads the hash table with the built in functions
 * 
 * @param table 
 */
void preload_hash_table(hash_table table);

/**
 * @brief Resets the hash table
 * 
 * @param table 
 */
void clear_hash_table_to_inicialised(hash_table table);
#endif
