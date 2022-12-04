
#ifndef CODE_GEN_LIST_H
#define CODE_GEN_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define FUN_LENGTH "LABEL $length\n\
DEFVAR LF@&1type\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $lengthcmp LF@&1type string@string\n\
EXIT int@4\n\
LABEL $lengthcmp\n\
STRLEN LF@&RETVAL LF@&1\n\
RETURN\n"

#define FUN_ORD "LABEL $ord\n\
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

#define FUN_SUBSTR "LABEL $substr\n\
DEFVAR LF@&1type    #checking types of params\n\
TYPE LF@&1type LF@&1\n\
DEFVAR LF@&2type\n\
TYPE LF@&2type LF@&2\n\
DEFVAR LF@&3type\n\
TYPE LF@&3type LF@&3\n\
JUMPIFEQ $substrfirstargstr LF@&1type string@string\n\
EXIT int@4\n\
LABEL $substrfirstargstr\n\
JUMPIFEQ $substrsecondargint LF@&2type string@int\n\
EXIT int@4\n\
LABEL $substrsecondargint\n\
JUMPIFEQ $substrthirdargint LF@&3type string@int\n\
EXIT int@4\n\
LABEL $substrthirdargint\n\
\
DEFVAR LF@&1len\n\
STRLEN LF@&1len LF@&1   #0 < second par < len\n\
DEFVAR LF@&2check\n\
LT LF@&2check LF@&2 int@0\n\
JUMPIFEQ $substroutbounds LF@&2check bool@true\n\
LT LF@&2check LF@&2 LF@&1len\n\
JUMPIFEQ $substroutbounds LF@&2check bool@false\n\
\
DEFVAR LF@&3check   #second par can't be < 0\n\
LT LF@&3check LF@&3 int@0\n\
JUMPIFEQ $substroutbounds LF@&3check bool@true\n\
\
DEFVAR LF@&lastidx\n\
DEFVAR LF@&charbuff\n\
MOVE LF@&RETVAL string@\n\
ADD LF@&lastidx LF@&2 LF@&3\n\
DEFVAR LF@&lastidxcheck\n\
LT LF@&lastidxcheck LF@&lastidx LF@&1len\n\
JUMPIFEQ $substrcyclebegin LF@&lastidxcheck bool@true\n\
MOVE LF@&lastidx LF@&1len\n\
\
LABEL $substrcyclebegin\n\
JUMPIFEQ $substrcycleend LF@&2 LF@&lastidx\n\
GETCHAR LF@&charbuff LF@&1 LF@&2\n\
CONCAT LF@&RETVAL LF@&RETVAL LF@&charbuff\n\
ADD LF@&2 LF@&2 int@1\n\
JUMP $substrcyclebegin\n\
LABEL $substrcycleend\n\
RETURN\n\
\
LABEL $substroutbounds\n\
MOVE LF@&RETVAL nil@nil\n\
RETURN\n"

#define FUN_CHR "LABEL $chr\n\
DEFVAR LF@&1type\n\
DEFVAR LF@&1lencheck\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $argint LF@&1type string@int\n\
EXIT int@4\n\
LABEL $argint\n\
\
LT LF@&1lencheck LF@&1 int@0\n\
JUMPIFEQ $chroutbounds LF@&1lencheck bool@true\n\
GT LF@&1lencheck LF@&1 int@255\n\
JUMPIFEQ $chroutbounds LF@&1lencheck bool@true\n\
\
INT2CHAR LF@&RETVAL LF@&1\n\
RETURN\n\
LABEL $chroutbounds\n\
EXIT int@4\n\
RETURN\n"

#define FUN_GENPLUSE "LABEL $$genplusE\n\
DEFVAR LF@&1type\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $$genplusEok LF@&1type string@int\n\
JUMPIFEQ $$genplusEok LF@&1type string@int\n\
EXIT int@4\n\
LABEL $$genplusEok\n\
RETURN\n"

