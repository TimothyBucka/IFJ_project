
#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>
#include <analysis.c>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { //all instructions from IFJcode22
    INS_MOVE,
    INS_CREATEFRAME,
    INS_PUSHFRAME,
    INS_POPFRAME,
    INS_DEFVAR,
    INS_CALL,
    INS_RETURN,
    INS_PUSHS, //functions
    INS_POPS,
    INS_CLEARS, //stack
    INS_ADD,
    INS_SUB,
    INS_MUL,
    INS_IDIV,
    INS_LT,
    INS_GT, //zevraj sa nepouziva ADAM POVEDAL XD ale dam ho tu lebo je v zadani //FIXME
    INS_EQ,
    INS_AND,
    INS_OR,
    INS_NOT,
    INS_INT2FLOAT,
    INS_FLOAT2INT,
    INS_INT2CHAR,
    INS_STRI2INT, //for calculating idk how to say it in english
    INS_READ,
    INS_WRITE, //input output
    INS_CONCAT,
    INS_STRLEN,
    INS_GETCHAR,
    INS_SETCHAR, //string operations
    INS_TYPE,
    INS_LABEL,
    INS_JUMP,
    INS_JUMPIFEQ,
    INS_JUMPIFNEQ, //flow control
    INS_EXIT,
    INS_BREAK ,
    INS_DPRINT //debugging
} INSTRUCTIONS;

#define FUNCTION_ORD "LABEL $ord\n\
DEFVAR LF@&1type\n\
DEFVAR LF@&2type\n\
DEFVAR LF@&1len\n\
TYPE LF@&1type LF@&1\n\
TYPE LF@&2type LF@&2\n\
JUMPIFEQ $firstargstr LF@&1type string@string\n\
EXIT int@4\n\
LABEL $firstargstr\n\
JUMPIFEQ $ordlengthcmp LF@&2type string@int\n\
EXIT int@4\n\
LABEL $ordlengthcmp\n\
STRLEN LF@&1len LF@&1\n\
\
DEFVAR LF@&2lencheck\n\
LT LF@&2lencheck LF@&2 int@0\n\
JUMPIFEQ $ordoutbounds LF@&2lencheck bool@true\n\
LT LF@&2lencheck LF@&2 LF@&1len\n\
JUMPIFEQ $ordoutbounds LF@&2lencheck bool@false\n\
\
STRI2INT LF@&RETVAL LF@&1 LF@&2\n\
RETURN\n\
LABEL $ordoutbounds\n\
MOVE LF@&RETVAL nil@nil\n\
RETURN\n"

#define FUNCTION_CHR "LABEL $chr\n\
DEFVAR LF@&1type\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $chrtypecmp LF@&1type string@int\n\
EXIT int@4\n\
LABEL $chrtypecmp\n\
DEFVAR LF@&1check\n\
LT LF@&1check LF@&1 int@0\n\
JUMPIFEQ $chroutbounds LF@&1check bool@true\n\
GT LF@&1check LF@&1 int@255\n\
JUMPIFEQ $chroutbounds LF@&1check bool@true\n\
\
INT2CHAR LF@&RETVAL LF@&1\n\
RETURN\n\
LABEL $chroutbounds\n\
MOVE LF@&RETVAL nil@nil\n\
RETURN\n"

#define FUNCTION_STRLEN "LABEL $strlen\n\
DEFVAR LF@&1type\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $strlenstr LF@&1type string@string\n\
EXIT int@4\n\
LABEL $strlenstr\n\
STRLEN LF@&RETVAL LF@&1\n\
RETURN\n"

