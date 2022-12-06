
#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define START ".IFJcode22\n\
CREATEFRAME\n\
CALL $$main\n\
JUMP $$end\n\
\n"

//TODO zmenit premenne
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

//TODO vypis pre int pomocou %d a pre float pomocou %a
#define FUNCTION_WRITE "LABEL $write\n\
PUSHFRAME\n\
DEFVAR LF@&1\n\
DEFVAR LF@&1type\n\
LABEL $write_loop\n\
JUMPIFEQ loop$end LF@&0 int@0\n\
POPS LF@&1\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $write_int string@int LF@&1type\n\
JUMPIFEQ $write_float string@float LF@&1type\n\
JUMPIFEQ $write_null string@nil LF@&1type\n\
LABEL $write_string \n\
WRITE LF@&1\n\
SUB LF@&0 LF@&0 int@1\n\
JUMP $write_loop\n\
LABEL $write$int\n\
MOVE LF@&1 LF@&1\n\
JUMP $write_string\n\
LABEL $write$float\n\
MOVE LF@&1 LF@&1\n\
JUMP $write_string\n\
LABEL $write$null\n\
MOVE LF@&1 string@nil\n\
JUMP $write_string\n\
LABEL loop$end\n\
CLEARS\n\
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


void start_of_generator();
void gen_fun_reads();
void gen_fun_readi();
void gen_fun_readf();
void gen_fun_write();
void gen_fun_floatval();
void gen_fun_intval();
void gen_fun_strval();
void gen_fun_strlen();
void gen_fun_substr();
void gen_fun_ord();
void gen_fun_chr();

#endif


