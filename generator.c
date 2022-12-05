#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "generator.h"
#include "chararray.h"

static bool built_in_functions()
{
	ADD_INST(FUNCTION_SUBSTR);
	ADD_INST(FUNCTION_CHR);
    ADD_INST(FUNCTION_ORD);
    ADD_INST(FUNCTION_STRLEN);
    // ADD_INST(FUNCTION_READS); //read string
    // ADD_INST(FUNCTION_READI); //read int
    // ADD_INST(FUNCTION_READF); //read float
    // ADD_INST(FUNCTION_WRITE);
    // ADD_INST(FUNCTION_FLOATVAL);
    // ADD_INST(FUNCTION_INTVAL);
    // ADD_INST(FUNCTION_STRVAL);
                 
	return true;
}

static bool generate_file_header()
{
	ADD_INST("# Start of program");

	ADD_INST(".IFJcode22");

	ADD_INST("DEFVAR GF@%input_prompt");
	ADD_INST("MOVE GF@%input_prompt string@?\\032");

	ADD_INST("DEFVAR GF@%tmp_op1");
	ADD_INST("DEFVAR GF@%tmp_op2");
	ADD_INST("DEFVAR GF@%tmp_op3");

	ADD_INST("DEFVAR GF@%exp_result");

	ADD_INST("JUMP $$main");

	return true;
}