#define FUNCTION_SUBSTR "LABEL $substr\n\
DEFVAR LF@&1type\n\
DEFVAR LF@&2type\n\
DEFVAR LF@&3type\n\
TYPE LF@&1type LF@&1\n\
TYPE LF@&2type LF@&2\n\
TYPE LF@&3type LF@&3\n\
JUMPIFEQ $substrstr LF@&1type string@string\n\
EXIT int@4\n\
LABEL $substrstr\n\
JUMPIFEQ $substrint LF@&2type string@int\n\
EXIT int@4\n\
LABEL $substrint\n\
JUMPIFEQ $substrint2 LF@&3type string@int\n\
EXIT int@4\n\
LABEL $substrint2\n\
\
DEFVAR LF@&2check\n\
LT LF@&2check LF@&2 int@0\n\
JUMPIFEQ $substroutbounds LF@&2check bool@true\n\
\
DEFVAR LF@&1len\n\
STRLEN LF@&1len LF@&1\n\
LT LF@&2check LF@&2 LF@&1len\n\
JUMPIFEQ $substroutbounds LF@&2check bool@false\n\
\
DEFVAR LF@&3check\n\
LT LF@&3check LF@&3 int@0\n\
JUMPIFEQ $substroutbounds LF@&3check bool@true\n\
\
DEFVAR LF@&3lencheck\n\
ADD LF@&3lencheck LF@&2 LF@&3\n\
LT LF@&3lencheck LF@&3lencheck LF@&1len\n\
JUMPIFEQ $substroutbounds LF@&3lencheck bool@false\n\
\
DEFVAR LF@&3lencheck2\n\
ADD LF@&3lencheck2 LF@&2 LF@&3\n\
GT LF@&3lencheck2 LF@&3lencheck2 int@0\n\
JUMPI

#define FUNCTION_SUBSTR "LABEL $substr\n\
LABEL $substrtypecmp\n\
DEFVAR LF@&2type\n\
TYPE LF@&2type LF@&2\n\
JUMPIFEQ $substrtypecmp2 LF@&2type string@int\n\
EXIT int@4\n\
LABEL $substrtypecmp2\n\
DEFVAR LF@&3type\n\
TYPE LF@&3type LF@&3\n\
JUMPIFEQ $substrtypecmp3 LF@&3type string@int\n\
EXIT int@4\n\
LABEL $substrtypecmp3\n\
DEFVAR LF@&2check\n\
LT LF@&2check LF@&2 int@0\n\
JUMPIFEQ $substroutbounds LF@&2check bool@true\n\
DEFVAR LF@&3check\n\
LT LF@&3check LF@&3 int@0\n\
JUMPIFEQ $substroutbounds LF@&3check bool@true\n\
\
STRLEN LF@&RETVAL LF@&1\n\
DEFVAR LF@&1lencheck\n\
LT LF@&1lencheck LF@&RETVAL LF@&2\n\
JUMPIFEQ $substroutbounds LF@&1lencheck bool@true\n\
\
DEFVAR LF@&3lencheck\n\
LT LF@&3lencheck LF@&RETVAL LF@&3\n\
JUMPIFEQ $substroutbounds LF@&3lencheck bool@true\n\
\
DEFVAR LF@&2endcheck\n\
LT LF@&2endcheck LF@&2 LF@&3\n\
JUMPIFEQ $substroutbounds LF@&2endcheck bool@true\n\
\
DEFVAR LF@&3endcheck\n\
LT LF@&3endcheck LF@&3 LF@&2\n\
JUMPIFEQ $substroutbounds LF@&3endcheck bool@true\n\
\
DEFVAR LF@&2len\n\
DEFVAR LF@&3len\n\
SUB LF@&2len LF@&3 LF@&2\n\
ADD LF@&3len LF@&2 LF@&3\n\
\
DEFVAR LF@&2lencheck\n\
DEFVAR LF@&3lencheck\n\
LT LF@&2lencheck LF@&2 int@0\n\
LT LF@&3lencheck LF@&3 int@0\n\
JUMPIFEQ $substroutbounds LF@&2lencheck bool@true\n\
JUMPIFEQ $substroutbounds LF@&3lencheck bool@true\n\
RETURN\n"

#endif