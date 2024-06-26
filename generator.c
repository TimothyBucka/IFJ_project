/**
 * Project: IFJ22 Compiler
 *
 * @file generator.c
 * @brief Implementation of generator
 *
 * @authors xstect00
 */
#include "generator.h"

int IF_COUNT = 0;
int WHILE_COUNT = 0;
int FUNCTION_COUNT = 0;
void start_of_generator() {

    printf(START);

    generate_buildin_functions();

    generate_main();
}

void end_of_generator() {
    printf("LABEL $end\n");
    printf("EXIT int@0\n");
}

void generate_buildin_functions() {
    printf(FUNCTION_READS);
    printf(FUNCTION_READI);
    printf(FUNCTION_READF);
    printf(FUNCTION_WRITE);
    printf(FUNCTION_FLOAT_VALUE);
    printf(FUNCTION_INT_VALUE);
    printf(FUNCTION_STRING_VALUE);
    printf(FUNCTION_STRING_LENGTH);
    printf(FUNCTION_SUBSTRING);
    printf(FUNCTION_ORD);
    printf(FUNCTION_CHR);
}

bool generate_main() {
    printf("LABEL $main\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@tempstring\n");
    printf("DEFVAR LF@tempbool\n");

    printf("DEFVAR LF@&1\n");
    printf("DEFVAR LF@&2\n");

    return true;
}

bool generate_end_of_main() {
    printf("LABEL $END_MAIN\n");
    printf("POPFRAME\n");
    printf("CLEARS\n");

    return true;
}

bool generate_function(char *function_id) {
    printf("LABEL $");
    printf("%s", function_id); // FIXME
    printf("\n");
    printf("PUSHFRAME\n");

    return true;
}

bool generate_end_of_function() { // FIXME mby
    printf("POPFRAME\n");
    printf("CLEARS\n");

    return true;
}

bool data_type_to(data_type type) {
    if (
        type == INT) {
        printf("int@");
    }
    else if (type == FLOAT) {
        printf("float@");
    }
    else if (type == STRING) {
        printf("string@");
    }
    else if (type == NULL_TYPE) {
        printf("nil@");
    }
    else {
        return false;
    }

    return true;
}



bool implicit_conversion(data_type type, data_type converted_type, char *var1) {

    if (converted_type == FLOAT && type == INT) {
        printf("INT2FLOAT TF@%s TF@%s\n", var1, var1);
    }
    else if (converted_type == INT && type == FLOAT) {
        printf("FLOAT2INT TF@%s TF@%s\n", var1, var1);
    }
    else if (converted_type == NULL_TYPE && type == STRING) {
        printf("MOVE TF@%s string@"
               "\n",
               var1);
    }
    else if (converted_type == NULL_TYPE && type == INT) {
        printf("MOVE TF@%s int@0\n", var1);
    }
    else if (converted_type == NULL_TYPE && type == FLOAT) {
        printf("MOVE TF@%s float@0.0\n", var1);
    }
    else {
        return false;
    }

    return true;
}

bool generate_variable_value(token *token_ptr) {
    printf("MOVE LF@");
    printf("%s", token_ptr->VAL.string);
    printf(" ");
    if (generate_term(token_ptr) == 0) { // FIXME
        return false;
    }
    else {
        printf("\n");
    }

    return true;
}

bool pop_to_var(token *token_ptr) {
    printf("POPS LF@");
    printf("%s", token_ptr->VAL.string);
    printf("\n");

    return true;
}

bool pop_to_var_str(char* var_name) {
    printf("POPS LF@");
    printf("%s", var_name);
    printf("\n");

    return true;
}

bool generate_term(token *token_ptr) {

    if (token_ptr->ID == TOKEN_ID_INTEGER) {
        printf("int@%s", token_ptr->VAL.string);
    }
    else if (token_ptr->ID == TOKEN_ID_FLOAT) {
        printf("float@%a", atof(token_ptr->VAL.string));
    }
    else if (token_ptr->ID == TOKEN_ID_STRING) {
        printf("string@%s", token_ptr->VAL.string);
    }
    else if (token_ptr->ID == TOKEN_ID_VARIABLE) {
        printf("LF@%s", token_ptr->VAL.string);
    }
    else {
        return false;
    }

    return true;
}

