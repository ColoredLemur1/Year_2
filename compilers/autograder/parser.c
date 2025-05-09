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
void declar();
void type(ParserInfo* pi);
void assign();
void expr();
void term();
void factor();
void loop();
void doStatement();
void ifStatement();
void returnStatement();
void parseStatementBlock();
void error(ParserInfo* pi, SyntaxErrors er, const char* msg);

// Initialize the parser
int InitParser(char* file_name) {
    TokenCount = 0;
    return InitLexer(file_name);
}

// Main parsing function
ParserInfo Parse() {
    ParserInfo pi;
    pi.er = none;
    
    // Start parsing from the program level
    prog(&pi);
    
    return pi;
}

// Clean up parser resources
int StopParser() {
    return StopLexer();
}

// Error reporting function
void error(ParserInfo* pi, SyntaxErrors er, const char* msg) {
    pi->er = er;
    if (msg) {
        strncpy(pi->msg, msg, sizeof(pi->msg) - 1);
        pi->msg[sizeof(pi->msg) - 1] = '\0';
    } else {
        pi->msg[0] = '\0';
    }
}

// Parse top-level program
// Parse top-level program
void prog(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;

    // Check for 'class' keyword to start
    if (t.tp != RESWORD || strcmp(t.lx, "class") != 0) {
        error(pi, classExpected, "Expected 'class' keyword");
        return;
    }

    // Parse class name
    t = GetNextToken();
    TokenCount++;
    if (t.tp != ID) {
        error(pi, idExpected, "Expected class name identifier");
        return;
    }

    // Check for opening brace
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
        error(pi, openBraceExpected, "Expected '{' after class name");
        return;
    }

    // Parse class members (variable declarations and subroutine declarations)
    t = PeekNextToken();
    while (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        if (t.tp == RESWORD && (strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0)) {
            declar(pi); // Parse class variable declaration
        } else if (t.tp == RESWORD && (strcmp(t.lx, "constructor") == 0 || strcmp(t.lx, "function") == 0 || strcmp(t.lx, "method") == 0)) {
            // Parse subroutine declaration (constructor, function, method)
            // For now, just consume the tokens; you'll need to implement a proper
            // subroutine parsing function later
            t = GetNextToken(); // Consume constructor/function/method
            TokenCount++;
            t = GetNextToken(); // Consume return type
            TokenCount++;
            t = GetNextToken(); // Consume subroutine name
            TokenCount++;
            // Skip parameter list and body for now
            while ((t = GetNextToken()).tp != SYMBOL || strcmp(t.lx, "}") != 0) {
                TokenCount++;
            }
            TokenCount++; // Consume closing brace
        }
        
         else {
            stmt(pi); // Parse a statement
        }
        if (pi->er != none) {
            return; // Error occurred
        }
        t = PeekNextToken();
    }

    // Check for closing brace
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        error(pi, closeBraceExpected, "Expected '}' to close class");
        return;
    }
}

// Parse a statement
void stmt(ParserInfo* pi) {
    Token t = PeekNextToken();

    if (t.tp == RESWORD) {
        if (strcmp(t.lx, "else") == 0) {
            if (InIfStatement == 0) {
                // 'else' without a preceding 'if'
                t = GetNextToken(); // Consume the token
                TokenCount++;
                error(pi, syntaxError, "Unexpected 'else' without matching 'if'");
                return;
            }
            // Don't consume 'else' here - let it be handled by ifStatement
            return;
        }
        else if (strcmp(t.lx, "var") == 0 || strcmp(t.lx, "static") == 0 || strcmp(t.lx, "field") == 0) {
            declar(pi);
        } else if (strcmp(t.lx, "while") == 0) {
            loop(pi);
        } else if (strcmp(t.lx, "do") == 0) {
            doStatement(pi);
        } else if (strcmp(t.lx, "let") == 0) {
            assign(pi);
        } else if (strcmp(t.lx, "if") == 0) {
            ifStatement(pi);
        } else if (strcmp(t.lx, "return") == 0) {
            returnStatement(pi);
        } else {
            t = GetNextToken(); // Consume the token
            TokenCount++;
            printf("Unexpected keyword: %s\n", t.lx); // Debug print
            error(pi, syntaxError, "Expected a valid statement keyword");
        }
    } else if (t.tp == ID) {
        assign(pi);
    } else {
        t = GetNextToken(); // Consume the token
        TokenCount++;
        error(pi, syntaxError, "Expected a statement");
    }
}

