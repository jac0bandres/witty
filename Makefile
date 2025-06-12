CC = gcc
CFLAGS = -Wall -O2
SRC = src/main.c src/pty.c src/term.c
BIN = witty

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)