#define FUN_GENMINUSE "LABEL $$genminusE\n\
DEFVAR LF@&RETVAL\n\
DEFVAR LF@&1type\n\
TYPE LF@&1type LF@&1\n\
JUMPIFEQ $$genminusEint LF@&1type string@int\n\
JUMPIFEQ $$genminusEfloat LF@&1type string@int\n\
EXIT int@4\n\
LABEL $$genminusEint\n\
SUB LF@&1 int@0 LF@&1\n\
MOVE LF@&RETVAL LF@&1\n\
RETURN\n\
LABEL $$genminusEfloat\n\
SUB LF@&1 int@0 LF@&1\n\
MOVE LF@&RETVAL LF@&1\n\
RETURN\n"



#define ADRCAT(adr, var1, var2) (adr.val.s = getStr(2, var1, var2), adr)

#define ADRNIL(adr)      (adr.type = ADRTYPE_NIL, adr)

                                // Usage of comma operator like this: (A,(B,C)) => A, B - execution of commands; C - parameter to a function
#define ADRINT(adr, var) (  (   adr.val.i = var,                        \
                                (adr.type = ADRTYPE_INT, adr)   )  )

#define ADRFLT(adr, var) (  (   adr.val.f = var,                        \
                                (adr.type = ADRTYPE_FLOAT, adr)   )  )

#define ADRSTR(adr, var) (  (   adr.val.s = getIfjCodeStr(var),             \
                                (adr.type = ADRTYPE_STRING, adr)   )  )

#define ADRBOOL(adr, var) (  (   adr.val.s = getStr(1, var),             \
                                (adr.type = ADRTYPE_BOOL, adr)   )  )

#define ADRVAR(adr, var) (  (   adr.val.s = getStr(1, var),             \
                                (adr.type = ADRTYPE_VAR, adr)   )  )

#define ADRVARTMP(adr, var) (  (   adr.val.s = getStr(1, var),             \
                                (adr.type = ADRTYPE_VAR_TMP, adr)   )  )

#define ADRLAB(adr, var1, var2) (  (   adr.val.s = getStr(2, var1, var2),             \
                                      (adr.type = ADRTYPE_LABEL, adr)   )  )

#define INSERTLAST(L, inst, inWhile, inFunDef) do{\
                        inFunDef ? ILPreInsertLabMain(L, inst, inWhile) : ILInsertLast(L, inst, inWhile);\
                        if (errflg != PROGRAM_OK)\
                            return;} while(0)

#define INSERTBEFOREWHILE(L, inst, inWhile, inFunDef) do{\
                        inFunDef ? ILPostInsertInFunDefBeforeWhile(L, inst) : ILPostInsertBeforeWhile(L, inst);\
                        if (errflg != PROGRAM_OK)\
                            return;} while(0)

typedef enum {      // Adresses
    OP_DEFVAR,      // 1
    OP_MOVE,        // 2
    OP_TYPE,        // 2
    OP_JUMPIFEQ,    // 3
    OP_LABEL,       // 1
    OP_EXIT,        // 1
    OP_JUMPIFNEQ,   // 3
    OP_INT2FLOAT,   // 2
    OP_FLOAT2INT,   // 2
    OP_JUMP,        // 1
    OP_CONCAT,      // 3
    OP_PUSHFRAME,   // 0
    OP_CREATEFRAME, // 0
    OP_POPFRAME,    // 0
    OP_RETURN,      // 0
    OP_CALL,        // 1
    OP_ADD,         // 3
    OP_SUB,         // 3
    OP_MUL,         // 3
    OP_DIV,         // 3
    OP_LT,          // 3
    OP_GT,          // 3
    OP_LEQ,         // 3
    OP_GEQ,         // 3
    OP_EQ,          // 3
    OP_NEQ,         // 3
    OP_NOT,         // 2
    OP_WRITE,       // 1
    OP_READ         // 1
} TOperation;

typedef enum {
    ADRTYPE_VAR,
    ADRTYPE_VAR_TMP,
    ADRTYPE_INT,
    ADRTYPE_FLOAT,
    ADRTYPE_STRING,
    ADRTYPE_BOOL,
    ADRTYPE_NIL,
    ADRTYPE_LABEL
} TAdrVal;

