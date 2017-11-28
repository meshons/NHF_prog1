#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdatomic.h>

#include "results.h"
#include "charset.h"

#ifndef texth
#define texth 1
#define max_glyph 319
typedef struct Glyph{
    SDL_Texture * texture;
    SDL_Rect rect;
}Glyph;
typedef enum Align { LEFT, CENTER, RIGHT } Align;

typedef struct Action{

    struct Action *next;
}Action;

unsigned short getGlyphId(unsigned short i);
unsigned short getGlyphCH(unsigned short i);
void loadGlyphs(Glyph *g,TTF_Font *font,SDL_Color color,SDL_Renderer *renderer);
void destroyGlyphs(Glyph *glyphs);
void text(unsigned short * uni,SDL_Renderer *renderer,Glyph * g,int x,int y, Align a,int c);
void texta(atomic_ushort * uni,SDL_Renderer *renderer,Glyph * g,int x,int y, Align a);
void background(SDL_Renderer *renderer,int x, int y, int w, int h, SDL_Color * c);

void render_result(SDL_Renderer *renderer,Glyph * g,int x,int y,int w, int h, SDL_Color * back,Result *res, int first, int nulltime);   
void render_category(SDL_Renderer *renderer,Glyph * g,int x,int y,int *size,int w,Category *c, int nulltime);
#endif