// Parse variable declaration
void declar(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;
    
    // Check for var/static/field keyword
    if (t.tp != RESWORD || (strcmp(t.lx, "var") != 0 && strcmp(t.lx, "static") != 0 && strcmp(t.lx, "field") != 0)) {
        error(pi, classVarErr, "Expected 'var', 'static', or 'field' keyword");
        return;
    }
    
    // Parse type
    type(pi);
    if (pi->er != none) {
        return;
    }
    
    // Parse variable name(s)
    t = GetNextToken();
    TokenCount++;
    if (t.tp != ID) {
        error(pi, idExpected, "Expected variable name");
        return;
    }
    
    // Handle multiple declarations separated by commas
    t = PeekNextToken();
    while (t.tp == SYMBOL && strcmp(t.lx, ",") == 0) {
        t = GetNextToken(); // Consume comma
        TokenCount++;
        
        t = GetNextToken();
        TokenCount++;
        if (t.tp != ID) {
            error(pi, idExpected, "Expected variable name after comma");
            return;
        }
        
        t = PeekNextToken();
    }
    
    // Check for semicolon
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        error(pi, semicolonExpected, "Expected ';' after declaration");
        return;
    }
}

// Parse type
void type(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;
    
    if ((t.tp == RESWORD && 
        (strcmp(t.lx, "int") == 0 || strcmp(t.lx, "char") == 0 || 
         strcmp(t.lx, "boolean") == 0)) || 
        t.tp == ID) {
        // Valid type - do nothing
    } else {
        error(pi, illegalType, "Expected a valid type (int, char, boolean, or class name)");
    }
}

// Parse assignment statement
void assign(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;
    
    // Variable name
    if (t.tp != ID) {
        error(pi, idExpected, "Expected variable name");
        return;
    }
    
    // Check for array indexing
    t = PeekNextToken();
    if (t.tp == SYMBOL && strcmp(t.lx, "[") == 0) {
        t = GetNextToken(); // Consume '['
        TokenCount++;
        
        // Parse array index expression
        expr(pi);
        if (pi->er != none) {
            return;
        }
        
        // Check for closing bracket
        t = GetNextToken();
        TokenCount++;
        if (t.tp != SYMBOL || strcmp(t.lx, "]") != 0) {
            error(pi, closeBracketExpected, "Expected ']' after array index");
            return;
        }
    }
    
    // Check for equals sign
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "=") != 0) {
        error(pi, equalExpected, "Expected '=' in assignment");
        return;
    }
    
    // Parse expression on right side
    expr(pi);
    if (pi->er != none) {
        return;
    }
    
    // Check for semicolon
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        error(pi, semicolonExpected, "Expected ';' after assignment");
    }
}

// Parse expression
void expr(ParserInfo* pi) {
    // Parse first term
    term(pi);
    if (pi->er != none) {
        return;
    }
    
    // Look for operators
    Token t = PeekNextToken();
    while (t.tp == SYMBOL && 
           (strcmp(t.lx, "+") == 0 || strcmp(t.lx, "-") == 0 || 
            strcmp(t.lx, "*") == 0 || strcmp(t.lx, "/") == 0 || 
            strcmp(t.lx, "&") == 0 || strcmp(t.lx, "|") == 0 || 
            strcmp(t.lx, "<") == 0 || strcmp(t.lx, ">") == 0 || 
            strcmp(t.lx, "=") == 0)) {
        
        t = GetNextToken(); // Consume operator
        TokenCount++;
        
        // Parse next term
        term(pi);
        if (pi->er != none) {
            return;
        }
        
        t = PeekNextToken();
    }
}

// Parse term
void term(ParserInfo* pi) {
    // Parse first factor
    factor(pi);
    if (pi->er != none) {
        return;
    }
    
    // Look for multiplicative operators
    Token t = PeekNextToken();
    while (t.tp == SYMBOL && 
           (strcmp(t.lx, "*") == 0 || strcmp(t.lx, "/") == 0)) {
        
        t = GetNextToken(); // Consume operator
        TokenCount++;
        
        // Parse next factor
        factor(pi);
        if (pi->er != none) {
            return;
        }
        
        t = PeekNextToken();
    }
}

