#ifndef OPENGL_CELL_H
#define OPENGL_CELL_H

#include <stdint.h>

typedef struct {
    uint16_t grid_row, grid_col;
    uint32_t glyph_row, glyph_col;
    uint32_t glyph_width, glyph_height;
    int32_t glyph_offset_x, glyph_offset_y;
    uint8_t r, g, b, a; // Colors
    uint8_t bg_r, bg_g, bg_b, bg_a; // Background colors
    uint8_t flags; // Bit flags for cell properties
    uint8_t grid_width, grid_height; // Grid dimensions
} Cell;

#endif
