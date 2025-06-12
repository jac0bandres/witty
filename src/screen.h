#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t codepoint;
    uint8_t width;
    SDL_Color fg, bg;
    bool dirty;
    bool has_glyph;
} Cell;

typedef struct {
    int cols, rows;
    Cell **grid;
} Screen;

typedef struct {
    int row, col;
} Cursor;

#define ROWS 24
#define COLS 80

extern Cell screen[ROWS][COLS];
extern Cursor cursor;

Screen *init_screen(int cols, int rows);
void clear_screen(Screen *screen);
void put_char(char ch, int r, int c, Screen *screen);
void render_screen(Screen *screen);
void destroy_screen(Screen *screen);
void move_cursor(int r, int c, Screen *screen);
void put_char_at_cursor(char c, Screen *screen);
