/**
 * Project: IFJ22 Compiler
 * 
 * @file lexer.c
 * @brief Implementation of lexer
 * 
 * @authors xmacur09, xfrust00, xstect00, xbucka00
*/

#include "lexer.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


lexer_T *lexer_init() {
    lexer_T *lexer = calloc(1, sizeof(struct lexer_struct));


    lexer->state = STATE_START;
    lexer->i = 0;
    lexer->prologParsed = false;
    lexer_advance(lexer);

    return lexer;
}


void lexer_free(lexer_T *lexer) {
    free(lexer);
}


void lexer_advance(lexer_T *lexer) {
    if (lexer->c != EOF) {
        lexer->i += 1;
        lexer->c = getc(stdin);
    }
}


void lexer_skip_comment(lexer_T *lexer) {
    if (lexer->c != '/')
        return;

    char prev;
    int block_start;

    lexer_advance(lexer);
    switch (lexer->c) {
    case '/':
        while (lexer->c != '\n' && lexer->c != EOF) {
            lexer_advance(lexer);
        }
        lexer_skip_whitespace(lexer);
        break;
    case '*':
        prev = '/';
        block_start = 0;
        while ((prev != '*' || lexer->c != '/') && lexer->c != EOF) {
            if (block_start)
                prev = lexer->c;
            if (lexer->c == '*')
                block_start = 1;
            lexer_advance(lexer);
        }
        if (lexer->c == '/')
            lexer_advance(lexer);
        lexer_skip_whitespace(lexer);
        break;
    }
}


void lexer_skip_whitespace(lexer_T *lexer) {
    while (lexer->c == ' ' || lexer->c == '\r' || lexer->c == '\t' || lexer->c == '\n') {
        lexer_advance(lexer);
    }
}


int isoctdigit(char c) { return c >= '0' && c <= '7'; }


int is_keyword(char *src) {
    
    if (!strcmp(src, "else")) {
        return KW_ELSE;
    }
    else if (!strcmp(src, "float")) {
        return KW_FLOAT;
    }
    else if (!strcmp(src, "?float")) {
        return KW_FLOAT_NULL;
    }
    else if (!strcmp(src, "function")) {
        return KW_FUNCTION;
    }
    else if (!strcmp(src, "if")) {
        return KW_IF;
    }
    else if (!strcmp(src, "int")) {
        return KW_INT;
    }
    else if (!strcmp(src, "?int")) {
        return KW_STRING_NULL;
    }
    else if (!strcmp(src, "null")) {
        return KW_NULL;
    }
    else if (!strcmp(src, "return")) {
        return KW_RETURN;
    }
    else if (!strcmp(src, "string")) {
        return KW_STRING;
    }
    else if (!strcmp(src, "?string")) {
        return KW_STRING_NULL;
    }
    else if (!strcmp(src, "void")) {
        return KW_VOID;
    }
    else if (!strcmp(src, "while")) {
        return KW_WHILE;
    }
    return 0;
}


