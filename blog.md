# witty: Terminal Emulator

I wanted to create my own terminal emulator. Ghostty and kitty are great, but ghostty doesn't have a good scripting interface without using yaml files and kitty has Python. I'd make my own from C and expose functionality for plugins.

## Rendering
My first experience with C was when I was 19 building a cellular automata. I used SDL2, which after fighting through dependency hell on my old Ubuntu machine, was able to get it up and running rendering a grid of cells following von Neumann's game of life rules. Ghostty uses GFLW to interact with OpenGL and provide that GPU accelerated goodness, but I wanted a rich feature system and SDL2 provides that. I wouldn't implement most of these features myself, like sounds and drawing, but expose these from witty for plugin creation.

### Cells
Every cell will have a glyph and a set of properties to determine appearance and behavior. Throw it into a struct and we got our first terminal element:

```#ifndef OPENGL_CELL_H
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
```
