#include "analysis.h"
#include "expressions.h"

extern error ERROR;
extern int ERRORFROMLEXER;
extern int BODYRECURSIONCOUNT;

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
    // printf("Unexpected token_ptr parsed in syntax analisys\n");
    // printf("Expected %d", acceptedID);
    // printf("got %d\n", token_ptr->ID);
    return false;
}

bool run_analysis(lexer_T *lexer, DLL *dll) {

    hash_table global = init_hash_table();
    hash_table local = init_hash_table();

    symtables tables = {global, local};

    bool var = parse_body(lexer, dll, tables);

    debug_print_table(global);
    debug_print_table(local);

    hash_table_free(tables.global);

    hash_table_free(tables.local);

    return var;
}

bool parse_arguments_prime(lexer_T *lexer, DLL *dll, symtables tables) {
    token *token_ptr;

    next_tok;
    if (accept(token_ptr, TOKEN_ID_COMMA)) {
        next_tok;
        if (expect(token_ptr, TOKEN_ID_VARIABLE)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return false;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_INTEGER)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return false;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_STRING)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
                return false;
            }
        }
        else if (expect(token_ptr, TOKEN_ID_FLOAT)) {
            if (!parse_arguments_prime(lexer, dll, tables)) {
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

    next_tok;

    // case Function
    if (accept(token_ptr, TOKEN_ID_VARIABLE)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_INTEGER)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_STRING)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }
    else if (accept(token_ptr, TOKEN_ID_FLOAT)) {
        if (!parse_arguments_prime(lexer, dll, tables)) {
            return false;
        }
    }

    else {
        return_tok;
        return true;
    }
    return true;
}

