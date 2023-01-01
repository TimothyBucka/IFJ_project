/**
 * Project: IFJ22 Compiler
 *
 * @file analysis.c
 * @brief Implementation of syntax analysis
 *
 * @authors xmacur09, xstect00, xbucka00
 */

#include "analysis.h"
#include "expressions.h"
#include <stdlib.h>

extern error ERROR;
extern int ERRORFROMLEXER;
extern int BODYRECURSIONCOUNT;
int ARGSCOUNT = 0;
int PARAMSCOUNT = 0;
int IFRECURSIONCOUNT = 0;

bool accept(token *token_ptr, token_ID acceptedID) {
    if (acceptedID == token_ptr->ID) {
        return true;
    }
    return false;
}

bool expect(token *token_ptr, token_ID acceptedID) {
    if (acceptedID == token_ptr->ID) {
        return true;
    }

    return false;
}

bool run_analysis(lexer_T *lexer, DLL *dll) {
    start_of_generator();

    hash_table global = init_hash_table();
    hash_table local = init_hash_table();

    preload_hash_table(global);

    symtables tables = {global, local};

    bool var = parse_body(lexer, dll, tables);

    // debug_print_table(global);
    // debug_print_table(local);

    hash_table_free(tables.global);

    hash_table_free(tables.local);

    return var;
}

bool parse_arguments_prime(lexer_T *lexer, DLL *dll, symtables tables, table_item_data *data, char *function_name) {
    token *token_ptr;
    ARGSCOUNT++;

    next_tok;
    if (accept(token_ptr, TOKEN_ID_COMMA)) {
        next_tok;

        if (accept(token_ptr, TOKEN_ID_VARIABLE)) {
            table_item_data *var_data = hash_table_lookup(tables.local, token_ptr->VAL.string);
            if (var_data == NULL) {
                var_data = hash_table_lookup(tables.global, token_ptr->VAL.string);
                if (var_data == NULL) {
                    ERROR = UNDEFINED_VAR_ERR;
                    return false;
                }
            }

            compare_params(var_data->f_or_v.variable->type);
            if (!strcmp(function_name, "write")) {
                write_single_var(token_ptr);
            }
            if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
                return false;
            }
        }
        else if (accept(token_ptr, TOKEN_ID_INTEGER)) {
            compare_params(INT);
            if (!strcmp(function_name, "write")) {
                write_single_var(token_ptr);
            }
            if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
                return false;
            }
        }
        else if (accept(token_ptr, TOKEN_ID_STRING)) {
            compare_params(STRING);
            if (!strcmp(function_name, "write")) {
                write_single_var(token_ptr);
            }
            if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
                return false;
            }
        }
        else if (accept(token_ptr, TOKEN_ID_FLOAT)) {
            compare_params(FLOAT);
            if (!strcmp(function_name, "write")) {
                write_single_var(token_ptr);
            }
            if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
                return false;
            }
        }

        else {
            return_tok;
            return false;
        }
    }
    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_arguments(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;
    ARGSCOUNT = 0;
    char *function_name = dll->activeElement->previousElement->data.VAL.string; // name of function thats being called
    table_item_data *data = hash_table_lookup(tables.global, function_name);

    if (data != NULL) {
    }

    if (data == NULL) {
        ERROR = UNDEFINED_FUNCTION_ERR;
        return false;
    }
    next_tok;

    if (accept(token_ptr, TOKEN_ID_VARIABLE)) {
        table_item_data *var_data = hash_table_lookup(tables.local, token_ptr->VAL.string);
        if (var_data == NULL) {
            var_data = hash_table_lookup(tables.global, token_ptr->VAL.string);
            if (var_data == NULL) {
                ERROR = UNDEFINED_VAR_ERR;
                return false;
            }
        }
        compare_params(var_data->f_or_v.variable->type);

        if (!strcmp(function_name, "write")) {
            write_single_var(token_ptr);
        }

        if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_INTEGER)) {
        if (!strcmp(function_name, "write")) {
            write_single_var(token_ptr);
        }
        else {
            generate_function_frame();
            push_argument_from_static(token_ptr, INT, ARGSCOUNT);
        }
        compare_params(INT);

        if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_STRING)) {
        if (!strcmp(function_name, "write")) {
            write_single_var(token_ptr);
        }
        else {
            generate_function_frame();
            push_argument_from_static(token_ptr, STRING, ARGSCOUNT);
        }
        compare_params(STRING);
        if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_FLOAT)) {
        if (!strcmp(function_name, "write")) {
            write_single_var(token_ptr);
        }
        else {
            generate_function_frame();
            push_argument_from_static(token_ptr, FLOAT, ARGSCOUNT);
        }
        compare_params(FLOAT);
        if (!parse_arguments_prime(lexer, dll, tables, data, function_name)) {
            return false;
        }
    }

    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_type(lexer_T *lexer, DLL *dll) {
    token *token_ptr;

    next_tok;
    if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_VOID) {
        return true;
    }
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && ((token_ptr->VAL.keyword == KW_INT) || (token_ptr->VAL.keyword == KW_INT_NULL))) {
        return true;
    }
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && ((token_ptr->VAL.keyword == KW_FLOAT) || (token_ptr->VAL.keyword == KW_FLOAT_NULL))) {
        return true;
    }
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && ((token_ptr->VAL.keyword == KW_STRING) || (token_ptr->VAL.keyword == KW_STRING_NULL))) {
        return true;
    }
    else {
        return_tok;
        return false;
    }
}