bool generate_user_input(token *token_ptr, data_type type) {
    printf("READ LF@");
    printf("%s", token_ptr->VAL.string);
    printf(" ");
    if (data_type_to(type) == 0) { // FIXME
        return false;
    }
    else {
        printf("\n");
    }

    return true;
}

bool operation_rule(rules operation, token *token_ptr) {
    switch (operation) {
    case E_PLUS_E:
        printf("ADDS\n");
        break;
    case E_MINUS_E:
        printf("SUBS\n");
        break;
    case E_TIMES_E:
        printf("MULS\n");
        break;
    case E_DIVIDE_E:
        printf("DIVS\n");
        break;
    case E_CONCAT_E:
        printf("POPS LF@&2\n");
        printf("POPS LF@&1\n");
        printf("CONCAT LF@tempstring LF@&1 LF@&2\n");
        printf("PUSHS LF@tempstring\n");
        break;
    case E_LT_E:
        printf("LTS\n");
        break;
    case E_GT_E:
        printf("GTS\n");
        break;
    case E_EQ_E:
        printf("EQS\n");
        break;
    case E_NEQ_E:
        printf("EQS\n");
        printf("NOTS\n");
        break;
    case E_LEQ_E:
        printf("PUSHS LF@&1\n");
        printf("PUSHS LF@&2\n");
        printf("PUSHS LF@&1\n");
        printf("PUSHS LF@&2\n");
        printf("EQS\n");
        printf("POPS LF@tempbool\n");
        printf("LTS\n");
        printf("PUSHS LF@tempbool\n");
        printf("ORS\n");
        printf("PUSHS bool@true\n");
        printf("EQS\n");
        break;
    case E_GEQ_E:
        printf("PUSHS LF@&1\n");
        printf("PUSHS LF@&2\n");
        printf("PUSHS LF@&1\n");
        printf("PUSHS LF@&2\n");
        printf("EQS\n");
        printf("POPS LF@tempbool\n");
        printf("GTS\n");
        printf("PUSHS LF@tempbool\n");
        printf("ORS\n");
        printf("PUSHS bool@true\n");
        printf("EQS\n");
        break;
    case ID:

        if (token_ptr->ID == TOKEN_ID_VARIABLE) {
            printf("PUSHS LF@");
            printf("%s", token_ptr->VAL.string);
            printf("\n");
        }
        else if (token_ptr->ID == TOKEN_ID_INTEGER || token_ptr->ID == TOKEN_ID_FLOAT || token_ptr->ID == TOKEN_ID_STRING) {
            printf("PUSHS ");
            generate_term(token_ptr);
            printf("\n");
        }
        else if (token_ptr->ID == TOKEN_ID_KEYWORD && token_ptr->VAL.keyword == KW_NULL) {
            printf("PUSHS nil@nil\n");
        }
        break;
    default:
    case NONE:
        break;
    }

    return true;
}

bool generate_label(char *function_id) {
    printf("LABEL $");
    printf("%s", function_id);
    printf("\n");

    return true;
}

bool create_var(token *token_ptr) {
    printf("DEFVAR LF@");
    printf("%s", token_ptr->VAL.string);
    printf("\n");

    return true;
}

bool create_var_str(char *var_name) {
    printf("DEFVAR LF@");
    printf("%s", var_name);
    printf("\n");

    return true;
}

bool generate_return() {
    printf("JUMP $END_MAIN\n");

    return true;
}

// =============== WHILE =================
bool generate_while_label() {
    WHILE_COUNT++;
    printf("LABEL $");
    printf("WHILE&%d", WHILE_COUNT);
    printf("\n");

    return true;
}
bool generate_while_begin() {
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS $");
    printf("WHILE_END&%d", WHILE_COUNT);
    printf("\n");

    return true;
}

