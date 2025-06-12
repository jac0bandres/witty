CC = gcc
CFLAGS = -Wall -O2 `sdl2-config --cflags` -lSDL2_ttf -lSDL2 `pkg-config --cflags --libs fontconfig`
LDLIBS = `sdl2-config --libs` -lSDL2 -lSDL2_tff
SRC = src/main.c src/pty.c src/term.c src/screen.c src/render.c
BIN = witty 

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)

