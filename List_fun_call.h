#ifndef LIST_FUN_CALL_H
#define LIST_FUN_CALL_H

#include "symtable.h"
#include "semantics.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    char *name;
    data_type *parameters; // dont care about the names of the parameters
    data_type supposed_return_type;
    int num_of_params;
} fun_call;

typedef struct List_element {
    fun_call *data;
    struct List_element *next;
} List_fun_call_element;

/// @brief List of function calls
typedef struct {
    List_fun_call_element *first;
    List_fun_call_element *active;
} List_fun_call;

void add_parameter_type(data_type*, int, data_type);

void init_list_fun_call(List_fun_call *list);

void add_fun_call(List_fun_call *list, fun_call *fun);

void dispose_list_fun_call(List_fun_call *list);

bool search_fun_call(List_fun_call *list, symtables *tables);

#endif // LIST_FUN_CALL_H