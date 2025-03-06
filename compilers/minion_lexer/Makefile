CC = gcc
CFLAGS = -Wall -g
SRC = src/Lexer.c src/main.c
OBJ = $(SRC:.c=.o)
EXEC = lexer

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(EXEC)