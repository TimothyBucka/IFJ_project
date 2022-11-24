
#include "expressions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "DLL.h"

#define next_tok;       if(dll->activeElement == dll->lastElement)\
                        { ERROR = lexer_next_token(lexer, token);\
                        DLL_push(dll, token);\
                            if(ERROR != SUCCESS){\
                                return false;\
                            }\
                        }\
                        else{DLL_move_active_right(dll);token = DLL_get_active(dll);}



bool accept(token *token, token_ID acceptedID) {
    if (acceptedID == token->ID) {
        return true;
    }
    return false;
}

bool expect(token *token, token_ID acceptedID) {
    if (acceptedID == token->ID) {
        return true;
    }
    printf("Unexpected token parsed in syntax analisys\n");
    printf("Expected %d", acceptedID);
    printf("got %d\n",token->ID);
    return false;
}

bool parse_arguments(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));

    next_tok;

    //case Function
    if (accept(token, TOKEN_ID_VARIABLE)) {     //  function

    }

    else {
        return true;
    }
    return true;
}

bool parse_type(lexer_T *lexer, DLL *dll) {
    return true;
}

bool parse_local_scope(lexer_T *lexer, DLL *dll) {
    return true;
}

bool parse_assignment_prime (lexer_T *lexer, DLL *dll ){ 
    token *token = calloc(1, sizeof(token));


    //TODO add FUNCALL case

    return parse_expresion(lexer, dll, false);
}

bool parse_assignment (lexer_T *lexer, DLL *dll) { 
    token *token = calloc(1, sizeof(token));


    next_tok;
    if(!expect(token, TOKEN_ID_VARIABLE)){return false;}
    next_tok;
    if(!expect(token, TOKEN_ID_EQUALS)){return false;}
    if(!parse_assignment_prime(lexer, dll)){return false;}

    

    return true;
}



bool parse_body(lexer_T *lexer, DLL *dll) {
    token *token = calloc(1, sizeof(token));



    next_tok;

    //case Function
    if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_FUNCTION) {     //  function
        next_tok;
        if (!expect(token, TOKEN_ID_IDENTIFIER)){return false;}                     //  func_id
        next_tok;
        if (!expect(token, TOKEN_ID_LBRACKET)){return false;}                       //  (
        if (!parse_arguments(lexer, dll)){return false;}                         //  parameters
        next_tok;
        if (!expect(token, TOKEN_ID_RBRACKET)){return false;}                       //  )
        next_tok;
        if (!expect(token, TOKEN_ID_COLON)){return false;}                          //  :
        if(!parse_type(lexer, dll)){return false;}                                //  type    
        next_tok;
        if(!expect(token, TOKEN_ID_LCURLYBRACKET)){return false;}                   //  {
        if(!parse_body(lexer, dll)){return false;}                                //  body
        next_tok;
        if(!expect(token, TOKEN_ID_RCURLYBRACKET)){return false;}                   //  }
        parse_body(lexer, dll);                                                   //  body
    }

    //case If
    else if(accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_IF){        //  if
        next_tok;
        if(!expect(token, TOKEN_ID_LBRACKET)){return false;}                        //  (
        if(!parse_expresion(lexer, dll, true)){return false;}                           //  expresion
        next_tok;
        if(!expect(token, TOKEN_ID_RBRACKET)){return false;}                        //  )
        next_tok;
        if(!expect(token, TOKEN_ID_LCURLYBRACKET)){return false;}                   //  {
        if(!parse_body(lexer, dll)){return false;}                                //  body
        next_tok;
        if(!expect(token, TOKEN_ID_RCURLYBRACKET)){return false;}                   //  }
        next_tok;
        if(!accept(token, TOKEN_ID_KEYWORD) || token->VAL.keyword != KW_ELSE){return false;}    //  else
        next_tok;
        if(!expect(token, TOKEN_ID_LCURLYBRACKET)){return false;}                   //  {
        if(!parse_body(lexer, dll)){return false;}                                //  body
        next_tok;
        if(!expect(token, TOKEN_ID_RCURLYBRACKET)){return false;}                   //  }
        if(!parse_body(lexer, dll)){return false;}                                //  body
    }
    
    //case While
    else if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_WHILE) {   //  while
        next_tok;
        if(!expect(token, TOKEN_ID_LBRACKET)){return false;}                        //  (
        if(!parse_expresion(lexer, dll, true)){return false;}
        next_tok;
        if(!expect(token, TOKEN_ID_RBRACKET)){return false;}
        next_tok;
        if(!expect(token, TOKEN_ID_LCURLYBRACKET)){return false;}
        if(!parse_local_scope(lexer, dll)){return false;}
        next_tok;
        if(!expect(token, TOKEN_ID_RCURLYBRACKET)){return false;}
        if(!parse_body(lexer, dll)){return false;}
    }

    //case Return
    else if (accept(token, TOKEN_ID_KEYWORD) && token->VAL.keyword == KW_RETURN)
    {
        //TODO add return
        next_tok;
        if(expect(token, TOKEN_ID_SEMICOLLON)){
            parse_body(lexer, dll);
        }
    }

    //TODO case Function call    
    else if (accept(token, TOKEN_ID_IDENTIFIER))
    {
        //TODO add return
        next_tok;
        if(!expect(token, TOKEN_ID_LBRACKET)){return false;}
        if(!parse_arguments(lexer, dll)){return false;}
        next_tok;
        if(!expect(token, TOKEN_ID_RBRACKET)){return false;}
    }

    
    else {
        DLL_move_active_left(dll);
        //case assignment
        if (parse_assignment(lexer, dll)){
        
        }
        else{
            DLL_move_active_left(dll);
            return true;
        }
    }

    return true;

}

int main() {

    lexer_T *lexer = lexer_init();
    
    DLL *dll = createDLL();
    if (parse_body(lexer, dll)){
        printf("PARSED\n");
    }
    else{
        printf("FAILED TO PARSE\n");
        printf("error : %d", ERROR);
    }

    return ERROR;
}