void clean_string(char **str) {
    size_t str_len = strlen(*str);

    char *new_str = chararray_init(str_len +1);
    if (new_str == NULL) {
        // TODO error
    }

    for (size_t i = 0, n_i = 0; i < str_len; i++) {
        char c = (char)0;

        if ((*str)[i] == '$') { // escape character
            printf("Unescaped $\n");
            continue;
        }

        if ((*str)[i] == '\\') {
            switch ((*str)[i + 1]) {
            case 'x':
                if (i + 3 < str_len && isxdigit((*str)[i + 2]) && // check if the next two characters are hex digits
                    isxdigit((*str)[i + 3])) {
                    char hex[3] = {(*str)[i + 2], (*str)[i + 3], '\0'};
                    int num = (int)strtol(hex, NULL, 16);
                    if (num >= 0x01 && num <= 0xFF) { // check if the number is in the range of 1-255
                        c = num;
                        i += 3;
                    }
                    else {
                        c = (*str)[i];
                    }
                }
                else {
                    c = (*str)[i]; // take the backslash
                }
                break;
            case '"':
                c = '"';
                i++;
                break;
            case 'n':
                c = '\n';
                i++;
                break;
            case 't':
                c = '\t';
                i++;
                break;
            case '\\':
                c = '\\';
                i++;
                break;
            case '$':
                c = '$';
                i++;
                break;
            default:
                if (i + 3 < str_len && isoctdigit((*str)[i + 1]) && isoctdigit((*str)[i + 2]) && isoctdigit((*str)[i + 3])) { // check if the next three characters are octal digits
                    char oct[4] = {(*str)[i + 1], (*str)[i + 2], (*str)[i + 3], '\0'};
                    int num = (int)strtol(oct, NULL, 8);
                    if (num >= 1 && num <= 255) { // check if the number is in the range of 1-255
                        c = num;
                        i += 3;
                    }
                    else {
                        c = (*str)[i];
                    }
                }
                else {
                    c = (*str)[i];
                }
            }
        }
        else {
            c = (*str)[i];
        }
        new_str[n_i++] = c;
    }

    char *str_final = chararray_init(4*strlen(new_str) +1);
    //if (str_final == NULL) {
    //   return INTERNAL_ERR;
    //}

    for (size_t i = 0; i < strlen(new_str); i++) {
        char num[7] = {(char)0};
        char c_as_string[2];

        c_as_string[0] = new_str[i];
        c_as_string[1] = '\0';

        if ((new_str[i] >= 0 && new_str[i] <= 32) || new_str[i] == 35 || new_str[i] == 92) { // 0-32, 35, 92
            sprintf(num, "\\0%d", new_str[i]);
            strcat(str_final, num);
        } else {
            strcat(str_final, c_as_string);
        }
    }

    chararray_free(new_str);
    chararray_free(*str);

    *str = str_final;
}


void str_to_double(char *string) { 
    int stringCounter = 0;
    int CELCounter = 0;
    int EXPCounter = 0;
    char sign = '+';
    char arrayCEL[100];
    char arrayEXP[10];
    arrayEXP[0] = '1';
    enum stav mojstav = CEL;
    while (1) {
        if (mojstav == CEL && (string[stringCounter] >= '0' && string[stringCounter] <= '9')) { // if the character is a digit
            arrayCEL[CELCounter] = string[stringCounter];
            stringCounter++;
            CELCounter++;
        }
        else if (mojstav == CEL && string[stringCounter] == '.') { // if the character is a dot
            // convert arrayCEL to integer
            int cel = atoi(arrayCEL);
            if (cel >= 1000000000) 
            {
                printf("Chyba: cislo je prilis velke");
            }
            arrayCEL[CELCounter++] = '.';
            stringCounter++;
        }
        else if (mojstav == CEL && (string[stringCounter] == 'e' || string[stringCounter] == 'E')) { // if the character is e or E
            mojstav = EXP;
            stringCounter++;
        }
        else if (mojstav == EXP && (string[stringCounter] == '+' || string[stringCounter] == '-')) { // if the character is + or -
            sign = string[stringCounter];
            stringCounter++;
        }
        else if (mojstav == EXP && (string[stringCounter] >= '0' && string[stringCounter] <= '9')) { // if the character is a digit
            arrayEXP[EXPCounter] = string[stringCounter];
            stringCounter++;
            EXPCounter++;
        }
        else if (string[stringCounter] == '\0') {
            arrayEXP[EXPCounter] = '\0';
            arrayCEL[CELCounter] = '\0';
            break;
        }
        else {
            printf("Chyba vstupu");
            
        }
    }

    

    char str[100];
    char exponent[100];
    strcpy(str, arrayCEL);
    strcpy(exponent, arrayEXP);
    double exponentNumber = atof(exponent);
    double doubleNum = atof(str);

    if (sign == '-') {
        for (int i = 0; i < exponentNumber; i++) {
            doubleNum = doubleNum / 10;
        }
        printf("%f\n", doubleNum);
    }
    else {
        double outcome = round(pow(10, exponentNumber));
        outcome = doubleNum * outcome;
        printf("%a\n", outcome);
    }
}


