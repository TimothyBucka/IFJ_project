#include "lexer.h"
#include "chararray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_T *lexer_init() {
    lexer_T *lexer = calloc(1, sizeof(struct lexer_struct));

    /*

    TODO add check

    */

    lexer->state = STATE_START;
    lexer->i = 0;
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
    else if (!strcmp(src, "function")) {
        return KW_FUNCTION;
    }
    else if (!strcmp(src, "if")) {
        return KW_IF;
    }
    else if (!strcmp(src, "int")) {
        return KW_INT;
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

    char *new_str = chararray_init(str_len + 1);
    if (new_str == NULL) {
        // TODO error
    }

    for (size_t i = 0, n_i = 0; i < str_len; i++) {
        char c = (char)0;

        if ((*str)[i] == '$') {
            printf("Unescaped $\n");
            continue; // TODO: implement
        }

        if ((*str)[i] == '\\') {
            switch ((*str)[i + 1]) {
            case 'x':
                if (i + 3 < str_len && isxdigit((*str)[i + 2]) &&
                    isxdigit((*str)[i + 3])) {
                    char hex[3] = {(*str)[i + 2], (*str)[i + 3], '\0'};
                    int num = (int)strtol(hex, NULL, 16);
                    if (num >= 0x01 && num <= 0xFF) {
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
                if (i + 3 < str_len && isoctdigit((*str)[i + 1]) && isoctdigit((*str)[i + 2]) && isoctdigit((*str)[i + 3])) {
                    char oct[4] = {(*str)[i + 1], (*str)[i + 2], (*str)[i + 3], '\0'};
                    int num = (int)strtol(oct, NULL, 8);
                    if (num >= 1 && num <= 255) {
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
    free(*str);
    *str = new_str;
}

void lexer_next_token(lexer_T *lexer, token *Token, int *ended) {
    char *value = chararray_init(0);
    unsigned int char_i; // used eg char_i=lexer->i to track number of loaded characters

    while (1) {
        //printf("State %d, character '%c'\n", lexer->state, lexer->c);
        switch (lexer->state) {

        case STATE_START:
            lexer_skip_whitespace(lexer);

            if (lexer->c == '$') {
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_START;
            }
            else if (isalpha(lexer->c) || lexer->c == '_') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_IDENTIFIER_OR_KEYWORD_E;
            }
            else if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_E;
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
                printf("Token is plus\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_PLUS;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }
            else if (lexer->c == '-') {
                printf("Token is minus\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_MINUS;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }
            else if (lexer->c == '*') {
                printf("Token is multiplication\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_MULTIPLICATION;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }
            else if (lexer->c == '/') {
                lexer->state = STATE_SLASH;
                // without lexer advance. lexer_skip_comment takes care
            }

            else if (lexer->c == '.') {
                printf("Token is concatenanion\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_CONCAT;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }
            else if (lexer->c == '"') {
                // chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_QUOTATION_CENTER_E;
                break;
            }

            else if (lexer->c == '(') {
                printf("Token is (\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_LBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == ')') {
                printf("Token is )\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_RBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }
            else if (lexer->c == '{') {
                printf("Token is {\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_LCURLYBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == '}') {
                printf("Token is }\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_RCURLYBRACKET;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == ';') {
                printf("Token is ;\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_SEMICOLLON;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == ',') {
                printf("Token is ,\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_COMMA;
                Token->VAL.string = value;
                lexer_advance(lexer);
                return;
            }

            else if (lexer->c == EOF) {
                Token->ID = TOKEN_ID_EOF;
                *ended = 1;
                return;
            }

            else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer_advance(lexer);
            }
            break;

        case STATE_VARIABLE_START:
            if (isalpha(lexer->c) || lexer->c == '_') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_VARIABLE_E;
                break;
            }
            else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer->state = STATE_START;
            }
            break;

        case STATE_VARIABLE_E:
            if (isalnum(lexer->c) || lexer->c == '_') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                printf("Token is variable\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_VARIABLE;
                Token->VAL.string = value;
                return;
            }
            break;

        case STATE_IDENTIFIER_OR_KEYWORD_E:
            if (isalnum(lexer->c) || lexer->c == '_') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                keyword kw = is_keyword(value);
                if (kw) {
                    printf("Token is keyword %s\n", value);
                    lexer->state = STATE_START;
                    Token->ID = TOKEN_ID_KEYWORD;
                    Token->VAL.keyword = kw;
                }
                else {
                    printf("Token is identifier\n");
                    lexer->state = STATE_START;
                    Token->ID = TOKEN_ID_IDENTIFIER;
                    Token->VAL.string = value;
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
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL.string = value;
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
            else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer_advance(lexer);
            }
            break;

        case STATE_DOUBLE_E:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            }
            else if (tolower(lexer->c) == 'e') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_START;
            }
            else {
                printf("Token is double\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_DOUBLE;
                Token->VAL.string = value;
                return;
            }
            break;

        case STATE_INTEGER_EXPONENT_START:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_E;
            }
            else if (lexer->c == '+' || lexer->c == '-') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_SIGN;
                break;
            }
            else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                lexer_advance(lexer);
            }
            break;

        case STATE_INTEGER_EXPONENT_SIGN:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_INTEGER_EXPONENT_E;
            }
            else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                //FIXME: error
                lexer_advance(lexer);
            }
            break;

        case STATE_INTEGER_EXPONENT_E:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                printf("Token is integer exponent\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL.string = value;
                return;
            }
            break;

        case STATE_DOUBLE_EXPONENT_START:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_E;
            }
            else if (lexer->c == '+' || lexer->c == '-') {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
                lexer->state = STATE_DOUBLE_EXPONENT_SIGN;
            }
            else {
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
            }
            else {
                printf("Lexer error in %d\n----------------------\n---------------------\n", lexer->state);
                // FIXME: error
                lexer_advance(lexer);
            }
            break;

        case STATE_DOUBLE_EXPONENT_E:
            if (isdigit(lexer->c)) {
                chararray_append(value, lexer->c);
                lexer_advance(lexer);
            }
            else {
                printf("Token is double exponent\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_INTEGER;
                Token->VAL.string = value;
                return;
            }
            break;

        case STATE_EQ_E:
            if (lexer->c == '=') {
                lexer_advance(lexer);
                lexer->state = STATE_EQEQ;
            }
            else {
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
            }
            else {
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
            }
            else {
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
            }
            else {
                printf("Token is smaller than\n");
                lexer->state = STATE_START;
                Token->ID = TOKEN_ID_ST;
                return;
            }

        case STATE_QUOTATION_CENTER_E:
            if (lexer->c == '"') {
                printf("Token is string\n");
                lexer->state = STATE_START;
                lexer_advance(lexer);
                clean_string(&value);
                Token->ID = TOKEN_ID_STRING;
                Token->VAL.string = value;
                return;
            }
            else if (lexer->c == '\\') {
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
            char_i = lexer->i;
            lexer_skip_comment(lexer);
            lexer->state = STATE_START;
            if (lexer->i - char_i == 1) { // moved just one character ie no comment skipped
                printf("Token is division\n");
                Token->ID = TOKEN_ID_DIVISION;
                Token->VAL.string = value;
                return;
            }
            break;

            // case STATE_LINE_COMMENT_E:
            //     if (lexer->c == '\n') {
            //         lexer->state = STATE_START;
            //     }
            //     lexer_advance(lexer);
            //     break;

            // case STATE_BLOCK_COMMENT_START:
            //     if (lexer->c == '*') {
            //         lexer->state = STATE_BLOCK_COMMENT_E;
            //     }
            //     lexer_advance(lexer);
            //     break;

            // case STATE_BLOCK_COMMENT_E:
            //     if (lexer->c == '/') {
            //         lexer->state = STATE_START;
            //     }
            //     else if (lexer->c == '*') {
            //     }
            //     else {
            //         lexer->state = STATE_BLOCK_COMMENT_START;
            //     }
            //     lexer_advance(lexer);
            //     break;
        }
    }
}
