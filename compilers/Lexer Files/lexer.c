/************************************************************************
University of Leeds
School of Computing
COMP2932- Compiler Design and Construction
Lexer Module

I confirm that the following code has been developed and written by me and it is entirely the result of my own work.
I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program from me.
I confirm that I will not publish the program online or share it with anyone without permission of the module leader.

Student Name: Javier Duarte Macias
Student ID: 201753404
Email: sc23jdm@leeds.ac.uk
Date Work Commenced:
*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

int LineCount;
int TokenReady;
FILE* f;
Token NextToken;
char FileName[32] = "";
const char Symbols[] = "+-*/=<>();,.{}[]";


int EatWC(){
  int c;
    c = getc(f);
    while (c != EOF && isspace(c)) {
        if ((char) c == '\n') LineCount++;
        c = getc(f);
    }

    if (c == '/') {
        c = getc(f);
        if (c == '/') {
            // Single line comment
            while (c != EOF && c != '\n') {
                c = getc(f);
            }
            if (c == '\n') {
                LineCount++;
                c = getc(f);
            }
            if (c == EOF) {
                return c;
            }
            if (isspace(c) || c == '/') {
                ungetc(c, f);
                return EatWC();
            }
        } else if (c == '*') {
            // Multi-line comment
            int prevChar = 0;
            while (c != EOF && !(prevChar == '*' && c == '/')) {
                prevChar = c;
                c = getc(f);
                if (c == '\n') LineCount++;
            }
            if (c == EOF) {
                // End of file in comment
                NextToken.tp = ERR;
                NextToken.ec = EofInCom;
                strcpy(NextToken.lx, "Error: End of file in comment");
                TokenReady = 1;
                return EOF;
            }
            c = getc(f);
            if (c == EOF) {
                return c;
            }
            if (isspace(c) || c == '/') {
                ungetc(c, f);
                return EatWC();
            }
        } else {
            ungetc(c, f);
            c = '/';
        }
    }
    return c;
}

//returns 1 if reserved word, 0 if not
int isReservedWord(const char* str) {
  const char* reserved[] = {
      "class", "constructor", "function", "method", "field", "static", "var",
      "int", "char", "boolean", "void", "true", "false", "null", "this",
      "let", "do", "if", "else", "while", "return", NULL
  };
  for (int i = 0; reserved[i] != NULL; i++) {
      if (strcmp(str, reserved[i]) == 0) {
          return 1;
      }
  }
  return 0;
}

// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
  f = fopen(file_name, "r");
  if (f == NULL){
    return 0;
  }
  strncpy(FileName, file_name, sizeof(FileName) - 1);
  FileName[sizeof(FileName) - 1] = '\0';          
  TokenReady = 0;
  LineCount = 1;
  return 1;
}


// Get the next token from the source file
Token GetNextToken ()
{
    Token t;
    t.tp = ERR;
    strncpy(t.fl, FileName, sizeof(t.fl) - 1);
    t.fl[sizeof(t.fl) - 1] = '\0';

    // Check if a token is already ready (peeked)
    if (TokenReady) {
        t = NextToken;       // Return the peeked token
        TokenReady = 0;      // Clear the flag
        return t;
    }
    
    if (f == NULL){
        t.tp = EOFile; // Or ERR, depending on desired behavior
        return t;
    }

    //remove white space
    int c = EatWC();

    if (c == EOF){
        t.tp = EOFile;
        t.ln = LineCount;
        return t;
    }

    t.ln = LineCount;

    //Numbers
    if (c >= '0' && c <= '9'){
        t.tp = INT;
        t.lx[0] = c;
        int i = 1;
        while ((c = fgetc(f)) != EOF && c >= '0' && c <= '9') {
            if (i < sizeof(t.lx) - 1) t.lx[i++] = c;
        }
        t.lx[i] = '\0';
        if (c != EOF) ungetc(c, f);
    }
    //Word or reserved word
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'){
        t.lx[0] = c;
        int i = 1;
        while ((c = fgetc(f)) != EOF && 
                ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                (c >= '0' && c <= '9') || c == '_')) {
            if (i < sizeof(t.lx) - 1) t.lx[i++] = c;
        }
        t.lx[i] = '\0';
        if (c != EOF) ungetc(c, f);
        int isReserved = isReservedWord(t.lx);
        if (isReserved) {
            t.tp = RESWORD;
        } else {
            t.tp = ID;
        }
    }
    // string
    else if(c == '"'){
        t.tp = STRING;
        int i = 0;
        while ((c = fgetc(f)) != EOF && c != '"') {
            if (c == '\n') {
                t.tp = ERR;
                t.ec = NewLnInStr;
                strcpy(t.lx, "Error: New line in string literal");
                LineCount++;
                return t;
            }
            if (i < sizeof(t.lx) - 1) t.lx[i++] = c;
        }
        t.lx[i] = '\0';
        if (c != '"') {  
            t.tp = ERR;
            t.ec = EofInStr;
            strcpy(t.lx, "Error: End of file in string literal");
        }
    }
    //symbols
    else if (strchr(Symbols, c) != NULL){
        t.tp = SYMBOL;
        t.lx[0] = c;
        t.lx[1] = '\0';
    }else{
        t.tp = ERR;
        t.ec = IllSym;
        char errorMsg[128];
        snprintf(errorMsg, sizeof(errorMsg), "Error: Illegal symbol '%c'", c);
        strcpy(t.lx, errorMsg);
    }
    return t;
}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  if (!TokenReady){
    NextToken = GetNextToken();
    TokenReady = 1;
  }
  return NextToken;
}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
	if (f != NULL) {
    fclose(f);
    f = NULL;
  }
  TokenReady = 0;  
  LineCount = 1;   
  return 0;
}

// do not remove the next line
#ifndef TEST
int main ()
{
    // implement your main function here
    // NOTE: the autograder will not use your main function
    const char* TokenTypeNames[] = {
        "RESWORD", "ID", "INT", "SYMBOL", "STRING", "EOFile", "ERR"
    };
    
    const char* ErrorTypeNames[] = {
        "EofInCom", "NewLnInStr", "EofInStr", "IllSym"
    };

    if (!InitLexer("Main.jack")) {
        fprintf(stderr, "Failed to initialize lexer.\n");
        return 1;
    }
    while (1) {
        Token t = GetNextToken();
        if (t.tp == EOFile){
            printf("< %s, %d, End of File, %s >\n", 
                t.fl, t.ln, TokenTypeNames[t.tp]);
            break;
        } else if (t.tp == ERR) {
            printf("< %s, %d, %s, %s, %s >\n", 
                t.fl, t.ln, t.lx, TokenTypeNames[t.tp], ErrorTypeNames[t.ec]);
        } else {
            printf("< %s, %d, %s, %s >\n", 
                t.fl, t.ln, t.lx, TokenTypeNames[t.tp]);
        }
    }
    StopLexer();
    return 0;
}
#endif