bool parse_parameters_prime(lexer_T *lexer, DLL *dll, symtables tables, function *func) {
    token *token_ptr;

    next_tok;
    if (accept(token_ptr, TOKEN_ID_COMMA)) {
        if (!parse_type(lexer, dll)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (expect(token_ptr, TOKEN_ID_VARIABLE)) {
            create_var(&dll->activeElement->data);
            PARAMSCOUNT++;
            func->parameters = realloc(func->parameters, (func->num_of_params + 1) * sizeof(parameter));
            parameter param = {dll->activeElement->data.VAL.string, kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword)};
            func->parameters[func->num_of_params] = param;

            table_item_data *local_data;
            char *variable_name = dll->activeElement->data.VAL.string;
            if (!hash_table_has_item(tables.local, variable_name)) {
                local_data = malloc(sizeof(table_item_data));
                local_data->name = variable_name;
                variable *var = malloc(sizeof(variable));
                var->type = kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword);
                local_data->f_or_v.variable = var;
                local_data->is_var = true;
                func->num_of_params = PARAMSCOUNT;
                hash_table_insert(tables.local, local_data);
            }
            else {
                return_error(SEM_OTHER_ERR);
            }

            if (!parse_parameters_prime(lexer, dll, tables, func)) {
                return_error(SYNTAX_ERR);
            }
        }
        else {
            return_tok;
            return_error(SYNTAX_ERR);
        }
    }
    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_parameters(lexer_T *lexer, DLL *dll, symtables tables, function *func) {
    PARAMSCOUNT = 0;
    token *token_ptr;
    if (!parse_type(lexer, dll)) {
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {

            return_error(SYNTAX_ERR);
        }
        else {
            return_tok;
            return true;
        }
    }
    next_tok;

    if (accept(token_ptr, TOKEN_ID_VARIABLE)) {
        func->parameters = realloc(func->parameters, (func->num_of_params + 1) * sizeof(parameter)); // adding function info to global table
        parameter param = {dll->activeElement->data.VAL.string, kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword)};
        func->parameters[func->num_of_params] = param;
        create_var(&dll->activeElement->data);
        move_from_argument_to_var(&dll->activeElement->data, PARAMSCOUNT);
        PARAMSCOUNT++;

        table_item_data *local_data;
        char *variable_name = dll->activeElement->data.VAL.string;

        if (!hash_table_has_item(tables.local, variable_name)) {
            local_data = malloc(sizeof(table_item_data));
            local_data->name = variable_name;
            variable *var = malloc(sizeof(variable));
            var->type = kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword);
            local_data->f_or_v.variable = var;
            local_data->is_var = true;
            func->num_of_params = PARAMSCOUNT;
            hash_table_insert(tables.local, local_data);
        }

        else {
            return_error(UNDEFINED_FUNCTION_ERR);
        }

        if (!parse_parameters_prime(lexer, dll, tables, func)) {
            return_error(SYNTAX_ERR);
        }
    }

    else {
        return_tok;
        return true;
    }

    return true;
}

bool parse_assignment_prime(lexer_T *lexer, DLL *dll, symtables tables, data_type *type) {
    bool r = parse_expression(lexer, dll, tables, type, false);
    return r;
}

