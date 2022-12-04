#include "List_fun_call.h"

// also increments the number of parameters
void add_parameter_type(data_type *parameters, int num_of_params, data_type type) {
    data_type *tmp = realloc(parameters, (num_of_params + 1) * sizeof(data_type));

    if (tmp != NULL) {
        parameters = tmp;
        parameters[num_of_params++] = type;
    }
}

void init_list_fun_call(List_fun_call *list) {
    list->first = NULL;
    list->active = NULL;
}

void add_fun_call(List_fun_call *list, fun_call *fun) {
    List_fun_call_element *new_element = malloc(sizeof(*new_element));
    new_element->data = fun;
    new_element->next = NULL;
    if (list->first == NULL) {
        list->first = new_element;
        list->active = new_element;
    }
    else {
        list->active->next = new_element;
        list->active = new_element;
    }
}

void dispose_list_fun_call(List_fun_call *list) {
    List_fun_call_element *tmp = list->first;
    while (tmp != NULL) {
        list->active = tmp->next;
        if (tmp->data) {
            if (tmp->data->parameters) {
                free(tmp->data->parameters);
            }
            if (tmp->data->name) {
                free(tmp->data->name);
            }
            free(tmp->data);
        }
        free(tmp);
        tmp = list->active;
    }
}

bool serach_fun_call(List_fun_call *list, symtables *tables) {
    List_fun_call_element *tmp = list->first;
    while (tmp != NULL) {
        table_item_data *item = hash_table_lookup(tables->global, tmp->data->name);
        if (item != NULL && !item->is_var) {
            if (item->f_or_v.function->num_of_params == tmp->data->num_of_params) {
                for (int i = 0; i < tmp->data->num_of_params; i++) {
                    if (item->f_or_v.function->parameters[i].type != tmp->data->parameters[i]) {
                        // wrong argument type
                        return false;
                    }
                }
            }
            else {
                // wrong amount of parameters
                return false;
            }
            if (item->f_or_v.function->return_type != tmp->data->supposed_return_type && tmp->data->supposed_return_type != UNDEFINED) {
                // wrong return type
                return false;
            }
        }
        else {
            // function not declared
            return false;
        }
    }
    return true;
}