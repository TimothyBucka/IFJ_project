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
    void start_of_generator(){

        printf(START);

        //generate_buildin_functions();

        generate_main();
    }

    void end_of_generator(){

        printf("LABEL $end\n");
        printf("EXIT int@0\n");
    }

    void generate_buildin_functions(){
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
// TODO zavolame write zakazdym z danym termom a mame hotovy write
//MBY can be fixed with danny a urobit cez while loop podla poctu termov
    }

bool generate_main () {
    printf("LABEL $main\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    return true;
}

bool generate_end_of_main () {
    printf("POPFRAME\n");
    printf("CLEARS\n");

    return true;
}

bool generate_function (char *function_id) {
    printf("LABEL $");
    printf("%s", function_id); //FIXME
    printf("\n");
    printf("PUSHFRAME\n");

    return true;
}

bool generate_end_of_function () { //FIXME mby
    printf("POPFRAME\n");
    printf("CLEARS\n");

    return true;
} 

bool data_type_to (data_type type) {
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

bool generate_function_return (data_type type) {
    printf("MOVE LF@&retval ");
    if(data_type_to(type)==0){
        return false;
    }
    else {
        printf("\n");
    }
    return true;
}

bool generate_function_call (char *function_id) {
    printf("CALL $");
    printf("%s", function_id);
    printf("\n");

    return true;
}

bool implicit_conversion (data_type type, data_type converted_type, char *var1) {
    //printf("DEFVAR TF@%s\n", var1);

    if (converted_type == FLOAT && type == INT) {
        printf("INT2FLOAT TF@%s TF@%s\n", var1, var1);
    }
    else if (converted_type == INT && type == FLOAT) {
        printf("FLOAT2INT TF@%s TF@%s\n", var1, var1);
    }
    else if (converted_type == NULL_TYPE && type == STRING) {
        printf("MOVE TF@%s string@""\n", var1);
    }
    else if (converted_type == NULL_TYPE && type == INT) {
        printf("MOVE TF@%s int@0\n",   var1);
    }
    else if (converted_type == NULL_TYPE && type == FLOAT) {
        printf("MOVE TF@%s float@0.0\n", var1);
    }
    else {
        return false;    
    }
    // printf("MOVE LF@");
    // printf("%s", var1);
    // printf(" TF@&%s\n");

    return true;
}


bool generate_variable_value (token* token_ptr) {
    printf("MOVE LF@");
    printf("%s", token_ptr->VAL.string);
    printf(" ");
    if(generate_term(token_ptr)==0){ //FIXME
        return false;
    }
    else {
        printf("\n");
    }
    return true;
}

bool pop_to_var(token* token_ptr){
    printf("POPS LF@");
    printf("%s", token_ptr->VAL.string);
    printf("\n");
    return true;
}

bool generate_term(token* token_ptr){
    char buffer[100];

    if (token_ptr->ID == TOKEN_ID_INTEGER){
        printf("int@%s", token_ptr->VAL.string);
    }
    else if (token_ptr->ID == TOKEN_ID_FLOAT){
        printf("float@%a", atof(token_ptr->VAL.string));
    }
    else if (token_ptr->ID == TOKEN_ID_STRING){
        printf("string@%s", token_ptr->VAL.string);
    }
    else if (token_ptr->ID == TOKEN_ID_VARIABLE){
        printf("LF@%s", token_ptr->VAL.string);
    }
    else{
        return false;
    }
    return true;
}

bool generate_user_input (token* token_ptr, data_type type) {
    printf("READ LF@");
    printf("%s", token_ptr->VAL.string);
    printf(" ");
    if(data_type_to(type)==0){ //FIXME
        return false;
    }
    else {
        printf("\n");
    }
    return true;
}

//TODO WRITE 

bool operation_rule (rules operation, token* token_ptr) {
    switch (operation)
    {
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
        printf("DEFVAR LF@tempstring\n");
        printf("CONCAT LF@tempstring LF@&1 LF@&2\n");
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
        printf("DEFVAR LF@tempbool\n");
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
        //TODO EQS hodi true/false na vrchol zasobnika vymysliet co dalej
        break;
    case E_GEQ_E:
        printf("DEFVAR LF@tempbool\n");
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
        //TODO EQS hodi true/false na vrchol zasobnika vymysliet co dalej
        break;
    case ID:

        if(token_ptr->ID == TOKEN_ID_VARIABLE){
            printf("PUSHS LF@");
            printf("%s", token_ptr->VAL.string);
            printf("\n");
        }
        else if (token_ptr->ID == TOKEN_ID_INTEGER || token_ptr->ID == TOKEN_ID_FLOAT || token_ptr->ID == TOKEN_ID_STRING){
            printf("PUSHS ");    
            generate_term(token_ptr);
            printf("\n");
        }
        
        break;
    default:
    case NONE:
        break;
    }
    return true;
}

bool generate_label (char *function_id) {
    printf("LABEL $");
    printf("%s", function_id);
    printf("\n");
    return true;
}


bool create_var (token* token_ptr) {
    printf("DEFVAR LF@");
    printf("%s", token_ptr->VAL.string);
    printf("\n");
    return true;
}

//TODO bool generate_while_begin (char *function_id) {
//     printf("JUMPIFEQ $");
//     printf("%s", function_id);
//     return true;
// }

// bool generate_while_end (char *function_id) {
//     printf("JUMP $");
    
//     return true;
// }


// =============== IF ================= //som vnoreny v IFe zacinam od 1
bool generate_if_begin (int depth) {
    IF_COUNT++;
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS $");
    printf("ELSE&%d",IF_COUNT);
    printf("\n"); //IF toto mi da true jumpuje na else ak false pokracuje v IFe
    return true;
    //nasleduje if prikazy IDK ake si chcete dat popripade skoci na else
}

bool generate_if_else (int depth) {
    printf("JUMP $");
    printf("IF&%d",IF_COUNT);
    printf("_end\n");
    printf("LABEL $");
    printf("ELSE&%d\n",IF_COUNT); 
    return true;
    //nasleduju else prikazy
}

bool generate_if_end (int depth) {
    printf("LABEL $");
    printf("IF&%d",IF_COUNT);
    printf("_end\n");
    return true;
}

// =============== END OF IF =================

bool write_single_var(token* token_ptr){
    printf("WRITE ");
    if (generate_term(token_ptr)==false){
        return false;
    }
    printf("\n");
    return true;
}

bool pop_to_strname(char* name){
    printf("POPS TF@");
    printf("%s", name);
    printf("\n");
    return true;
}

bool create_temp_frame(){
    printf("CREATEFRAME\n");
    return true;
}

bool push_from_strname(char* name){
    printf("PUSHS TF@");
    printf("%s", name);
    printf("\n");
    return true;
}

