
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

#define FUNCTION_FLOAT_VALUE "LABEL $float_value\n\
PUSHS GF@%s\n\
TYPE GF@%s_type\n\


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