typedef struct {
    TAdrVal type;
    union {
        int i;
        double f;
        char *s;
    } val;
} TAdr;

typedef struct {
    TOperation op;
    TAdr adr1;
    TAdr adr2;
    TAdr adr3;
} TInst;

typedef struct TILElem {
    TInst inst;
    struct TILElem *lptr;
    struct TILElem *rptr;
} *TILElemPtr;

typedef struct {
    TILElemPtr First;
    TILElemPtr BeforeWhile;
    TILElemPtr LabMain;
    TILElemPtr InFunDefBeforeWhile;
    TILElemPtr Last;
} TInstrList;

/**
 * @brief Initialises an empty list
 * 
 * @param L Pointer to a list
 */
void ILInit (TInstrList *L);

/**
 * @brief Sets BeforeWhile pointer to point to the last element in the main body of the program
 * 
 * @param L Pointer to a list
 */
void ILSetBeforeWhile (TInstrList *L);

/**
 * @brief Sets InFunDefBeforeWhile pointer to point to an element located left from the main label
 * 
 * @param L Pointer to a list
 */
void ILSetInFunDefBeforeWhile (TInstrList *L);

/**
 * @brief Sets LabMain pointer to point to the first element
 * 
 * @param L Pointer to a list
 */
void ILSetLabMain (TInstrList *L);

/**
 * @brief Disposes a list
 * 
 * @param L Pointer to a list
 */
void ILDisposeList(TInstrList *L);

/**
 * @brief Inserts an element to the first position
 * 
 * @param L Pointer to a list
 * @param inst Instruction to be added to a new element
 */
void ILInsertFirst (TInstrList *L, TInst inst);

/**
 * @brief Inserts an element to the last position
 * 
 * @param L Pointer to a list
 * @param inst Instruction to be added to a new element 
 * @param inWhile Flag whether parser is currently inside a while cycle
 */
void ILInsertLast(TInstrList *L, TInst inst, bool inWhile);

/**
 * @brief Inserts an element left from the main label
 * 
 * @param L Pointer to a list
 * @param inst Instruction to be added to a new element 
 * @param inWhile Flag whether parser is currently inside a while cycle
 */
void ILPreInsertLabMain (TInstrList *L, TInst inst, bool inWhile);

/**
 * @brief Inserts an element to the right of the BeforeWhile pointer
 * 
 * @param L Pointer to a list
 * @param inst Instruction to be added to a new element 
 */
void ILPostInsertBeforeWhile (TInstrList *L, TInst inst);

/**
 * @brief Inserts an element to the right of the InFunDefBeforeWhile pointer
 * 
 * @param L Pointer to a list
 * @param inst Instruction to be added to a new element 
 */
void ILPostInsertInFunDefBeforeWhile (TInstrList *L, TInst inst);

/**
 * @brief Internal function to print a value stored in an adress 
 * 
 * @param adr Adress
 */
static void printAdr(TAdr adr);

/**
 * @brief Internal function to print a value stored in an instruction
 * 
 * @param inst Instruction
 */
static void printInst(TInst inst);

/**
 * @brief Prints all instructions, adds built in functions and header to the beginning
 * 
 * @param L Pointer to a list
 */
void ILPrintAllInst(TInstrList L);

/**
 * @brief Dynamically allocates space to concatenate C-strings
 * 
 * @param n Number of strings to concatenate
 * @param ... Strings
 * @return char* 
 */
char *getStr(int n, ...);

/**
 * @brief Dynamically alocates space and converts C-string to IFJcode18 string
 * 
 * @param s C-string
 * @return char* 
 */
char *getIfjCodeStr(char *s);

/**
 * @brief Fills TInst structure with adresses and returns it
 * 
 * @param op Operation
 * @param adr1 Adress 1
 * @param adr2 Adress 2
 * @param adr3 Adress 3
 * @return TInst 
 */
TInst getInst(TOperation op, TAdr adr1, TAdr adr2, TAdr adr3);

#endif
