#ifndef LEXER_H
#define LEXER_H

#define NumberKeywords 12

typedef enum {
    eof,
    key,
    id,
    num,
    sym
} TokenType;

typedef struct {
    TokenType t;
    char x[128];
    int ln;
} Token;

int Init();
int IsKeyWord(char* str);
Token BuildToken();
Token GetToken();
Token PeekToken();

extern FILE* f;
extern int TokenReady;
extern Token t;
extern int LineCount;

#endif // LEXER_H