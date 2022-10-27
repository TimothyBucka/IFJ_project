#include "lexer.h"
#include "chararray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_T *lexer_init(char *src) {
    lexer_T *lexer = calloc(1, sizeof(struct lexer_struct));

    /*

    TODO add check

    */

    lexer->src = src;
    lexer->src_size = strlen(src);
    lexer->i = 0;
    lexer->c = src[lexer->i];
    lexer->state = STATE_START;
    lexer->prologParsed = false;

    return lexer;
}

void lexer_free(lexer_T *lexer) {
    free(lexer);
}

void lexer_advance(lexer_T *lexer) {
    if (lexer->i < lexer->src_size && lexer->c != '\0') {
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T *lexer) {
    while (lexer->c == ' ' || lexer->c == '\r' || lexer->c == '\t' || lexer->c == '\n') {
        lexer_advance(lexer);
    }
}

int is_keyword(char *src) {         //TODO ret keyword
    if (!strcmp(src, "else")) {
        return 1;
    } else if (!strcmp(src, "float")) {
        return 2;
    } else if (!strcmp(src, "function")) {
        return 3;
    }
    return 0;
}

void lexer_next_token(lexer_T *lexer, token *Token, int *ended) {
    char prolog[] = "<?php\ndeclare(strict_types=1);";

    if (!lexer->prologParsed)
    {
        while (strlen(prolog) != 0)
        {
            if (lexer->c == prolog[0])
            {
                lexer_advance(lexer);
                memmove(prolog, prolog+1, strlen(prolog));  //remove first char
            }
            else{
                printf("Invalid prolog\n");
                return;
            }
            
        }
        lexer->prologParsed = true;
        
    }
    
    char *value = chararray_init();
    while (1) {
        printf("State %d, character '%c'\n", lexer->state, lexer->c);
        switch (lexer->state) {

        case STATE_START:
            lexer_skip_whitespace(lexer);

            if (lexer->c == '$') {
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_START;
            } else if (isalpha(lexer->c) || lexer->c == '_') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_IDENTIFIER_OR_KEYWORD_E;
            } else if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_E;
            } 
            
            else if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_EQ_E;
            } else if (lexer->c == '>') {
                lexer_advance(lexer);
                lexer->state = STATE_GT_E;
            } else if (lexer->c == '<') {
                lexer_advance(lexer);
                lexer->state = STATE_ST_E;
            }
            

            else if (lexer->c == '+') {
                printf("Token is plus\n");

                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_PLUS;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            } else if (lexer->c == '-') {
                printf("Token is minus\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_MINUS;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            } else if (lexer->c == '*') {
                printf("Token is multiplication\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_MULTIPLICATION;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            } else if (lexer->c == '/') {
                lexer->state = STATE_SLASH;
                lexer_advance(lexer);
            } 
            
            else if (lexer->c == '.') {
                printf("Token is concatenanion\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_CONCAT;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            } else if (lexer->c == '"') {
                // chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_QUOTATION_CENTER_E;
                break;
            }

            else if (lexer->c == '(')
            {
                printf("Token is (\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_LBRACKET;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == ')')
            {
                printf("Token is (\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_RBRACKET;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            }
            else if (lexer->c == '{')
            {
                printf("Token is (\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_LCURLYBRACKET;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == '}')
            {
                printf("Token is (\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_LCURLYBRACKET;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == ';')
            {
                printf("Token is (\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_SEMICOLLON;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == ',')
            {
                printf("Token is (\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_COMMA;
                Token->VAL = tok_val;
                lexer_advance(lexer);
                return;
            }
            

            else if (lexer->c == '\0')
            {
                Token->ID = TOKEN_ID_EOF;
                *ended = 1;
                return;
            }


            else
            {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer_advance(lexer);
            }
            break;

        case STATE_VARIABLE_START:
            if (isalpha(lexer->c) || lexer->c == '_')  
            {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_E;
                break;
            }
            else
            {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer->state = STATE_START;
            }
            break;

        case STATE_VARIABLE_E:
            if (isalnum(lexer->c) || lexer->c == '_') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            } else {
                printf("Token is variable\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_VARIABLE;
                Token->VAL = tok_val;
                return;
            }
            break;

        case STATE_IDENTIFIER_OR_KEYWORD_E:
            if (isalnum(lexer->c) || lexer->c == '_') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            } else {
                if (is_keyword(value)) {
                    printf("Token is keyword %s\n", value);
                    lexer->state = STATE_START;
                    token_VAL tok_val;
                    tok_val.keyword = KW_FUNCTION;
                    Token->ID = TOKEN_ID_KEYWORD;
                    Token->VAL = tok_val;
                } else {
                    printf("Token is identifier\n");
                    lexer->state = STATE_START;
                    token_VAL tok_val;
                    tok_val.string = value;
                    Token->ID = TOKEN_ID_IDENTIFIER;
                    Token->VAL = tok_val;
                }
                return;
            }
            break;

        case STATE_INTEGER_E:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            } 
            else if (lexer->c == '.') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_AND_SEPARATOR;
            } 
            else if (tolower(lexer->c) == 'e') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_START;
            } 
            else {
                printf("Token is integer\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL = tok_val;
                return;
            }
            break;

        case STATE_INTEGER_AND_SEPARATOR:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_E;
                break;
            }
            else
            {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer_advance(lexer);
            }
            break;

        case STATE_DOUBLE_E:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            } else if (tolower(lexer->c) == 'e') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_START;
            } else {
                printf("Token is double\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_DOUBLE;
                Token->VAL = tok_val;
                return;
            }
            break;

        case STATE_INTEGER_EXPONENT_START:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_E;
            } else if (lexer->c == '+' || lexer->c == '-') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_SIGN;
                break;
            }
            else
            {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer_advance(lexer);
            }
            break;

        case STATE_INTEGER_EXPONENT_SIGN:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_E;
            } else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                //FIXME: error
                lexer_advance(lexer);
            }
            break;

        case STATE_INTEGER_EXPONENT_E:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            } else {
                printf("Token is integer exponent\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL = tok_val;
                return;
            }
            break;

        case STATE_DOUBLE_EXPONENT_START:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_E;
            } else if (lexer->c == '+' || lexer->c == '-') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_SIGN;
            } else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                // FIXME: error
                lexer_advance(lexer);
            }
            break;

        case STATE_DOUBLE_EXPONENT_SIGN:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_E;
            } else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                // FIXME: error
                lexer_advance(lexer);
            }
            break;

        case STATE_DOUBLE_EXPONENT_E:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            } else {
                printf("Token is double exponent\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL = tok_val;
                return;
            }
            break;

        case STATE_EQ_E:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_EQEQ;
            } else {
                printf("Token is equals\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_EQUALS;
                return;
            }
            break;

        case STATE_EQEQ:
            if (lexer->c == '=') {
                printf("Token is triple equals\n");
                lexer_advance(lexer);
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_TRIPLE_EQUALS;
                return;
            } else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                // FIXME: error
                lexer_advance(lexer);
                return;
            }

        case STATE_GT_E:
            if (lexer->c == '=') {
                printf("Token is greather than equals\n");
                lexer_advance(lexer);
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_GTE;
                return;
            } else {
                printf("Token is greather than\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_GT;
                return;
            }

        case STATE_ST_E:
            if (lexer->c == '=') {
                printf("Token is smaller than equals\n");
                lexer_advance(lexer);
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_STE;
                return;
            } else {
                printf("Token is smaller than\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_ST;
                return;
            }


        case STATE_QUOTATION_CENTER_E:
            if (lexer->c == '"') { // TODO add chceck if " is not escaped
                printf("Token is string\n");
                lexer->state = STATE_START;
                lexer_advance(lexer);
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_STRING;
                Token->VAL = tok_val;
                return;
            }
            else if(lexer->c == '\\'){
                lexer->state = STATE_QUOTATION_ESCAPE_CHAR;
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            }
            break;
        
        case STATE_QUOTATION_ESCAPE_CHAR:
            lexer->state = STATE_QUOTATION_CENTER_E;
            chararray_append(value, lexer->c);
            lexer_advance(lexer);
            break;

        case STATE_SLASH:
            if (lexer->c == '/') {
                lexer->state = STATE_LINE_COMMENT_E;
                lexer_advance(lexer);
            } else if (lexer->c == '*') {
                lexer->state = STATE_BLOCK_COMMENT_START;
                lexer_advance(lexer);
            } else {
                printf("Token is division\n");
                lexer->state = STATE_START;
                token_VAL tok_val;
                tok_val.string = value;
                Token->ID = TOKEN_ID_DIVISION;
                Token->VAL = tok_val;
                return;
            }
            break;

        case STATE_LINE_COMMENT_E:
            if (lexer->c == '\n') {
                lexer->state = STATE_START;
            }
            lexer_advance(lexer);
            break;

        case STATE_BLOCK_COMMENT_START:
            if (lexer->c == '*') {
                lexer->state = STATE_BLOCK_COMMENT_E;
            }
            lexer_advance(lexer);
            break;

        case STATE_BLOCK_COMMENT_E:
            if (lexer->c == '/') {
                lexer->state = STATE_START;
            }
            else if (lexer->c == '*'){

            }
            else
            {
                lexer->state = STATE_BLOCK_COMMENT_START;
            }
            lexer_advance(lexer);
            break;
        }

    }
}