bool generate_while_end() {
    printf("JUMP $");
    printf("WHILE&%d", WHILE_COUNT);
    printf("\n");

    printf("LABEL $");
    printf("WHILE_END&%d", WHILE_COUNT);
    printf("\n");

    return true;
}

// =============== WHILE END =================

// =============== IF ================= //som vnoreny v IFe zacinam od 1
bool generate_if_begin() {
    IF_COUNT++;
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS $");
    printf("ELSE&%d", IF_COUNT);
    printf("\n"); // IF toto mi da true jumpuje na else ak false pokracuje v IFe

    return true;
    // nasleduje if prikazy IDK ake si chcete dat popripade skoci na else
}

bool generate_if_else() {
    printf("JUMP $");
    printf("IF&%d", IF_COUNT);
    printf("_end\n");
    printf("LABEL $");
    printf("ELSE&%d\n", IF_COUNT);
    return true;
    // nasleduju else prikazy
}

bool generate_if_end() {
    printf("LABEL $");
    printf("IF&%d", IF_COUNT);
    printf("_end\n");
    return true;
}

// =============== END OF IF =================

bool write_single_var(token *token_ptr) {
    printf("WRITE ");
    if (generate_term(token_ptr) == false) {
        return false;
    }
    printf("\n");

    return true;
}

bool read_input(token *token_ptr, data_type type) {
    printf("READ LF@");
    printf("%s ", token_ptr->VAL.string);
    if (type == FLOAT) {
        printf("float");
    }
    else if (type == INT) {
        printf("int");
    }
    else if (type == STRING) {
        printf("string");
    }

    printf("\n");

    return true;
}

bool pop_to_strname(char *name) {
    printf("POPS TF@");
    printf("%s", name);
    printf("\n");

    return true;
}

bool create_temp_frame() {
    printf("CREATEFRAME\n");

    return true;
}

bool push_from_strname(char *name) {
    printf("PUSHS TF@");
    printf("%s", name);
    printf("\n");

    return true;
}

bool push_argument_from_static(token* token_ptr, data_type type, int arg_num){
    printf("DEFVAR TF@&argument%d\n", arg_num);
    printf("MOVE  TF@&argument%d ", arg_num);

    if (type == INT) {
        printf("int@");
    }
    else if (type == FLOAT) {
        printf("float@");
    }
    else if (type == STRING) {
        printf("string@");
    }

    printf("%s\n", token_ptr->VAL.string);

    return true;
}

bool push_argument_from_var(token* token_ptr, int arg_num);

bool generate_function_label(token *token_ptr) {
    printf("JUMP $jump_around_function_number_%d\n", FUNCTION_COUNT);
    printf("LABEL $%s\n", token_ptr->VAL.string);
    printf("PUSHFRAME\n");

    return true;
}

bool generate_function_retval() {
    printf("DEFVAR LF@&retval\n");
    printf("MOVE LF@&retval nil@nil\n");

    return true;
}

bool generate_function_exit() {

    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $jump_around_function_number_%d\n", FUNCTION_COUNT);
    FUNCTION_COUNT++;
    return true;
}

bool generate_function_frame() {
    printf("CREATEFRAME\n");

    return true;
}

bool generate_function_call(token *token_ptr) {
    printf("CALL $%s\n", token_ptr->VAL.string);

    return true;
}

bool generate_function_return() {
    printf("MOVE LF@&retval LF@helper_return_value_from_expression\n");
    // if(data_type_to(type)==0){
    //     return false;
    // }
    // else {
    //     printf("\n");
    // }

    return true;
}

bool move_from_argument_to_var(token *token_ptr, int arg_num) {
    printf("MOVE LF@");
    printf("%s", token_ptr->VAL.string);
    printf(" LF@&argument%d\n", arg_num);

    return true;
}