bool parse_type(lexer_T *lexer, DLL *dll, symtables tables) {
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
        if (!parse_type(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (expect(token_ptr, TOKEN_ID_VARIABLE)) {
            func->parameters = realloc(func->parameters, (func->num_of_params + 1) * sizeof(parameter));
            parameter param = {dll->activeElement->data.VAL.string, kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword)};
            func->parameters[func->num_of_params] = param;


                table_item_data *local_data;
                char* variable_name = dll->activeElement->data.VAL.string;
                data_type assi_type;
                if (!hash_table_has_item(tables.local, variable_name)) {
                    local_data = malloc(sizeof(table_item_data));
                    local_data->name = variable_name;
                    variable *var = malloc(sizeof(variable));
                    var->type = kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword);
                    local_data->f_or_v.variable = var;
                    local_data->is_var = true;
                    hash_table_insert(tables.local, local_data);
                } else {
                    //error duplicate variable???
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
    token *token_ptr;
    if (!parse_type(lexer, dll, tables)) {
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

    // case Function
    if (accept(token_ptr, TOKEN_ID_VARIABLE)) {
        func->parameters = realloc(func->parameters, (func->num_of_params + 1) * sizeof(parameter));                                        //adding function info to global table
        parameter param = {dll->activeElement->data.VAL.string, kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword)};
        func->parameters[func->num_of_params] = param;



        table_item_data *local_data;
        char* variable_name = dll->activeElement->data.VAL.string;
        data_type assi_type;
        if (!hash_table_has_item(tables.local, variable_name)) {
            local_data = malloc(sizeof(table_item_data));
            local_data->name = variable_name;
            variable *var = malloc(sizeof(variable));
            var->type = kw_to_data_type(dll->activeElement->previousElement->data.VAL.keyword);
            local_data->f_or_v.variable = var;
            local_data->is_var = true;
            hash_table_insert(tables.local, local_data);
        } else {
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
    token *token_ptr;

    // TODO add FUNCALL case
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

    printf("Recursion depth %d\n", BODYRECURSIONCOUNT);
    hash_table table_to_use = tables.global;
    if (BODYRECURSIONCOUNT != 1)
    {
        table_to_use = tables.local;
    }

    // //TODO add variable info to symtable
    table_item_data *data;
    char* variable_name = token_ptr->VAL.string;
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
    next_tok;
    if (accept(token_ptr, TOKEN_ID_IDENTIFIER)) {
        table_item_data *fun = hash_table_lookup(tables.global, token_ptr->VAL.string);
        if(fun == NULL) {
            return_error(UNDEFINED_FUNCTION_ERR);
        }
        
        var->type = fun->f_or_v.function->return_type;
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
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
    }
    else {
        return_tok;
        if (!parse_assignment_prime(lexer, dll, tables, &assi_type)) {
            return_error(SYNTAX_ERR);
        }


        

        if (!hash_table_has_item(table_to_use, variable_name)) {
            data->f_or_v.variable->type = assi_type;
            hash_table_insert(table_to_use, data);
        } else {
            data = hash_table_lookup(table_to_use, variable_name);
            data->f_or_v.variable->type = assi_type;  
        }


        
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
            return_error(SYNTAX_ERR);
        }
    }

    return true;
}

bool parse_function_call(lexer_T *lexer, DLL *dll, symtables tables){
    token *token_ptr;
    next_tok;
    if (accept(token_ptr, TOKEN_ID_IDENTIFIER)){
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

        // TODO add function info to symtable
        // table_item_data *data;
        // char* function_name = token_ptr->VAL.string;
        // data_type assi_type;
        // if (!hash_table_has_item(tables.global, token_ptr->VAL.string)) {
        //     data = malloc(sizeof(table_item_data));
        //     data->name = token_ptr->VAL.string;
        //     function *var = malloc(sizeof(function));
        //     var->return_type = UNDEFINED;
        //     data->f_or_v.function = var;
        //     data->is_var = true;
        // }





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
    BODYRECURSIONCOUNT ++;
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
        if (!parse_type(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        } //  type
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

        // TODO add function info to symtable

        data->is_var = false;
        data->f_or_v.function = func;
        hash_table_insert(tables.global, data);
        


        BODYRECURSIONCOUNT--;
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        } //  body
    }

    // case If
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_IF) { //  if
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_LBRACKET)) {
            return_error(SYNTAX_ERR);
        } //  (
        if (!parse_expression(lexer, dll, tables, &final_type, true)) {
            return_error(SYNTAX_ERR);
        } //  expresion
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RBRACKET)) {
            return_error(SYNTAX_ERR);
        } //  )
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
        next_tok;
        if (!accept(token_ptr, TOKEN_ID_KEYWORD) || token_ptr->VAL.keyword != KW_ELSE) {
            return_error(SYNTAX_ERR);
        } //  else
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
        BODYRECURSIONCOUNT--;
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        } //  body
    }

    // case While
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_WHILE) { //  while
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
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
        next_tok;
        if (!expect(token_ptr, TOKEN_ID_RCURLYBRACKET)) {
            return_error(SYNTAX_ERR);
        }
        BODYRECURSIONCOUNT--;
        if (!parse_body(lexer, dll, tables)) {
            return_error(SYNTAX_ERR);
        }
    }

    // case Return
    else if (accept(token_ptr, TOKEN_ID_KEYWORD) && token_ptr->VAL.keyword == KW_RETURN) {
        // TODO add return
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
        BODYRECURSIONCOUNT --; 
        return true;
    }

    else {
        return_tok;
        //  case assignment
        if (parse_function_call(lexer, dll, tables)){
            BODYRECURSIONCOUNT--;
            if (!parse_body(lexer, dll, tables)) {
                return_error(SYNTAX_ERR);
            }
            
            return true;
        }
        if (parse_assignment(lexer, dll, tables)) {
            BODYRECURSIONCOUNT --;
            if (!parse_body(lexer, dll, tables)) {
                return_error(SYNTAX_ERR);
            }
            return true;
        }
        if (parse_expression(lexer, dll, tables, &final_type, false)) {
            next_tok;
            if (!expect(token_ptr, TOKEN_ID_SEMICOLLON)) {
                return_error(SYNTAX_ERR);
            }
            BODYRECURSIONCOUNT --;
            if (!parse_body(lexer, dll, tables)) {
                return_error(SYNTAX_ERR);
            }
            return true;
        }
        else {
            if (dll->activeElement != NULL /*&& expect(&dll->activeElement->data, TOKEN_ID_SEMICOLLON)*/) {
                if ((ERROR == SYNTAX_ERR && ERRORFROMLEXER ==0)||(ERROR == UNDEFINED_VAR_ERR && ERRORFROMLEXER ==0)) {
                    ERROR = SUCCESS;
                    BODYRECURSIONCOUNT --;
                    return true;
                }
                else {
                    BODYRECURSIONCOUNT --;
                    return false;
                }
            }
            else {
                return_error(SYNTAX_ERR);
            }
        }
        BODYRECURSIONCOUNT --;
        return true;
    }

    return true;
}