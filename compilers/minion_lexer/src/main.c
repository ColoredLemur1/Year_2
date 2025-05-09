#include "parser.h"
#include <stdio.h>

int main() {
    // Initialize the lexer (assuming you have an InitLexer function)
    if (!Init("bob.txt")) {
        fprintf(stderr, "Error initializing lexer\n");
        return 1;
    }

    // Call the top-level parsing function
    prog();

    // Clean up (assuming you have a StopLexer function)
    StopLexer();

    return 0;
}