/**
 * Project: IFJ22 Compiler
 * 
 * @file generator.h
 * @brief Interface for code generator
 * 
 * @authors xstect00
*/

#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "chararray.h"
#include "expressions.h"
#include "analysis.h"
#include "DLL.h"
#include "token.h"

#define START ".IFJcode22\n\
CALL $main\n\
JUMP $end\n\
\n"

#define FUNCTION_READS "LABEL $reads\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
DEFVAR LF@param1\n\
READ LF@param1 string\n\
DEFVAR LF@errorCheck\n\
TYPE LF@errorCheck LF@param1\n\
JUMPIFNEQ $ERROR$READS string@string LF@errorCheck\n\
DEFVAR LF@strlen\n\
STRLEN LF@strlen LF@param1\n\
JUMPIFEQ $ERROR$READS LF@strlen int@0\n\
SUB LF@strlen LF@strlen int@1\n\
DEFVAR LF@getchar\n\
GETCHAR LF@getchar LF@param1 LF@strlen\n\
JUMPIFNEQ $END$OF$READS LF@getchar string@\\010\n\
SETCHAR LF@param1 LF@strlen string@\\000\n\
LABEL $ERROR$OF$READS\n\
MOVE LF@&1 LF@param1\n\
POPFRAME\n\
RETURN\n\
LABEL $ERROR$READS\n\
MOVE LF@&1 nil@nil\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_READI "LABEL $readi\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
DEFVAR LF@param1\n\
DEFVAR LF@errorCheck\n\
READ LF@param1 int\n\
TYPE LF@errorCheck LF@param1\n\
JUMPIFNEQ $ERROR$READI string@int LF@errorCheck\n\
MOVE LF@&1 LF@param1\n\
POPFRAME\n\
RETURN\n\
LABEL $ERROR$READI\n\
MOVE LF@&1 nil@nil\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_READF "LABEL $readf\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
DEFVAR LF@param1\n\
DEFVAR LF@errorCheck\n\
READ LF@param1 float\n\
TYPE LF@errorCheck LF@param1\n\
JUMPIFNEQ $ERROR$READF string@float LF@errorCheck\n\
MOVE LF@&1 LF@param1\n\
POPFRAME\n\
RETURN\n\
LABEL $ERROR$READF\n\
MOVE LF@&1 nil@nil\n\
POPFRAME\n\
RETURN\n\
\n"

//FIXME netusim ako poriesit vyber spravneho typu 2 Komentar
#define FUNCTION_WRITE "#CALL IT MULTIPLE TIMES WITH DIFFERENT PARAMETERS\n\
LABEL $write\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
POPS LF@&1\n\
MOVE LF@&1 LF@&1\n\
WRITE LF@&1\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_FLOAT_VALUE "LABEL $float_value\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
POPS LF@&1\n\
DEFVAR LF@&1type\n\
DEFVAR LF@&1val\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $float_value_null nil@nil LF@&1\n\
JUMPIFEQ $float_value_int string@int LF@&1type\n\
MOVE LF@&1val LF@&1\n\
POPFRAME\n\
RETURN\n\
LABEL $float_value_int\n\
INT2FLOAT LF@&1val LF@&1\n\
POPFRAME\n\
RETURN\n\
LABEL $float_value_null\n\
MOVE LF@&1val int@0\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_INT_VALUE "LABEL $int_value\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
POPS LF@&1\n\
DEFVAR LF@&1type\n\
DEFVAR LF@&1val\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $int_value_null nil@nil LF@&1\n\
JUMPIFEQ $int_value_float string@float LF@&1type\n\
MOVE LF@&1val LF@&1\n\
POPFRAME\n\
RETURN\n\
LABEL $int_value_float\n\
FLOAT2INT LF@&1val LF@&1\n\
POPFRAME\n\
RETURN\n\
LABEL $int_value_null\n\
MOVE LF@&1val int@0\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_STRING_VALUE "LABEL $string_value\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
POPS LF@&1\n\
DEFVAR LF@&1type\n\
DEFVAR LF@&1val\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $string_value_null nil@nil LF@&1\n\
#ROZSIRENIE NEROBIME\n\
MOVE LF@&1val LF@&1\n\
POPFRAME\n\
RETURN\n\
LABEL $string_value_null\n\
MOVE LF@&1val string@""\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_STRING_LENGTH "LABEL $length\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
POPS LF@&1\n\
STRLEN LF@&1 LF@&1\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_SUBSTRING "LABEL $substr\n\
PUSHFRAME\n\
DEFVAR LF@string\n\
DEFVAR LF@start\n\
DEFVAR LF@last\n\
DEFVAR LF@i\n\
DEFVAR LF@checking\n\
DEFVAR LF@ret\n\
DEFVAR LF@bool\n\
DEFVAR LF@len\n\
DEFVAR LF@string2\n\
POPS LF@string\n\
POPS LF@start\n\
POPS LF@last\n\
SUB LF@i LF@last LF@start\n\
ADD LF@i LF@i int@1\n\
GT LF@checking LF@start LF@last\n\
JUMPIFEQ $substr_error LF@checking bool@true\n\
STRLEN LF@len LF@string\n\
GT LF@checking LF@last LF@len\n\
JUMPIFEQ $substr_error LF@checking bool@true\n\
EQ LF@checking LF@start LF@len\n\
JUMPIFEQ $substr_error LF@checking bool@true\n\
MOVE LF@ret string@\n\
CLEARS\n\
PUSHS LF@len\n\
PUSHS int@0\n\
GTS\n\
PUSHS LF@start\n\
PUSHS int@0\n\
LTS \n\
NOTS\n\
ANDS\n\
POPS LF@bool\n\
JUMPIFEQ $substr_error LF@bool bool@false\n\
PUSHS LF@len\n\
PUSHS LF@start\n\
SUBS\n\
PUSHS LF@i\n\
LTS\n\
POPS LF@bool\n\
ADD LF@i LF@i LF@start\n\
JUMPIFEQ $substr_lts LF@bool bool@false\n\
MOVE LF@i LF@len\n\
LABEL $substr_lts\n\
LABEL $substr_loop\n\
PUSHS LF@start\n\
PUSHS LF@i\n\
LTS\n\
POPS LF@bool\n\
JUMPIFEQ $substr_end LF@bool bool@false\n\
GETCHAR LF@string2 LF@string LF@start\n\
CONCAT LF@ret LF@ret LF@string2\n\
ADD LF@start LF@start int@1\n\
JUMP $substr_loop\n\
LABEL $substr_error\n\
MOVE LF@ret nil@nil\n\
LABEL $substr_end\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_ORD "LABEL $ord\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
DEFVAR LF@string\n\
DEFVAR LF@retval\n\
POPS LF@string\n\
MOVE LF@&1 int@0\n\
JUMPIFEQ $ord_error LF@string string@""\n\
STRI2INT LF@&1 LF@string LF@&1\n\
MOVE LF@retval LF@&1\n\
LABEL $ord_error\n\
MOVE LF@retval nil@nil\n\
POPFRAME\n\
RETURN\n\
\n"

#define FUNCTION_CHR "LABEL $chr\n\
PUSHFRAME\n\
DEFVAR LF@parameter\n\
DEFVAR LF@ret\n\
POPS LF@parameter\n\
INT2CHAR LF@ret LF@parameter\n\
POPFRAME\n\
RETURN\n\
\n"

void start_of_generator();

void generate_buildin_functions();

bool generate_main();

bool generate_function_return(token *token_ptr);

bool generate_variable_value(token *token_ptr);

bool generate_term(token* token_ptr);

bool generate_user_input(token *token_ptr, data_type type);

#endif
