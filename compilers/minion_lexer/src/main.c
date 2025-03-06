#include <stdio.h>
#include "lexer.h"

int main() {
    if (!Init()) {
        fprintf(stderr, "Failed to initialize lexer.\n");
        return 1;
    }

    Token token;
    while ((token = GetToken()).t != eof) {
        switch (token.t) {
            case key:
                printf("Keyword: %s (Line: %d)\n", token.x, token.ln);
                break;
            case id:
                printf("Identifier: %s (Line: %d)\n", token.x, token.ln);
                break;
            case num:
                printf("Number: %s (Line: %d)\n", token.x, token.ln);
                break;
            default:
                printf("Symbol: %s (Line: %d)\n", token.x, token.ln);
                break;
        }
    }

    fclose(f);
    return 0;
}