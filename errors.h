#ifndef ERRORS_H
#define ERRORS_H

typedef enum error_code {
    SUCCESS,                // 0
    LEXICAL_ERR,            // 1 - chyba programu v ramci lexikalni analyzy (chybna struktura aktualniho lexemu)
    SYNTAX_ERR,             // 2 - chyba v proramu v ramci syntakticke analyzy (chybna syntaxe programu)
    UNDEFINED_FUNCTION_ERR, // 3 - semanticka chyba v rpogramu - nedefinovana funkce, pokus o redefinici funkce
    PARAMETERS_ERR,         // 4 - spatny pocet/typ parametru u volani funkce ci typ navratove hodnoty z funkce
    UNDEFINED_VAR_ERR,      // 5 - nedefinovana promenna
    RETURN_EXPR_ERR,        // 6 - chybejici/prebyvajici vyraz v prikazu navratu z funkce
    EXPR_ERR,               // 7 - typova nekompatibilita v aritmetickych, retezcovych a relacnych vyrazech
    SEM_OTHER_ERR,          // 8 - ostatni semanticke chyby
    INTERNAL_ERR = 99,      // 99 interna chyba prekladace (alokace)
} error;

#endif
