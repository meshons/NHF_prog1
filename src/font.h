#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef GLYPHH
#define GLYPHH 1
#define max_glyph 319
typedef struct Glyph{
    SDL_Texture * texture;
    SDL_Rect rect;
}Glyph;
typedef enum Align { LEFT, RIGHT } Align;
#endif

unsigned short getGlyphId(int i);
void loadGlyphs(Glyph *g,TTF_Font *font,SDL_Color color,SDL_Renderer *renderer);
void destroyGlyphs(Glyph *glyphs);

