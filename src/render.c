#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <fontconfig/fontconfig.h>
#include "screen.h"

#define FONT_SIZE 16

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;

void init_renderer(int *rows, int *cols) {
    SDL_SetHint("SDL_VIDEO_X11_WMCLASS", "witty");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    };
    TTF_Init();

    window = SDL_CreateWindow("witty", 
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              800, 600,
                              SDL_WINDOW_SHOWN);


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    FcInit();
    FcPattern *pattern = FcNameParse((const FcChar8 *)"monospace");
    FcConfigSubstitute(NULL, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcResult result;
    FcPattern *match = FcFontMatch(NULL, pattern, &result);
    FcChar8 *file = NULL;

    if (match) {
        if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch) {
            SDL_Log("Loading font from: %s", file);
            font = TTF_OpenFont((const char *) file, FONT_SIZE);
            if (!font) {
                SDL_Log("font is null");
                exit(0);
            }
        }
        FcPatternDestroy(match);
    }
    FcPatternDestroy(pattern);
    FcFini(); 

    int win_w, win_h;
    SDL_GetWindowSize(window, &win_w, &win_h);
    int char_w, char_h;
    TTF_SizeText(font, "M", &char_w, &char_h);

    *cols = win_w / char_w;
    *rows = win_h / char_h;
}

void render_frame(Screen *screen) {
    if (!renderer) {
        SDL_Log("renderer is null");
        exit(1);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char ch = screen->grid[r][c].codepoint;
            if (ch == ' ') continue;
            
            char text[2] = {ch, '\0'};
            if (!text || strlen(text) == 0) {
                SDL_Log("Text is null");
                exit(0);
            }
            SDL_Color fg = {255, 255, 255, 255};
            SDL_Surface *surf = TTF_RenderText_Solid(font, text, fg);
            if (!surf) {
                SDL_Log("surface is null");
                exit(1);
            }
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (!tex) {
                SDL_Log("texture is null");
                exit(1);
            }
            SDL_Rect dst = {c * FONT_SIZE, r * FONT_SIZE, surf->w, surf->w};
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }
    }

    SDL_RenderPresent(renderer);
}

void destroy_renderer() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
