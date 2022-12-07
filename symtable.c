/**
 * Project: IFJ22 Compiler
 * 
 * @file symtable.c
 * @brief Implementation of symbol table
 * 
 * @authors xmacur09, xbucka00
*/

#include "symtable.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief Get the hash object
 * 
 * @param key 
 * @return int 
 */
int get_hash(char *key) {
    int length = strlen(key);

    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++)
    {
        hash_value = key[i] + P*hash_value; /* Polynomial rolling hash code */
    }

    hash_value = ((A * hash_value + B) % PRIME) % TABLESIZE; /* MAD (multiply add divide) compression method using positive int A and B <0;p-1> (also not a multiple of N) and prime larger then N*/
    return hash_value;
}
/**
 * @brief Creates a new hash table
 * 
 * @return hash_table 
 */
hash_table init_hash_table() {
    hash_table table = calloc(TABLESIZE, sizeof(table_item_t *));
    for (int i = 0; i < TABLESIZE; i++) {
        table[i] = NULL;
    }
    return table;
}
/**
 * @brief Inserts a new item into the hash table
 * 
 * @param table 
 * @param key 
 * @return table_item_data* 
 */
table_item_data *hash_table_lookup(hash_table table, char *key) {   
    table_item_t *item = table[get_hash(key)];
    if (item != NULL) {
        while (item != NULL) {
            if (!strcmp(item->value.name, key)) 
            {                                   
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
/**
 * @brief Inserts a new item into the hash table
 * 
 * @param table 
 * @param key 
 * @return true 
 * @return false 
 */
bool hash_table_has_item(hash_table table, char *key) {
    table_item_t *item = table[get_hash(key)];
    if (item != NULL) {
        while (item != NULL) {
            if (!strcmp(item->value.name, key)) 
            {                                  
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
/**
 * @brief Inserts a new item into the hash table
 * 
 * @param table 
 * @param item 
 */
void hash_table_insert(hash_table table, table_item_data *item) {
    table_item_t *new_item = calloc(1, sizeof(table_item_t));
    new_item->next_item = table[get_hash(item->name)];
    new_item->value = *item;
    table[get_hash(item->name)] = new_item;
}
/**
 * @brief Deletes an item from the hash table
 * 
 * @param table 
 * @param key 
 */
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
/**
 * @brief Deletes the hash table
 * 
 * @param table 
 */
void hash_table_free(hash_table table) {
    for (int i = 0; i < TABLESIZE; i++) {

        while (table[i] != NULL) {
            table_item_t *item = table[i];
            table[i] = (*table[i]).next_item;
            if (item->value.is_var) {
                free(item->value.f_or_v.variable);
            }
            else {
                // free(item->value.f_or_v.function->parameters);
                free(item->value.f_or_v.function);
            }
            free(item);
            item = &(*table[i]);
        }
    }
    free(table);
}

/**
 * @brief Resets the hash table
 * 
 * @param table 
 */
void clear_hash_table_to_inicialised(hash_table table) {
    for (int i = 0; i < TABLESIZE; i++) {

        while (table[i] != NULL) {
            table_item_t *item = table[i];
            table[i] = (*table[i]).next_item;
            if (item->value.is_var) {
                free(item->value.f_or_v.variable);
            }
            else {
                free(item->value.f_or_v.function);
            }
            item = &(*table[i]);
        }
    }
}


/**
 * @brief Prints the hash table
 * 
 * @param table 
 */
void debug_print_table(hash_table table) {
    printf("\n\n--------------------------------------\n");
    for (int i = 0; i < TABLESIZE; i++) {
        if (table[i] == NULL) {
            continue;
        }
        printf("%d: ", i);
        table_item_t *item = table[i];
        while (item != NULL && item->value.name != NULL) {
            if (item->value.is_var) {
                printf("[Varialbe %s, type: %d]", item->value.name, item->value.f_or_v.variable->type);
            }
            else {
                printf("[Function %s, type: %d, param_c %d]", item->value.name, item->value.f_or_v.function->return_type, item->value.f_or_v.function->num_of_params);
            }
            item = item->next_item;
        }
        printf("\n");
    }
    printf("--------------------------------------\n");
}

/**
 * @brief Preloads the hash table with the built in functions
 * 
 * @param table 
 */
void preload_hash_table(hash_table table) {

    // readInt
    function *func_readi = malloc(sizeof(function));
    table_item_data *data_readi = malloc(sizeof(table_item_data));
    func_readi->return_type = INT;
    data_readi->is_var = false;
    data_readi->name = "readi";
    data_readi->f_or_v.function = func_readi;
    hash_table_insert(table, &(*data_readi));

    // readFloat
    function *func_readf = malloc(sizeof(function));
    table_item_data *data_readf = malloc(sizeof(table_item_data));
    func_readf->return_type = FLOAT;
    data_readf->is_var = false;
    data_readf->name = "readf";
    data_readf->f_or_v.function = func_readf;
    hash_table_insert(table, &(*data_readf));

    // readString
    function *func_reads = malloc(sizeof(function));
    table_item_data *data_reads = malloc(sizeof(table_item_data));
    func_reads->return_type = STRING;
    data_reads->is_var = false;
    data_reads->name = "reads";
    data_reads->f_or_v.function = func_reads;
    hash_table_insert(table, &(*data_reads));

    // ord
    function *func_ord = malloc(sizeof(function));
    table_item_data *data_ord = malloc(sizeof(table_item_data));
    parameter *param_ord = malloc(sizeof(parameter));
    func_ord->return_type = INT;
    param_ord->type = STRING;
    func_ord->num_of_params = 1;
    func_ord->parameters = param_ord;
    data_ord->is_var = false;
    data_ord->name = "ord";
    data_ord->f_or_v.function = func_ord;
    hash_table_insert(table, &(*data_ord));

    // chr
    function *func_chr = malloc(sizeof(function));
    table_item_data *data_chr = malloc(sizeof(table_item_data));
    parameter *param_chr = malloc(sizeof(parameter));
    func_chr->return_type = STRING;
    param_chr->type = INT;
    func_chr->num_of_params = 1;
    func_chr->parameters = param_chr;
    data_chr->is_var = false;
    data_chr->name = "chr";
    data_chr->f_or_v.function = func_chr;
    hash_table_insert(table, &(*data_chr));

    // substring
    function *func_substring = malloc(sizeof(function));
    table_item_data *data_substring = malloc(sizeof(table_item_data));
    parameter *param_substring = malloc(sizeof(parameter) * 3);
    func_substring->return_type = STRING;
    param_substring[0].type = STRING;
    param_substring[1].type = INT;
    param_substring[2].type = INT;
    func_substring->num_of_params = 3;
    func_substring->parameters = param_substring;
    data_substring->is_var = false;
    data_substring->name = "substring";
    data_substring->f_or_v.function = func_substring;
    hash_table_insert(table, &(*data_substring));

    // floatval
    function *func_floatval = malloc(sizeof(function));
    table_item_data *data_floatval = malloc(sizeof(table_item_data));
    parameter *param_floatval = malloc(sizeof(parameter));
    func_floatval->return_type = FLOAT;
    param_floatval->type = TERM_TYPE;
    func_floatval->num_of_params = 1;
    func_floatval->parameters = param_floatval;
    data_floatval->is_var = false;
    data_floatval->name = "floatval";
    data_floatval->f_or_v.function = func_floatval;
    hash_table_insert(table, &(*data_floatval));

    // intval
    function *func_intval = malloc(sizeof(function));
    table_item_data *data_intval = malloc(sizeof(table_item_data));
    parameter *param_intval = malloc(sizeof(parameter));
    func_intval->return_type = INT;
    param_intval->type = TERM_TYPE;
    func_intval->num_of_params = 1;
    func_intval->parameters = param_intval;
    data_intval->is_var = false;
    data_intval->name = "intval";
    data_intval->f_or_v.function = func_intval;
    hash_table_insert(table, &(*data_intval));

    // strval
    function *func_strval = malloc(sizeof(function));
    table_item_data *data_strval = malloc(sizeof(table_item_data));
    parameter *param_strval = malloc(sizeof(parameter));
    func_strval->return_type = STRING;
    param_strval->type = TERM_TYPE;
    func_strval->num_of_params = 1;
    func_strval->parameters = param_strval;
    data_strval->is_var = false;
    data_strval->name = "strval";
    data_strval->f_or_v.function = func_strval;
    hash_table_insert(table, &(*data_strval));

    // write
    function *func_write = malloc(sizeof(function));
    table_item_data *data_write = malloc(sizeof(table_item_data));
    parameter *param_write = malloc(sizeof(parameter));
    func_write->return_type = NULL_TYPE;
    param_write->type = TERM_TYPE;
    func_write->num_of_params = 1;
    func_write->parameters = param_write;
    data_write->is_var = false;
    data_write->name = "write";
    data_write->f_or_v.function = func_write;
    hash_table_insert(table, &(*data_write));
}