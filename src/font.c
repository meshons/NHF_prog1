#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "font.h"

unsigned short getGlyphId(int i){
    // 32 - 126
    if(i>=32 && i<=126)
        return i-32;
    // 160 - 383
    else if(i>=160 && i<=383)
        return i-160+95;
    return 65535;
}

int getGlyphCH(unsigned short i){
    //32 - 126
    if(i<=94)   
        return i+32;
    //160 - 383
    else if(i>=95 && i<=318)
        return i+65;
    return -1;
}

void loadGlyphs(Glyph *g,TTF_Font *font,SDL_Color color,SDL_Renderer *renderer){
    unsigned short i;
    for(i=0;i<max_glyph;i++){
        SDL_Surface *surface;
        surface = TTF_RenderGlyph_Blended(font,getGlyphCH(i),color);
        g[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
        g[i].rect.x = 0;
        g[i].rect.y = 0;
        g[i].rect.w = surface->w;
        g[i].rect.h = surface->h;
        SDL_FreeSurface(surface);        
    }
}   

void destroyGlyphs(Glyph *glyphs){
    unsigned short i;
    for(i=0;i<max_glyph;i++)
        SDL_DestroyTexture(glyphs[i].texture);
}

void text(unsigned short * uni,SDL_Renderer *renderer,Glyph * g,int x,int y, Align a){
    SDL_Rect rect;
    int i;
    unsigned short ch;
    if(a == LEFT){
        rect.x = x;
        rect.y = y;
        for(i=0;uni[i] != 0;i++){
            ch = getGlyphId(uni[i]);
            rect.w = g[ch].rect.w;
            rect.h = g[ch].rect.h;
            SDL_RenderCopy(renderer,g[ch].texture,NULL,&rect);
            rect.x += g[ch].rect.w;
        }
    }
}
