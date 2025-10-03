#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

// Global variables
int TokenCount = 0;
int InIfStatement = 0; // Flag to indicate if we are in an if statement

// Forward declarations for recursive descent parser
void prog();
void stmt();
void emit();
void parseClassVariableDeclaration(ParserInfo* pi);
void parseSubroutine(ParserInfo* pi);
void parseParameterList(ParserInfo* pi);
void parseSubroutineBody(ParserInfo* pi);
void parseVarDeclaration(ParserInfo* pi);
void parseLetStatement(ParserInfo* pi);
void parseIfStatement(ParserInfo* pi);
void parseWhileStatement(ParserInfo* pi);
void parseDoStatement(ParserInfo* pi);
void parseReturnStatement(ParserInfo* pi);
void parseExpressionList(ParserInfo* pi);
void expr(ParserInfo* pi);
void error(ParserInfo* pi);
void factor(ParserInfo* pi);
void term(ParserInfo* pi);


const char* ErrorMessage(SyntaxErrors error) {
    switch (error) {
        case none: return "No error";
        case lexerErr: return "Lexer error";
        case classExpected: return "Expected 'class' keyword";
        case idExpected: return "Expected identifier";
        case openBraceExpected: return "Expected '{'";
        case closeBraceExpected: return "Expected '}'";
        case openParenExpected: return "Expected '('";
        case closeParenExpected: return "Expected ')'";
        case semicolonExpected: return "Expected ';'";
        case illegalType: return "Illegal type";
        case equalExpected: return "Expected '='";
        case syntaxError: return "Syntax error";
        case closeBracketExpected: return "Expected ']'";
        default: return "Unknown error";
    }
}


// Initialize the parser
int InitParser(char* file_name) {
    TokenCount = 0;
    return InitLexer(file_name);
}

ParserInfo Parse() {
    ParserInfo pi;
    pi.er = none;
    
    // Initialize token with default values
    pi.tk.tp = ERR;
    pi.tk.ln = 0;
    pi.tk.lx[0] = '\0';
    pi.tk.fl[0] = '\0';
    pi.tk.ec = NoLexErr;
    
    // Modify error() to not exit but store the error info
    prog(&pi);
    
    return pi;
}

void error(ParserInfo* pi) {
    int line = pi->ln;
    SyntaxErrors error = pi->er;
    const char* msg = ErrorMessage(error);
    printf("Error at line %d, %s\n", line, msg);
    exit(1);
}

void emit(const char* code) {
    printf("%s", code);
}

void prog(ParserInfo* pi) {
    Token t = GetNextToken();
    
    if (t.tp != RESWORD || strcmp(t.lx, "class") != 0) {
        pi->er = classExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != ID) {
        pi->er = idExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
        pi->er = openBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    while (EOFile != t.tp && strcmp(t.lx, "}") != 0) {
        if (t.tp == RESWORD && (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0)) {
            parseClassVariableDeclaration(pi);
        } else if (t.tp == RESWORD && (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0)) {
            parseSubroutine(pi);
        } else {
            stmt(pi); 
        }
        t = PeekNextToken();
    }

}

void parseClassVariableDeclaration(ParserInfo* pi) {
    Token t;
    t = GetNextToken(); // check is done at prog
    t = GetNextToken();
    if (!((t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)) || t.tp == ID)) {
        pi->er = illegalType;
        pi->ln = t.ln;
        error(pi);
        return;
    }

    t = GetNextToken();
    if (t.tp != ID) {
        return;
    }

    // Parse (',' varName)*
    t = PeekNextToken();
    while (t.tp == SYMBOL && strcmp(t.lx, ",") == 0) {
        GetNextToken(); 

        t = GetNextToken(); 
        if (t.tp != ID) {
  
            return;
        }
        t = PeekNextToken();
    }

    
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        return;
    }
}

void parseSubroutine(ParserInfo* pi) {
    Token t;
    t = GetNextToken(); // check is done at prog
    t = GetNextToken();
    if (t.tp == RESWORD && strcmp(t.lx, "void") == 0){

    }else if (!((t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)) || t.tp == ID)) {
        pi->er = illegalType;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != ID) {
        pi->er = idExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "(") != 0) {
        pi->er = openParenExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }

    parseParameterList(pi); 
    if (pi->er != none) {
        return; 
    }

    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
        pi->er = closeParenExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }

    parseSubroutineBody(pi); 
}