bool parse_assignment(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;
    if (!accept(token_ptr, TOKEN_ID_VARIABLE)) {
        return_tok;
        return false;
    }

    hash_table table_to_use = tables.global;
    if (BODYRECURSIONCOUNT != 1) {
        table_to_use = tables.local;
    }

    table_item_data *data;
    char *variable_name = token_ptr->VAL.string;
    variable *var;
    data_type assi_type;
    if (!hash_table_has_item(table_to_use, token_ptr->VAL.string)) {
        data = malloc(sizeof(table_item_data));
        data->name = token_ptr->VAL.string;
        var = malloc(sizeof(variable));
        var->type = UNDEFINED;
        data->f_or_v.variable = var;
        data->is_var = true;
    }

    next_tok;
    if (!accept(token_ptr, TOKEN_ID_EQUALS) && !accept(token_ptr, TOKEN_ID_NOT_EQUALS) && !accept(token_ptr, TOKEN_ID_TRIPLE_EQUALS)) {
        return_tok;
        return_tok;

        return false;
    }
    // generate instructions for new variable
    token *func_name_token = &dll->activeElement->previousElement->data;
    table_item_data *def_var = hash_table_lookup(table_to_use, func_name_token->VAL.string);

    if (def_var == NULL) {
        create_var(func_name_token);
    }
    else {
        // if (def_var->f_or_v.variable->type == UNDEFINED) {
        //     create_var(func_name_token);
        // }
    }

    next_tok;
    if (accept(token_ptr, TOKEN_ID_IDENTIFIER)) {

        table_item_data *fun = hash_table_lookup(tables.global, token_ptr->VAL.string);
        if (fun == NULL) {
            return_error(UNDEFINED_FUNCTION_ERR);
        }

        switch (fun->f_or_v.function->return_type) {
        case INT_NULL:
            assi_type = INT;
            break;
        case FLOAT_NULL:
            assi_type = FLOAT;
            break;
        case STRING_NULL:
            assi_type = STRING;
            break;
        default:
            assi_type = fun->f_or_v.function->return_type;
            break;
        }

        if (!hash_table_has_item(table_to_use, variable_name)) {
            data->f_or_v.variable->type = assi_type;
            hash_table_insert(table_to_use, data);
        }
        else {
            data = hash_table_lookup(table_to_use, variable_name);
            data->f_or_v.variable->type = assi_type;
        }
        if (strcmp(token_ptr->VAL.string, "readi") == 0 || strcmp(token_ptr->VAL.string, "readf") == 0 || strcmp(token_ptr->VAL.string, "reads") == 0) {
            read_input(&dll->activeElement->previousElement->previousElement->data, assi_type);
        }

        next_tok;

        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        if (!parse_arguments(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            return_error(SYNTAX_ERR);
        }
    }
    else {
        return_tok;
        if (!parse_assignment_prime(lexer, dll, tables, &assi_type)) {
            return_error(SYNTAX_ERR);
        }
        // pop the result of the expression to the variable
        pop_to_var(func_name_token);

        next_tok;
        if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            return_error(SYNTAX_ERR);
        }

        if (!hash_table_has_item(table_to_use, variable_name)) {
            data->f_or_v.variable->type = assi_type;
            hash_table_insert(table_to_use, data);
        }
        else {
            data = hash_table_lookup(table_to_use, variable_name);
            data->f_or_v.variable->type = assi_type;
        }
    }

    return true;
}