/* This must vypisat 
.IFJcode22
CALL NULLMAIN42069
JUMP ENDENDENDEND

LABEL shiftRight
PUSHFRAME
DEFVAR LF@$optype
DEFVAR LF@i
PUSHS int@0
POPS LF@i
CLEARS
DEFVAR LF@ret
TYPE LF@$optype LF@n
PUSHS LF@$optype
PUSHS string@
JUMPIFNEQS checkinitializedgood69
EXIT int@5
LABEL checkinitializedgood69
PUSHS LF@n
POPS LF@ret
CLEARS
LABEL $WhileStart70
TYPE LF@$optype LF@i
PUSHS LF@$optype
PUSHS string@
JUMPIFNEQS checkinitializedgood71
EXIT int@5
LABEL checkinitializedgood71
PUSHS LF@i
TYPE LF@$optype LF@bits
PUSHS LF@$optype
PUSHS string@
JUMPIFNEQS checkinitializedgood72
EXIT int@5
LABEL checkinitializedgood72
PUSHS LF@bits
CREATEFRAME
DEFVAR TF@a
DEFVAR TF@b
DEFVAR TF@res
POPS TF@b
POPS TF@a
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@nil
EQS
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@nil
EQS
ORS
PUSHS bool@true
JUMPIFEQS $LESSNOTCOMPARE73
TYPE TF@res TF@a
PUSHS TF@res
TYPE TF@res TF@b
PUSHS TF@res
JUMPIFEQS $LESSCOMPARE73
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@string
EQS
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@bool
EQS
ORS
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@string
EQS
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@bool
EQS
ORS
ORS
PUSHS bool@true
JUMPIFNEQS $LESSVALIDTYPE73
EXIT int@7
LABEL $LESSVALIDTYPE73
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@int
JUMPIFNEQS $LESKIP173
INT2FLOAT TF@a TF@a
LABEL $LESKIP173
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@int
JUMPIFNEQS $LESKIP273
INT2FLOAT TF@b TF@b
LABEL $LESKIP273
LABEL $LESSCOMPARE73
PUSHS TF@a
PUSHS TF@b
LTS
JUMP $LESSEND73
LABEL $LESSNOTCOMPARE73
PUSHS bool@false
LABEL $LESSEND73
CREATEFRAME
DEFVAR TF@a
POPS TF@a
DEFVAR TF@type
TYPE TF@type TF@a
PUSHS TF@type
PUSHS string@bool
JUMPIFNEQS $IsSomethingElse70
PUSHS TF@a
JUMP $ENDOFCONDITION70
LABEL $IsSomethingElse70
TYPE TF@type TF@a
PUSHS TF@type
PUSHS string@int
JUMPIFNEQS $Next170
PUSHS TF@a
PUSHS int@0
JUMPIFNEQS $PUSHTRUUU70
PUSHS bool@false
JUMP $ENDOFCONDITION70
LABEL $Next170
TYPE TF@type TF@a
PUSHS TF@type
PUSHS string@float
JUMPIFNEQS $Next270
PUSHS TF@a
PUSHS float@0x0p+0
JUMPIFNEQS $PUSHTRUUU70
PUSHS bool@false
JUMP $ENDOFCONDITION70
LABEL $Next270
TYPE TF@type TF@a
PUSHS TF@type
PUSHS string@string
JUMPIFNEQS $Next370
PUSHS TF@a
PUSHS string@
EQS
PUSHS TF@a
PUSHS string@0
EQS
PUSHS TF@a
PUSHS string@0.0
EQS
ORS
JUMPIFEQS $PUSHTRUUU70
PUSHS bool@false
JUMP $ENDOFCONDITION70
LABEL $Next370
PUSHS bool@false
JUMP $ENDOFCONDITION70
LABEL $PUSHTRUUU70
PUSHS bool@true
LABEL $ENDOFCONDITION70
PUSHS bool@true
JUMPIFNEQS $WhileEnd70
TYPE LF@$optype LF@ret
PUSHS LF@$optype
PUSHS string@
JUMPIFNEQS checkinitializedgood74
EXIT int@5
LABEL checkinitializedgood74
PUSHS LF@ret
PUSHS int@2
CREATEFRAME
DEFVAR TF@a
DEFVAR TF@b
DEFVAR TF@res
POPS TF@b
POPS TF@a
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@nil
EQS
PUSHS bool@true
JUMPIFNEQS $NOTNUL76
MOVE TF@a int@0
JUMP $tudytamskip76
LABEL $NOTNUL76
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@string
EQS
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@bool
EQS
ORS
PUSHS bool@true
JUMPIFEQS $FAIL75
LABEL $tudytamskip76
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@nil
EQS
PUSHS bool@true
JUMPIFNEQS $NOTNUL77
MOVE TF@b int@0
JUMP $tudytamskip77
LABEL $NOTNUL77
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@string
EQS
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@bool
EQS
ORS
PUSHS bool@true
JUMPIFEQS $FAIL75
LABEL $tudytamskip77
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@int
JUMPIFNEQS $DIVLABEL177
INT2FLOAT TF@a TF@a
LABEL $DIVLABEL177
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@int
JUMPIFNEQS $DIVLABEL277
INT2FLOAT TF@b TF@b
LABEL $DIVLABEL277
DIV TF@res TF@a TF@b
PUSHS TF@res
JUMP $EXIT77
LABEL $FAIL77
EXIT int@57
LABEL $EXIT77
POPS LF@ret
CLEARS
TYPE LF@$optype LF@i
PUSHS LF@$optype
PUSHS string@
JUMPIFNEQS checkinitializedgood78
EXIT int@5
LABEL checkinitializedgood78
PUSHS LF@i
PUSHS int@1
CREATEFRAME
DEFVAR TF@a
DEFVAR TF@b
DEFVAR TF@res
POPS TF@b
POPS TF@a
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@nil
EQS
PUSHS bool@true
JUMPIFNEQS $NOTNUL80
MOVE TF@a int@0
JUMP $tudytamskip80
LABEL $NOTNUL80
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@string
EQS
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@bool
EQS
ORS
PUSHS bool@true
JUMPIFEQS $FAIL79
LABEL $tudytamskip80
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@nil
EQS
PUSHS bool@true
JUMPIFNEQS $NOTNUL81
MOVE TF@b int@0
JUMP $tudytamskip81
LABEL $NOTNUL81
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@string
EQS
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@bool
EQS
ORS
PUSHS bool@true
JUMPIFEQS $FAIL79
LABEL $tudytamskip81
TYPE TF@res TF@a
PUSHS TF@res
TYPE TF@res TF@b
PUSHS TF@res
JUMPIFEQS $ADD81
TYPE TF@res TF@a
PUSHS TF@res
PUSHS string@int
JUMPIFNEQS $ADDLABEL181
INT2FLOAT TF@a TF@a
LABEL $ADDLABEL181
TYPE TF@res TF@b
PUSHS TF@res
PUSHS string@int
JUMPIFNEQS $ADDLABEL281
INT2FLOAT TF@b TF@b
LABEL $ADDLABEL281
LABEL $ADD81
ADD TF@res TF@a TF@b
PUSHS TF@res
JUMP $EXIT81
LABEL $FAIL81
EXIT int@57
LABEL $EXIT81
POPS LF@i
CLEARS
JUMP $WhileStart70
LABEL $WhileEnd70
TYPE LF@$optype LF@ret
PUSHS LF@$optype
PUSHS string@
JUMPIFNEQS checkinitializedgood82
EXIT int@5
LABEL checkinitializedgood82
PUSHS LF@ret
CREATEFRAME
DEFVAR TF@val2convert
POPS TF@val2convert
CALL intval
POPS LF@ret
CLEARS
TYPE LF@$optype LF@ret
PUSHS LF@$optype
PUSHS string@
JUMPIFNEQS checkinitializedgood83
EXIT int@5
LABEL checkinitializedgood83
PUSHS LF@ret
POPFRAME
RETURN
*/