error lexer_next_token(lexer_T *lexer, token *Token) {
    char prolog[] = "<?phpdeclare(strict_types=1);";

    if (!lexer->prologParsed) {
        for (int i = 0; i < 29; i++) { // length of prolog
            if (i == 5) {              // after <?php
                lexer_skip_whitespace(lexer);
                lexer_skip_comment(lexer);
            }
            if (lexer->c != prolog[i]) {
                if (i >= 5)
                    return LEXICAL_ERR; // error after <?php
                else
                    return SYNTAX_ERR; // missing <?php tag
            }
            lexer_advance(lexer);
        }
        lexer->prologParsed = true;
    }



    char *value = chararray_init(0);
    unsigned int char_i; // used eg char_i=lexer->i to track number of loaded characters

    while (1) {
        switch (lexer->state) {

        case STATE_START:
            lexer_skip_whitespace(lexer);

            if (lexer->c == '$') {
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_START;
            }
            else if (isalpha(lexer->c) || lexer->c == '_') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_IDENTIFIER_OR_KEYWORD_E;
            }
            else if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_E;
            }

            else if (lexer->c == '?') {
                lexer_advance(lexer);
                lexer->state = STATE_QUESTIONMARK;
            }

            else if (lexer->c == '!') {
                lexer_advance(lexer);
                lexer->state = STATE_NOT;
            }

            else if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_EQ_E;
            }
            else if (lexer->c == '>') {
                lexer_advance(lexer);
                lexer->state = STATE_GT_E;
            }
            else if (lexer->c == '<') {
                lexer_advance(lexer);
                lexer->state = STATE_ST_E;
            }

            else if (lexer->c == '+') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_PLUS;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }
            else if (lexer->c == '-') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_MINUS;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }
            else if (lexer->c == '*') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_MULTIPLICATION;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }
            else if (lexer->c == '/') {
                lexer->state = STATE_SLASH;
                // without lexer advance. lexer_skip_comment takes care
            }

            else if (lexer->c == '.') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_CONCAT;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }
            else if (lexer->c == '"') {
                lexer_advance(lexer);
                lexer->state = STATE_QUOTATION_CENTER_E;
                break;
            }

            else if (lexer->c == '(') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_LBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }

            else if (lexer->c == ')') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_RBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }
            else if (lexer->c == '{') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_LCURLYBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }

            else if (lexer->c == '}') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_RCURLYBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }

            else if (lexer->c == ';') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_SEMICOLLON;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }

            else if (lexer->c == ',') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_COMMA;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }

            else if (lexer->c == ':') {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_COLON;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return SUCCESS;
            }

            else if (lexer->c == EOF) {
                Token->ID = TOKEN_ID_EOF;
                Token->VAL.string = value;
                return SUCCESS;
            }

            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_VARIABLE_START:
            if (isalpha(lexer->c) || lexer->c == '_') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_E;
            }
            else {
                lexer->state = STATE_START;
                return LEXICAL_ERR;
            }
            break;

        case STATE_VARIABLE_E:
            if (isalnum(lexer->c) || lexer->c == '_') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_VARIABLE;
                Token->VAL.string = value;
                return SUCCESS;
            }
            break;

        case STATE_IDENTIFIER_OR_KEYWORD_E:
            if (isalnum(lexer->c) || lexer->c == '_') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                keyword kw = is_keyword(value);
                if (kw) {
                    lexer->state = STATE_START;
                    Token->ID = TOKEN_ID_KEYWORD;
                    Token->VAL.keyword = kw;
                }
                else if (value[0] == '?') {
                    lexer->state = STATE_START;
                    lexer_advance(lexer);
                    return LEXICAL_ERR;
                }
                else {
                    lexer->state = STATE_START;
                    Token->ID = TOKEN_ID_IDENTIFIER;
                    Token->VAL.string = value;
                    
                }
                return SUCCESS;
            }
            break;

        case STATE_INTEGER_E:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            else if (lexer->c == '.') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_AND_SEPARATOR;
            }
            else if (tolower(lexer->c) == 'e') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_START;
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL.string = value;
                return SUCCESS;
            }
            break;

        case STATE_INTEGER_AND_SEPARATOR:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_E;
                break;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_DOUBLE_E:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            else if (tolower(lexer->c) == 'e') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_START;
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_FLOAT;
                Token->VAL.string = value;
                return SUCCESS;
            }
            break;

        case STATE_INTEGER_EXPONENT_START:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_E;
            }
            else if (lexer->c == '+' || lexer->c == '-') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_SIGN;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_INTEGER_EXPONENT_SIGN:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_E;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_INTEGER_EXPONENT_E:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL.string = value;
                return SUCCESS;
            }
            break;

        case STATE_DOUBLE_EXPONENT_START:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_E;
            }
            else if (lexer->c == '+' || lexer->c == '-') {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_SIGN;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_DOUBLE_EXPONENT_SIGN:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_E;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_DOUBLE_EXPONENT_E:
            if (isdigit(lexer->c)) {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL.string = value;
                return SUCCESS;
            }
            break;

        case STATE_NOT:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_NOT_EQ;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_NOT_EQ:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_NOT_EQUALS;
                Token->VAL.string = value;
                return SUCCESS;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }
            break;

        case STATE_EQ_E:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_EQEQ;
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_EQUALS;
                Token->VAL.string = value;
                return SUCCESS;
            }
            break;

        case STATE_EQEQ:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_TRIPLE_EQUALS;
                Token->VAL.string = value;
                return SUCCESS;
            }
            else {
                lexer_advance(lexer);
                return LEXICAL_ERR;
            }

        case STATE_GT_E:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_GTE;
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_GT;
            }
            Token->VAL.string = value;
            return SUCCESS;

        case STATE_ST_E:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_LTE;
            }
            else {
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_LT;
            }
            Token->VAL.string = value;
            return SUCCESS;

        case STATE_QUOTATION_CENTER_E:
            if (lexer->c == '"') {
                lexer->state = STATE_START;
                lexer_advance(lexer);
                clean_string(&value);
                Token->ID = TOKEN_ID_STRING;
                Token->VAL.string = value;
                return SUCCESS;
            }
            else if (lexer->c == '\\') {
                lexer->state = STATE_QUOTATION_ESCAPE_CHAR;
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                chararray_append(&value, lexer->c);
                lexer_advance(lexer);
            }
            break;

        case STATE_QUOTATION_ESCAPE_CHAR:
            lexer->state = STATE_QUOTATION_CENTER_E;
            chararray_append(&value, lexer->c);
            lexer_advance(lexer);
            break;

        case STATE_SLASH:
            char_i = lexer->i;
            lexer_skip_comment(lexer);
            lexer->state = STATE_START;
            if (lexer->i - char_i == 1) { // moved just one character ie no comment skipped
                Token->ID = TOKEN_ID_DIVISION;
                Token->VAL.string = value;
                return SUCCESS;
            }
            break;

        case STATE_QUESTIONMARK:
            if (lexer->c == '>') {
                lexer->state = STATE_CLOSING_TAG;
                lexer_advance(lexer);
            }
            else {
                chararray_append(&value, '?');
                lexer->state = STATE_IDENTIFIER_OR_KEYWORD_E;
            }
            break;

        case STATE_CLOSING_TAG:
            if (lexer->c == EOF)
                lexer->state = STATE_START;
            else {
                lexer->state = STATE_START;
                lexer_advance(lexer);
                return SYNTAX_ERR; // syntactic error
            }
            break;
        }
    }
}
