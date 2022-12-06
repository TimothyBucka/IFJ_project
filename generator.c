#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "generator.h"
#include "chararray.h"


	// (FUNCTION_SUBSTR);
	// (FUNCTION_CHR);
    // (FUNCTION_ORD);
    // (FUNCTION_STRLEN);
    // // (FUNCTION_READS); //read string
    // // (FUNCTION_READI); //read int
    // // (FUNCTION_READF); //read float
    // // (FUNCTION_WRITE);
    // // (FUNCTION_FLOATVAL);
    // // (FUNCTION_INTVAL);
    // // (FUNCTION_STRVAL);


	// ("# Start of program");

	// (".IFJcode22");

	// ("DEFVAR GF@%input_prompt");
	// ("MOVE GF@%input_prompt string@?\\032");

	// ("DEFVAR GF@%tmp_op1");
	// ("DEFVAR GF@%tmp_op2");
	// ("DEFVAR GF@%tmp_op3");

	// ("DEFVAR GF@%exp_result");

	// ("JUMP $$main");


    void start_of_generator(){
        printf(START);
        
        gen_fun_reads();
        gen_fun_readi();
        gen_fun_readf();
        gen_fun_write();
        gen_fun_floatval();
        gen_fun_intval();
        gen_fun_strval();
        gen_fun_strlen();
        gen_fun_substr();
        gen_fun_ord();
        gen_fun_chr();

        printf("LABEL $$main\n");
    }

    void gen_fun_reads(){
        printf(FUNCTION_READS); 
    }

    void gen_fun_readi(){
        //TODO printf(FUNCTION_READI); 
    }

    void gen_fun_readf(){
        //TODO printf(FUNCTION_READF); 
    }

    void gen_fun_write(){
        //TODO printf(FUNCTION_WRITE); 
    }

    void gen_fun_floatval(){
        //TODO printf(FUNCTION_FLOATVAL); 
    }

    void gen_fun_intval(){
        //TODO printf(FUNCTION_INTVAL); 
    }

    void gen_fun_strval(){
        //TODO printf(FUNCTION_STRVAL); 
    }

    void gen_fun_strlen(){
        printf(FUNCTION_STRLEN); 
    }

    void gen_fun_substr(){
        printf(FUNCTION_SUBSTR); 
    }

    void gen_fun_ord(){
        printf(FUNCTION_ORD); 
    }

    void gen_fun_chr(){
        printf(FUNCTION_CHR); 
    }