//#define TEST_PARSER  // uncomment to run selfgrader for the parser

#include "lexer.h"

// syntax errors which my be encountered by the parser
typedef enum {
	none,					// no errors
	lexerErr,				// lexer error (if the lexer returns an error)
	classExpected,			// keyword class expected
	idExpected,				// identifier expected
	openBraceExpected,		// { expected
	closeBraceExpected,		// } expected
	memberDeclarErr,		// class member declaration must begin with static, field, constructor , function , or method
	classVarErr,			// class variables must begin with field or static
	illegalType,			// a type must be int, char, boolean, or identifier
	semicolonExpected,		// ; expected
	subroutineDeclarErr,	// subrouting declaration must begin with constructor, function, or method
	openParenExpected,		// ( expected
	closeParenExpected,		// ) expected
	closeBracketExpected,	// ] expected
	equalExpected,			// = expected
	syntaxError				// any other kind of syntax error
} SyntaxErrors;

// every parsing function should return this struct
// the struct contains an error type field (er)
// and a token field (tk) that is set to the token at or near which the error was encountered
// if no errors are encountered er should be set to none
typedef struct {
    SyntaxErrors er;    // Error type from the enumeration above
    int tk;             // Number of tokens parsed before the error occurred
    int ln;             // Line number where the error occurred 
    char fl[32];        // File where the error occurred
    char msg[128];      // Optional error message
} ParserInfo;

int InitParser (char* file_name);  // initialise the parser to parse source code in file_name
ParserInfo Parse (); // parse the input file (the one passed to InitParser)
int StopParser (); // stop the parser and do any necessary clean up
