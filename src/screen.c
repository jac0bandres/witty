#include <unistd.h>
#include "screen.h"
#include <stdbool.h>

Cursor cursor = {0, 0};

Screen *init_screen(int cols, int rows) {
    Screen *screen = malloc(sizeof(Screen));
    screen->grid = malloc(rows * sizeof(Cell *));
    screen->rows = rows;
    screen->cols = cols;

    for (int r = 0; r < rows; r++) {
        screen->grid[r] = calloc(cols, sizeof(Cell));
        for (int c = 0; c < cols; c++) {
            screen->grid[r][c].codepoint = ' ';
            screen->grid[r][c].fg = (SDL_Color){255, 255, 255, 255};
            screen->grid[r][c].bg = (SDL_Color){0, 0, 0, 255};
            screen->grid[r][c].width = 1;
            screen->grid[r][c].dirty = true;
            screen->grid[r][c].has_glyph = false;
        }
    }

    return screen;
}

void destroy_screen(Screen *screen) {
    if (!screen) return;

    for (int r = 0; r < screen->rows; r++) {
        free(screen->grid[r]);
        screen->grid[r] = NULL;
    }

    free(screen->grid);
    screen->grid = NULL;
    free(screen);
    screen = NULL;
}

void put_char(char ch, int r, int c, Screen *screen) {
    screen->grid[r][c].codepoint = ch;
}

void clear_screen(Screen *screen) {
    for (int r = 0; r < screen->rows; r++) {
        for (int c = 0; c < screen->cols; c++) {
            screen->grid[r][c].codepoint = ' ';
        }
    }
}

void render_screen(Screen *screen) {
    for (int r = 0; r < screen->rows; r++) {
        for (int c = 0; c < screen->cols; c++) {
            write(STDOUT_FILENO, &screen->grid[r][c].codepoint, 1);
        }
        write(STDOUT_FILENO, "\n", 1);
    }
}

void move_cursor(int r, int c, Screen *screen) {
    cursor.row = r;
    cursor.col = c;
    put_char('|', r, c, screen);
}

void put_char_at_cursor(char ch, Screen *screen) {
    screen->grid[cursor.row][cursor.col].codepoint = ch;
    cursor.col++;
    if (cursor.col >= screen->cols) {
        cursor.col = 0;
        cursor.row++;
        if (cursor.row >= screen->rows) cursor.row = 0;
    }
}
