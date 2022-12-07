/**
 * Project: IFJ22 Compiler
 * 
 * @file generator.c
 * @brief Implementation of generator
 * 
 * @authors xstect00
*/
#include "generator.h"



    void start_of_generator(){

        printf(START);

        generate_buildin_functions();

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
    if (type == FLOAT && converted_type == INT) {
        printf("INT2FLOAT TF@&conval TF@&conval\n");
    }
    else if (type == INT && converted_type == FLOAT) {
        printf("FLOAT2INT TF@&conval TF@&conval\n");
    }
    else if (type == NULL_TYPE && converted_type == STRING) {
        printf("MOVE TF@&conval string@""\n");
    }
    else if (type == NULL_TYPE && converted_type == INT) {
        printf("MOVE TF@&conval int@0\n");
    }
    else if (type == NULL_TYPE && converted_type == FLOAT) {
        printf("MOVE TF@&conval float@0.0\n");
    }
    else {
        return false;    
    }
    printf("MOVE LF@");
    printf("%s", var1);
    printf(" TF@&conval\n");

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



bool generate_term(token* token_ptr){
    char buffer[100];

    if (token_ptr->ID == TOKEN_ID_INTEGER){
        printf("float@%s", token_ptr->VAL.string);
    }
    else if (token_ptr->ID == TOKEN_ID_FLOAT){
        printf("float@%s", token_ptr->VAL.string);
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
        printf("PUSHS LF@");
        printf("%s", token_ptr->VAL.string);
        printf("\n");
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
    printf("\n"); //TODO LABEL aby bol unique takze doplnit nejake cislo popripade globalne pocitadlo a inicializovat ho pri kazdom volani
    return true;
}


bool create_var (token* token_ptr) {
    printf("DEFVAR LF@");
    printf("%s", token_ptr->VAL.string);
    printf("\n");
    return true;
}