bool parse_function_call(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;
    next_tok;
    // Function call
    if (accept(token_ptr, TOKEN_ID_IDENTIFIER)) {
        if(strcmp(token_ptr->VAL.string, "write")) {
            bool generate_function_frame();
        }
        token *func_name_token = token_ptr;
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        if (!parse_arguments(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            return_error(SYNTAX_ERR);
        }
        
        if(strcmp(func_name_token->VAL.string, "write")) {
            generate_function_call(func_name_token);    
        }

        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
    }
    else {
        return_tok;
        return false;
    }
    return true;
}

bool parse_body(lexer_T *lexer, DLL *dll, symtables tables) {
    BODYRECURSIONCOUNT++;
    data_type final_type;

    token *token_ptr;

    next_tok;

    // case Function
    if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_FUNCTION && BODYRECURSIONCOUNT == 1) { //  function

        next_tok;
        function *func = malloc(sizeof(function));
        table_item_data *data = malloc(sizeof(table_item_data));

        if (!expect(token_ptr, TOKEN_ID_IDENTIFIER)) {
            return_error(SYNTAX_ERR);
        } //  func_id
        data->name = token_ptr->VAL.string;

        // TODO call label and pushframe
        generate_function_label(token_ptr);

        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error(SYNTAX_ERR);
        } //  (
        if (!parse_parameters(lexer, dll, tables, func)) {
            return_error(SYNTAX_ERR);
        } //  parameters
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error(SYNTAX_ERR);
        } //  )
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_COLON)) {
            return_error(SYNTAX_ERR);
        } //  :
        if (!parse_type(lexer, dll)) {
            return_error(SYNTAX_ERR);
        } //  type

        // TODO defretval and set to nil
        generate_function_retval();

        func->return_type = kw_to_data_type(dll->activeElement->data.VAL.keyword);
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            return_error(SYNTAX_ERR);
        } //  {

        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        } //  body
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            return_error(SYNTAX_ERR);
        } //  }

        data->is_var = false;
        data->f_or_v.function = func;
        hash_table_insert(tables.global, data);

        BODYRECURSIONCOUNT--;
        clear_hash_table_to_inicialised(tables.local);

        // TODO call popframe and return
        generate_function_exit();

        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        } //  body
    }

    // case If
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_IF) { //  if
        IFRECURSIONCOUNT++;

        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  (
        if (!parse_expression(lexer, dll, tables, &final_type, true)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  expresion
        generate_if_begin();
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  )
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  {
        BODYRECURSIONCOUNT--;
        if (!parse_body(lexer, dll, tables)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  body
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  }
        next_tok;
        if (!accept(token_ptr, TOKEN_ID_KEYWORD) || token_ptr->VAL.keyword != KW_ELSE) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  else
        generate_if_else();
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  {
        if (!parse_body(lexer, dll, tables)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  body
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            IFRECURSIONCOUNT--;
            return_error(SYNTAX_ERR);
        } //  }
        generate_if_end();
        IFRECURSIONCOUNT--;
        BODYRECURSIONCOUNT--;
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        } //  body
    }

    // case While
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_WHILE) { //  while
        generate_while_label();
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error(SYNTAX_ERR);
        } //  (

        if (!parse_expression(lexer, dll, tables, &final_type, true)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LCURLYBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        generate_while_begin();

        BODYRECURSIONCOUNT--;
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        generate_while_end();

        BODYRECURSIONCOUNT--;
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
    }

    // case Return
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_RETURN) {
        // generate_return();
        next_tok;
        if (expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            BODYRECURSIONCOUNT--;

            if (!parse_body(lexer, dll, tables)) {
                return_error(SYNTAX_ERR);
            }
        }
        else {
            return_tok;
            if (!parse_expression(lexer, dll, tables, &final_type, false)) {
                return_error(SYNTAX_ERR);
            }
            create_var_str("helper_return_value_from_expression");
            pop_to_var_str("helper_return_value_from_expression");
            generate_function_return();
            next_tok;
            if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
                return_error(SYNTAX_ERR);
            }
            else {
                BODYRECURSIONCOUNT--;
                if (!parse_body(lexer, dll, tables)) {
                    return_error(SYNTAX_ERR);
                }
            }
        }
    }

    else if (accept(token_ptr, TOKEN_ID_EOF)) {
        BODYRECURSIONCOUNT--;
        return true;
    }

    else {
        return_tok;
        if ((ERROR == SUCCESS || ERROR == SYNTAX_ERR) && parse_function_call(lexer, dll, tables)) {
            BODYRECURSIONCOUNT--;
            if (!parse_body(lexer, dll, tables)) {
                return_error(SYNTAX_ERR);
            }

            return true;
        }
        //  case assignment
        if ((ERROR == SUCCESS || ERROR == SYNTAX_ERR) && parse_assignment(lexer, dll, tables)) {
            BODYRECURSIONCOUNT--;
            if (!parse_body(lexer, dll, tables)) {
                return_error(SYNTAX_ERR);
            }
            return true;
        }
        if ((ERROR == SUCCESS || ERROR == SYNTAX_ERR) && parse_expression(lexer, dll, tables, &final_type, false)) {
            next_tok;
            if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
                return_error(SYNTAX_ERR);
            }
            BODYRECURSIONCOUNT--;
            if (!parse_body(lexer, dll, tables)) {
                return_error(SYNTAX_ERR);
            }
            return true;
        }
        else {
            if (dll->activeElement != NULL /*&& expect(&dll->activeElement->data, TOKEN_ID_SEMICOLLON)*/) {
                if (ERROR == SYNTAX_ERR && ERRORFROMLEXER == 0) {
                    ERROR = SUCCESS;
                    BODYRECURSIONCOUNT--;
                    return true;
                }
                else {
                    BODYRECURSIONCOUNT--;
                    return false;
                }
            }
            else {
                return_error(SYNTAX_ERR);
            }
        }
        BODYRECURSIONCOUNT--;
        return true;
    }

    return true;
}