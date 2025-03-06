# c-lexer-project

This project implements a simple lexer in C. The lexer is designed to read a source file, tokenize its contents, and identify keywords, identifiers, and numbers.

## Project Structure

- **src/**: Contains the source code files.
  - **Lexer.c**: Implementation of the lexer, including functions for initialization, token building, and token retrieval.
  - **lexer.h**: Header file defining the structures and constants used in the lexer.
  - **main.c**: Entry point of the program that initializes the lexer and processes tokens.

- **Makefile**: Build instructions for compiling the source files and linking them into an executable.

## Building the Project

To build the project, navigate to the project directory and run the following command:

```
make
```

This will compile the source files and create an executable.

## Running the Program

After building the project, you can run the lexer with the following command:

```
./lexer
```

Make sure to have an input file named `bob.txt` in the same directory as the executable, as the lexer reads from this file.

## Lexer Functionality

The lexer performs the following tasks:

- Initializes the lexer and opens the input file.
- Consumes whitespace and comments.
- Builds tokens for keywords, identifiers, and numbers.
- Retrieves tokens and processes them accordingly.

## License

This project is licensed under the MIT License.