void doStatement(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;
    
    if (t.tp != RESWORD || strcmp(t.lx, "do") != 0) {
        error(pi, syntaxError, "Expected 'do' keyword");
        return;
    }
    
    // Parse subroutine call
    t = GetNextToken(); // Get identifier (class or var name)
    TokenCount++;
    if (t.tp != ID) {
        error(pi, idExpected, "Expected identifier after 'do'");
        return;
    }
    
    // Check for dot operator (for method calls)
    t = PeekNextToken();
    if (t.tp == SYMBOL && strcmp(t.lx, ".") == 0) {
        t = GetNextToken(); // Consume the dot
        TokenCount++;
        
        // Get method name
        t = GetNextToken();
        TokenCount++;
        if (t.tp != ID) {
            error(pi, idExpected, "Expected method name after '.'");
            return;
        }
    }
    
    // Check for opening parenthesis
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "(") != 0) {
        error(pi, openParenExpected, "Expected '(' after subroutine name");
        return;
    }
    
    // Parse expression list (arguments)
    if (PeekNextToken().tp != SYMBOL || strcmp(PeekNextToken().lx, ")") != 0) {
        expr(pi);
        if (pi->er != none) {
            return;
        }
        
        // Parse any additional arguments
        while (PeekNextToken().tp == SYMBOL && strcmp(PeekNextToken().lx, ",") == 0) {
            t = GetNextToken(); // Consume ','
            TokenCount++;
            
            expr(pi);
            if (pi->er != none) {
                return;
            }
        }
    }
    
    // Check for closing parenthesis
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
        error(pi, closeParenExpected, "Expected ')' after function arguments");
        return;
    }
    
    // Check for semicolon
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        error(pi, semicolonExpected, "Expected ';' after do statement");
    }
}

// Parse factor
void factor(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;
    
    if (t.tp == INT) {
        // Integer constant - do nothing
    } else if (t.tp == STRING) {
        // String constant - do nothing
    } else if (t.tp == ID) {
        // Variable or function call
        Token next = PeekNextToken();
        
        if (next.tp == SYMBOL && strcmp(next.lx, "(") == 0) {
            // Function call
            t = GetNextToken(); // Consume '('
            TokenCount++;
            
            // Parse expression list (arguments)
            if (PeekNextToken().tp != SYMBOL || strcmp(PeekNextToken().lx, ")") != 0) {
                expr(pi);
                if (pi->er != none) {
                    return;
                }
                
                // Parse any additional arguments
                while (PeekNextToken().tp == SYMBOL && strcmp(PeekNextToken().lx, ",") == 0) {
                    t = GetNextToken(); // Consume ','
                    TokenCount++;
                    
                    expr(pi);
                    if (pi->er != none) {
                        return;
                    }
                }
            }
            
            // Check for closing parenthesis
            t = GetNextToken();
            TokenCount++;
            if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
                error(pi, closeParenExpected, "Expected ')' after function arguments");
                return;
            }
        } else if (next.tp == SYMBOL && strcmp(next.lx, "[") == 0) {
            // Array access
            t = GetNextToken(); // Consume '['
            TokenCount++;
            
            expr(pi);
            if (pi->er != none) {
                return;
            }
            
            t = GetNextToken();
            TokenCount++;
            if (t.tp != SYMBOL || strcmp(t.lx, "]") != 0) {
                error(pi, closeBracketExpected, "Expected ']' after array index");
                return;
            }
        }
        // Simple variable - do nothing
    } else if (t.tp == SYMBOL && strcmp(t.lx, "(") == 0) {
        // Parenthesized expression
        expr(pi);
        if (pi->er != none) {
            return;
        }
        
        t = GetNextToken();
        TokenCount++;
        if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
            error(pi, closeParenExpected, "Expected ')' after expression");
            return;
        }
    } else if (t.tp == SYMBOL && (strcmp(t.lx, "-") == 0 || strcmp(t.lx, "~") == 0)) {
        // Unary operator
        factor(pi);
        if (pi->er != none) {
            return;
        }
    } else {
        error(pi, syntaxError, "Invalid expression factor");
    }
}

// Parse loop statement
void loop(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;
    
    if (t.tp != RESWORD || strcmp(t.lx, "while") != 0) {
        error(pi, syntaxError, "Expected 'while' keyword");
        return;
    }
    
    // Parse open parenthesis
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "(") != 0) {
        error(pi, openParenExpected, "Expected '(' after 'while'");
        return;
    }
    
    // Parse condition
    expr(pi);
    if (pi->er != none) {
        return;
    }
    
    // Parse close parenthesis
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
        error(pi, closeParenExpected, "Expected ')' after condition");
        return;
    }
    
    // Parse open brace
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
        error(pi, openBraceExpected, "Expected '{' after while condition");
        return;
    }
    
    // Parse loop body statements
    t = PeekNextToken();
    while (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        stmt(pi);
        if (pi->er != none) {
            return;
        }
        t = PeekNextToken();
    }
    
    // Parse close brace
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        error(pi, closeBraceExpected, "Expected '}' to close while loop");
    }
}