void parseParameterList(ParserInfo* pi){
    Token t = PeekNextToken();

    if (t.tp == SYMBOL && strcmp(t.lx, ")") == 0) {
        return; // Empty parameter list
    }

    t = GetNextToken();
    if (!((t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)) || t.tp == ID)) {
        pi->er = illegalType;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != ID) {
        pi->er = idExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    while (t.tp == SYMBOL && strcmp(t.lx, ",") == 0) {
        GetNextToken(); // consume ','

        t = GetNextToken(); // get the next token
        if (t.tp != ID) {
            pi->er = idExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
        t = GetNextToken(); // get the next token
        if (t.tp != ID){
            pi->er = idExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
        t = PeekNextToken();
    }
}

void parseSubroutineBody(ParserInfo* pi){
    Token t;

    t = GetNextToken(); // check is done at prog
    if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
        pi->er = openBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    while(t.tp == RESWORD && strcmp(t.lx, "var") == 0){
        parseVarDeclaration(pi);
        if (pi->er != none) {
            return; 
        }
        t = PeekNextToken();
    }

    t = PeekNextToken();
    while (!(t.tp == SYMBOL && strcmp(t.lx, "}") == 0)) {
        stmt(pi);
        if (pi->er != none) {
            return; 
        } 
        t = PeekNextToken();
    }

    if (t.tp == EOFile && pi->er == none) {
        pi->er = closeBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        pi->er = closeBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
}

void parseVarDeclaration(ParserInfo* pi){
    Token t;

    t = GetNextToken(); // check is done at prog
    t = GetNextToken();
    if (!((t.tp == RESWORD && (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || strcmp(t.lx, "boolean") == 0)) || t.tp == ID)) {
        pi->er = illegalType;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != ID) {
        pi->er = idExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    while (t.tp == SYMBOL && strcmp(t.lx, ",") == 0) {
        GetNextToken(); // consume ','

        t = GetNextToken(); // get the next token
        if (t.tp != ID) {
            pi->er = idExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
        t = PeekNextToken();
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        pi->er = semicolonExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
}

void stmt(ParserInfo* pi){
    Token t = PeekNextToken();
    if (t.tp == EOFile){
        return;
    }

    if (t.tp == RESWORD){
        if (strcmp(t.lx, "let") == 0) {
            parseLetStatement(pi);
        } else if (strcmp(t.lx, "if") == 0) {
            parseIfStatement(pi);
        } else if (strcmp(t.lx, "while") == 0) {
            parseWhileStatement(pi);
        } else if (strcmp(t.lx, "do") == 0) {
            parseDoStatement(pi);
        } else if (strcmp(t.lx, "return") == 0) {
            parseReturnStatement(pi);
        } else {
            pi->er = syntaxError;
            pi->ln = t.ln;
            error(pi);
        }
    } else {
        pi->er = syntaxError;
        pi->ln = t.ln;
        error(pi);
    }
}

void parseLetStatement(ParserInfo* pi){
    Token t;

    t = GetNextToken(); // check is done at prog
    t = GetNextToken();
    if (t.tp != ID) {
        pi->er = idExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    if (t.tp == SYMBOL && strcmp(t.lx, "[") == 0) {
        GetNextToken(); // consume '['
        expr(pi); // parse expression inside brackets
        if (pi->er != none) {
            return; 
        }
        t = GetNextToken();
        if (t.tp != SYMBOL || strcmp(t.lx, "]") != 0) {
            pi->er = closeBracketExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "=") != 0) {
        pi->er = equalExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    expr(pi); // parse expression after '='
    if (pi->er != none) {
        return; 
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        pi->er = semicolonExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
}

void parseIfStatement(ParserInfo* pi){
    Token t;

    t = GetNextToken(); // check is done at prog
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "(") != 0) {
        pi->er = openParenExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    expr(pi); // parse expression inside brackets
    if (pi->er != none) {
        return; 
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
        pi->er = closeParenExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
        pi->er = openBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    while (t.tp != EOFile && strcmp(t.lx, "}") != 0) {
        stmt(pi); 
        if (pi->er != none) {
            return; 
        } 
        t = PeekNextToken();
    }
    if (t.tp == EOFile && pi->er == none) {
        pi->er = closeBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        pi->er = closeBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    if (t.tp == RESWORD && strcmp(t.lx, "else") == 0) {
        GetNextToken(); // consume 'else'
        t = GetNextToken();
        if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
            pi->er = openBraceExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
        t = PeekNextToken();
        while (t.tp != EOFile && strcmp(t.lx, "}") != 0) {
            stmt(pi); 
            if (pi->er != none) {
                return; 
            } 
            t = PeekNextToken();
        }
        if (t.tp == EOFile && pi->er == none) {
            pi->er = closeBraceExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
        t = GetNextToken();
        if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
            pi->er = closeBraceExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
    }
}

void parseWhileStatement(ParserInfo* pi){
    Token t;

    t = GetNextToken(); // check is done at prog
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "(") != 0) {
        pi->er = openParenExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    expr(pi); // parse expression inside brackets
    if (pi->er != none) {
        return; 
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
        pi->er = closeParenExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
        pi->er = openBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = PeekNextToken();
    while (t.tp != EOFile && strcmp(t.lx, "}") != 0) {
        stmt(pi); 
        if (pi->er != none) {
            return; 
        } 
        t = PeekNextToken();
    }
    if (t.tp == EOFile && pi->er == none) {
        pi->er = closeBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        pi->er = closeBraceExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
}

void parseDoStatement(ParserInfo* pi){
    Token t;
    
    t = GetNextToken(); // check is done at prog
    t = GetNextToken();
    if (t.tp != ID) {
        pi->er = idExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
    Token peeked = PeekNextToken();
    if (peeked.tp == SYMBOL && strcmp(peeked.lx, "(") == 0) {
        GetNextToken(); // consume '('
        if (PeekNextToken().tp != SYMBOL || strcmp(PeekNextToken().lx, ")") != 0) {
            expr(pi); // parse expression inside brackets
            if (pi->er != none) {
                return; 
            }
            while (PeekNextToken().tp == SYMBOL && strcmp(PeekNextToken().lx, ",") == 0) {
                GetNextToken(); // consume ','
                expr(pi); // parse expression inside brackets
                if (pi->er != none) {
                    return; 
                }
            }
        }
        t = GetNextToken();
        if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
            pi->er = closeParenExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
    } else if (peeked.tp == SYMBOL && strcmp(peeked.lx, ".") == 0) {
        GetNextToken(); // consume '.'
        t = GetNextToken();
        if (t.tp != ID) {
            pi->er = idExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
        t = GetNextToken();
        if (t.tp != SYMBOL || strcmp(t.lx, "(") != 0) {
            pi->er = openParenExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
        if (PeekNextToken().tp != SYMBOL || strcmp(PeekNextToken().lx, ")") != 0) {
            expr(pi); // parse expression inside brackets
            if (pi->er != none) {
                return; 
            }
            while (PeekNextToken().tp == SYMBOL && strcmp(PeekNextToken().lx, ",") == 0) {
                GetNextToken(); // consume ','
                expr(pi); // parse expression inside brackets
                if (pi->er != none) {
                    return; 
                }
            }
        }
        t = GetNextToken();
        if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
            pi->er = closeParenExpected;
            pi->ln = t.ln;
            error(pi);
            return;
        }
    } else {
        pi->er = syntaxError;
        pi->ln = t.ln;
        error(pi);
    }

    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        pi->er = semicolonExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
}

void parseReturnStatement(ParserInfo* pi){
    Token t;

    t = GetNextToken(); // check is done at prog
    if (PeekNextToken().tp != SYMBOL || strcmp(PeekNextToken().lx, ";") != 0) {
        expr(pi); // parse expression after 'return'
        if (pi->er != none) {
            return; 
        }
    }
    t = GetNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        pi->er = semicolonExpected;
        pi->ln = t.ln;
        error(pi);
        return;
    }
}

void expr(ParserInfo* pi){
    term(pi);
    if (pi->er != none) {
        return; 
    }
    Token t = PeekNextToken();
    while (t.tp == SYMBOL &&
        (strcmp(t.lx, "+") == 0 || strcmp(t.lx, "-") == 0 ||
         strcmp(t.lx, "&") == 0 || strcmp(t.lx, "|") == 0 ||
         strcmp(t.lx, "<") == 0 || strcmp(t.lx, ">") == 0 ||
         strcmp(t.lx, "=") == 0)) {
     GetNextToken(); // Consume the operator
     term(pi);
     if (pi->er != none) return;
     t = PeekNextToken();
    }
}

void term(ParserInfo* pi){
    factor(pi);
    if (pi->er != none) {
        return; 
    }
    Token t = PeekNextToken();
    while (t.tp == SYMBOL && (strcmp(t.lx, "*") == 0 || strcmp(t.lx, "/") == 0)) {
        GetNextToken(); // consume the operator
        factor(pi); // parse the next term
        if (pi->er != none) {
            return; 
        }
        t = PeekNextToken();
    }
}

void factor(ParserInfo* pi){
    Token t = PeekNextToken();

    if (t.tp == SYMBOL && (strcmp(t.lx, "-")==0) || strcmp(t.lx, "~")==0) {
        GetNextToken();
        factor(pi); // parse the next factor
        return;
    }
    t = GetNextToken();
    if (t.tp == INT){

    } else if (t.tp == STRING){

    } else if (t.tp == RESWORD &&
        (strcmp(t.lx, "true") == 0 || strcmp(t.lx, "false") == 0 ||
         strcmp(t.lx, "null") == 0 || strcmp(t.lx, "this") == 0)) {
    } else if (t.tp == ID){
        Token peeked = PeekNextToken();
        if (peeked.tp == SYMBOL && strcmp(peeked.lx, "[") == 0) { // varName'['expression']'
            GetNextToken(); // Consume '['
            expr(pi);
            if (pi->er != none) return;
            Token t_close_bracket = GetNextToken();
            if (t_close_bracket.tp != SYMBOL || strcmp(t_close_bracket.lx, "]") != 0) {
                pi->er = closeBracketExpected;
                pi->ln = t_close_bracket.ln;
                error(pi);
                return;
            }
        } else if (peeked.tp == SYMBOL && strcmp(peeked.lx, "(") == 0) { // subroutineName'('expressionList')'
            GetNextToken(); // Consume '('
            parseExpressionList(pi);
            if (pi->er != none) return;
            Token t_close_paren = GetNextToken();
            if (t_close_paren.tp != SYMBOL || strcmp(t_close_paren.lx, ")") != 0) {
                pi->er = closeParenExpected;
                pi->ln = t_close_paren.ln;
                error(pi);
                return;
            }
        } else if (peeked.tp == SYMBOL && strcmp(peeked.lx, ".") == 0) { // (className|varName)'.'subroutineName'('expressionList')'
            GetNextToken(); // Consume '.'
            Token t_sub_name = GetNextToken();
            if (t_sub_name.tp != ID) {
                pi ->er = idExpected;
                pi->ln = t_sub_name.ln;
                error(pi);
                return;
            }
            Token t_open_paren = GetNextToken();
            if (t_open_paren.tp != SYMBOL || strcmp(t_open_paren.lx, "(") != 0) {
                pi->er = openParenExpected;
                pi->ln = t_open_paren.ln;
                error(pi);
                return;
            }
            parseExpressionList(pi);
            if (pi->er != none) return;
            Token t_close_paren = GetNextToken();
            if (t_close_paren.tp != SYMBOL || strcmp(t_close_paren.lx, ")") != 0) {
                pi->er = closeParenExpected;
                pi->ln = t_close_paren.ln;
                error(pi);
                return;
            }
        } 
    }
}

void parseExpressionList(ParserInfo* pi){
    Token t = PeekNextToken();
    if (t.tp == SYMBOL && strcmp(t.lx, ")") == 0) {
        return; // Empty expression list
    }
    expr(pi); // parse expression inside brackets
    if (pi->er != none) {
        return; 
    }
    t = PeekNextToken();
    while (t.tp == SYMBOL && strcmp(t.lx, ",") == 0) {
        GetNextToken(); // consume ','
        expr(pi); // parse expression inside brackets
        if (pi->er != none) {
            return; 
        }
        t = PeekNextToken();
    }
}

int StopParser() {
   return StopLexer();
}