void ifStatement(ParserInfo* pi) {
    InIfStatement = 1; // Set flag indicating we're in an if statement
    printf("InIfStatement before checking else: %d\n", InIfStatement);
    Token t = GetNextToken();
    TokenCount++;
    
    if (t.tp != RESWORD || strcmp(t.lx, "if") != 0) {
        error(pi, syntaxError, "Expected 'if' keyword");
        InIfStatement = 0;
        return;
    }
    
    // Parse open parenthesis
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "(") != 0) {
        error(pi, openParenExpected, "Expected '(' after 'if'");
        InIfStatement = 0;
        return;
    }
    
    // Parse condition
    expr(pi);
    if (pi->er != none) {
        InIfStatement = 0;
        return;
    }
    
    // Parse close parenthesis
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, ")") != 0) {
        error(pi, closeParenExpected, "Expected ')' after condition");
        InIfStatement = 0;
        return;
    }
    
    // Parse open brace
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
        error(pi, openBraceExpected, "Expected '{' after if condition");
        InIfStatement = 0;
        return;
    }
    
    // Parse if body statements
    t = PeekNextToken();
    while (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        printf("InIfStatement before checking else: %d\n", InIfStatement);
        stmt(pi);
        if (pi->er != none) {
            InIfStatement = 0;
            return;
        }
        t = PeekNextToken();
    }
    
    // Parse close brace for if block
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
        error(pi, closeBraceExpected, "Expected '}' to close if block");
        InIfStatement = 0;
        return;
    }
    
    // Check for optional else clause
    t = PeekNextToken();
    if (t.tp == RESWORD && strcmp(t.lx, "else") == 0) {
        // Consume the "else" token
        printf("InIfStatement before checking else: %d\n", InIfStatement);
        t = GetNextToken();
        TokenCount++;
        
        // Parse open brace for else block
        t = GetNextToken();
        TokenCount++;
        if (t.tp != SYMBOL || strcmp(t.lx, "{") != 0) {
            error(pi, openBraceExpected, "Expected '{' after 'else'");
            InIfStatement = 0;
            return;
        }
        
        // Parse else body statements
        t = PeekNextToken();
        while (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
            stmt(pi);
            if (pi->er != none) {
                InIfStatement = 0;
                return;
            }
            t = PeekNextToken();
        }
        
        // Parse close brace for else block
        t = GetNextToken();
        TokenCount++;
        if (t.tp != SYMBOL || strcmp(t.lx, "}") != 0) {
            error(pi, closeBraceExpected, "Expected '}' to close else block");
            InIfStatement = 0;
            return;
        }
    }
    
    InIfStatement = 0; // Reset flag after handling the entire if-else structure
}


void returnStatement(ParserInfo* pi) {
    Token t = GetNextToken();
    TokenCount++;
    
    if (t.tp != RESWORD || strcmp(t.lx, "return") != 0) {
        error(pi, syntaxError, "Expected 'return' keyword");
        return;
    }
    
   
    t = PeekNextToken();
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
       
        expr(pi);
        if (pi->er != none) {
            return;
        }
    }
    
    
    t = GetNextToken();
    TokenCount++;
    if (t.tp != SYMBOL || strcmp(t.lx, ";") != 0) {
        error(pi, semicolonExpected, "Expected ';' after return statement");
        return;
    }
}




// Modify your main function to include the parse tree output
#ifndef TEST_PARSER
int main() {
    char filename[100] = "Main.jack";

    if (!InitParser(filename)) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    
    ParserInfo pi = Parse();
    
    
    if (pi.er == none) {
        printf("\nParsing completed successfully.\n");
    } else {
        printf("\nError: ");
        
        switch (pi.er) {
            case lexerErr: printf("Lexical error"); break;
            case classExpected: printf("'class' expected"); break;
            case idExpected: printf("Identifier expected"); break;
            case openBraceExpected: printf("'{' expected"); break;
            case closeBraceExpected: printf("'}' expected"); break;
            case memberDeclarErr: printf("Class member declaration error"); break;
            case classVarErr: printf("Class variable declaration error"); break;
            case illegalType: printf("Illegal type"); break;
            case semicolonExpected: printf("';' expected"); break;
            case subroutineDeclarErr: printf("Subroutine declaration error"); break;
            case openParenExpected: printf("'(' expected"); break;
            case closeParenExpected: printf("')' expected"); break;
            case closeBracketExpected: printf("']' expected"); break;
            case equalExpected: printf("'=' expected"); break;
            case syntaxError: printf("Syntax error"); break;
            default: printf("Unknown error");
        }
        
        if (pi.msg[0] != '\0') {
            printf(": %s", pi.msg);
        }
        printf("\n");
        
        printf("Token count: %d\n", TokenCount);
    }
    
    StopParser();
    return pi.er == none ? 0 : 1;
}
#endif
