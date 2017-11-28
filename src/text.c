#include "text.h"

unsigned short getGlyphId(unsigned short i)
{
    // 32 - 126
    if (i >= 32 && i <= 126)
        return i - 32;
    // 160 - 383
    else if (i >= 160 && i <= 383)
        return i - 160 + 95;
    return 65535;
}

unsigned short getGlyphCH(unsigned short i)
{
    //32 - 126
    if (i <= 94)
        return i + 32;
    //160 - 383
    else if (i >= 95 && i <= 318)
        return i + 65;
    return -1;
}

void loadGlyphs(Glyph *g, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer)
{
    unsigned short i;
    for (i = 0; i < max_glyph; i++)
    {
        SDL_Surface *surface;
        surface = TTF_RenderGlyph_Blended(font, getGlyphCH(i), color);
        g[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
        g[i].rect.x = 0;
        g[i].rect.y = 0;
        g[i].rect.w = surface->w;
        g[i].rect.h = surface->h;
        SDL_FreeSurface(surface);
    }
}

void destroyGlyphs(Glyph *glyphs)
{
    unsigned short i;
    for (i = 0; i < max_glyph; i++)
        SDL_DestroyTexture(glyphs[i].texture);
}

void text(unsigned short *uni, SDL_Renderer *renderer, Glyph *g, int x, int y, Align a, int c)
{
    SDL_Rect rect;
    int i;
    unsigned short ch;
    if (a == LEFT)
    {
        rect.x = x;
        rect.y = y;
        i = 0;
        while (uni[i] != 0 && i < c - 2)
        {
            ch = getGlyphId(uni[i]);
            rect.w = g[ch].rect.w;
            rect.h = g[ch].rect.h;
            SDL_RenderCopy(renderer, g[ch].texture, NULL, &rect);
            rect.x += g[ch].rect.w;
            i++;
        }
        if (i == c - 2)
        {
            ch = getGlyphId((unsigned short)'.');
            rect.w = g[ch].rect.w;
            rect.h = g[ch].rect.h;
            SDL_RenderCopy(renderer, g[ch].texture, NULL, &rect);
            rect.x += g[ch].rect.w;
            rect.w = g[ch].rect.w;
            rect.h = g[ch].rect.h;
            SDL_RenderCopy(renderer, g[ch].texture, NULL, &rect);
            rect.x += g[ch].rect.w;
        }
    }
    else if (a == RIGHT)
    {
        i = 0;
        while (uni[i] != 0)
            i++;
        i--;
        ch = getGlyphId(uni[i]);
        rect.x = x;
        rect.y = y;
        while (i >= 0)
        {
            ch = getGlyphId(uni[i]);
            rect.x -= g[ch].rect.w;
            rect.w = g[ch].rect.w;
            rect.h = g[ch].rect.h;
            SDL_RenderCopy(renderer, g[ch].texture, NULL, &rect);
            i--;
        }
    }
}
void texta(atomic_ushort *uni, SDL_Renderer *renderer, Glyph *g, int x, int y, Align a)
{
    SDL_Rect rect;
    int i;
    unsigned short ch;
    if (a == LEFT)
    {
        rect.x = x;
        rect.y = y;
        for (i = 0; uni[i] != 0; i++)
        {
            ch = getGlyphId(uni[i]);
            rect.w = g[ch].rect.w;
            rect.h = g[ch].rect.h;
            SDL_RenderCopy(renderer, g[ch].texture, NULL, &rect);
            rect.x += g[ch].rect.w;
        }
    }
    else if (a == RIGHT)
    {
        rect.x = x;
        rect.y = y;
        for (i = 0; uni[i] != 0; i++)
        {
            ch = getGlyphId(uni[i]);
            rect.w = g[ch].rect.w;
            rect.h = g[ch].rect.h;
            SDL_RenderCopy(renderer, g[ch].texture, NULL, &rect);
            rect.x -= g[ch].rect.w;
        }
    }
}

void background(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color *c)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, c->a);
    SDL_RenderFillRect(renderer, &rect);
}

void render_result(SDL_Renderer *renderer, Glyph *g, int x, int y, int w, int h, SDL_Color *back, Result *res, int first, int nulltime)
{
    background(renderer, x, y, w, h, back);
    unsigned short start[6] = {'s', 't', 'a', 'r', 't', 0};
    unsigned short hiba[5] = {'h', 'i', 'b', 'a', 0};
    unsigned short vk[30] = {'v', 'k', 0};
    y += 3;
    unsigned short cpos[10] = {0};
    digit_2_uni((int)(res->pos), cpos);
    if (res->pos < 990)
        text(cpos, renderer, g, x + 75, y, RIGHT, 10);
    else if (res->pos == 996)
        text(start, renderer, g, x + 75, y, RIGHT, 10);
    else if (res->pos == 997)
        text(vk, renderer, g, x + 75, y, RIGHT, 10);
    else if (res->pos == 998 || res->pos == 999)
        text(hiba, renderer, g, x + 75, y, RIGHT, 10);

    unsigned short name[namesize * 2 + 1];
    int i, d = 0;
    for (i = 0; res->surname[i] != 0; i++)
        name[d++] = res->surname[i];
    name[d++] = 32;
    for (i = 0; res->vorname[i] != 0; i++)
        name[d++] = res->vorname[i];
    name[d] = 0;
    text(name, renderer, g, x + 130, y, LEFT, namesize);
    unsigned short ctime[15] = {0};
    unsigned short ditime[15] = {0};
    int dtime = 0;
    int l = 0;
    if (res->t != 0)
    {
        dtime = res->t;
    }
    else if (res->starttime != 0)
    {
        time_t now;
        struct tm *nowinfo;
        time(&now);
        nowinfo = localtime(&now);
        int nowint = nowinfo->tm_hour * 60 * 60 + nowinfo->tm_min * 60 + nowinfo->tm_sec;
        dtime = res->starttime;
        /*if (dtime >= 0)
            ctime[l++] = '+';
        else
            ctime[l++] = '-';*/
    }
    int dmin = dtime / 60;
    int dsec = dtime % 60;
    int k = 5;
    if (dtime > 0)
    {
        while (k >= 0)
        {
            if (dmin > pow_dec(k))
            {
                ctime[l++] = ((dmin % (10 * (pow_dec(k)))) / pow_dec(k)) + '0';
            }
            k--;
        }
        ctime[l++] = ':';
    }
    if (dsec >= 10)
        ctime[l++] = dsec / 10 + '0';
    else if (dsec < 10 && dtime > 0)
        ctime[l++] = '0';
    if (dtime > 0 || dsec > 0)
        ctime[l++] = dsec % 10 + '0';
    text((unsigned short *)res->club, renderer, g, x + 450, y, LEFT, 18);
    if (res->t == 0 && res->starttime != 0)
        text(ctime, renderer, g, x + 920, y, RIGHT, 10);
    else
        text(ctime, renderer, g, x + 800, y, RIGHT, 10);
    if (res->t > first && res->t != 0)
    {
        dtime = res->t - first;
        dmin = dtime / 60;
        dsec = dtime % 60;
        k = 5;
        l = 0;
        ditime[l++] = '+';
        if (dmin > 0)
        {
            while (k >= 0)
            {
                if (dmin >= pow_dec(k))
                {
                    ditime[l++] = ((dmin % (10 * (pow_dec(k)))) / pow_dec(k)) + '0';
                }
                k--;
            }
            ditime[l++] = ':';
        }
        if (dmin == 0)
        {
            ditime[l++] = '0';
            ditime[l++] = ':';
        }
        if (dsec >= 10)
            ditime[l++] = dsec / 10 + '0';
        else if (dsec < 10 && dtime > 0)
            ditime[l++] = '0';
        ditime[l++] = dsec % 10 + '0';
        text(ditime, renderer, g, x + 920, y, RIGHT, 10);
    }
    if (res->starttime != 0 && res->t == 0 && res->pos == 996)
    {
        time_t now = time(0);
        struct tm *localnow;
        localnow = localtime(&now);
        now = localnow->tm_hour * 60 * 60 + localnow->tm_min * 60 + localnow->tm_sec;
        int dif = now - (res->starttime + nulltime);
        l = 0;
        k = 5;
        unsigned short sdif[15] = {0};
        if (dif < 0)
        {
            sdif[l++] = '-';
            dif = dif * -1;
        }
        dmin = dif / 60;
        dsec = dif % 60;

        if (dmin > 0)
        {
            while (k >= 0)
            {
                if (dmin >= pow_dec(k))
                {
                    sdif[l++] = ((dmin % (10 * (pow_dec(k)))) / pow_dec(k)) + '0';
                }
                k--;
            }
            sdif[l++] = ':';
        }
        if (dmin == 0)
        {
            sdif[l++] = '0';
            sdif[l++] = ':';
        }
        if (dsec >= 10)
            sdif[l++] = dsec / 10 + '0';
        else if (dsec < 10)
            sdif[l++] = '0';
        sdif[l++] = dsec % 10 + '0';
        text(sdif, renderer, g, x + 800, y, RIGHT, 10);
    }
}

void render_category(SDL_Renderer *renderer, Glyph *g, int x, int y, int *size, int w, Category *c, int nulltime)
{
    if (c != NULL)
    {
        SDL_Color c_result1 = {255, 255, 255, 0};
        SDL_Color c_result2 = {255, 227, 163, 0};
        SDL_Color c_top = {145, 187, 255, 0};
        int yinc = 0;
        Result *list;
        unsigned short cname[namesize];
        ascii_2_unicode(c->name, cname);
        if (y + *size >= 0)
        {
            background(renderer, x, y + *size, w, 40, &c_top);
            text(cname, renderer, g, x + 30, y + 3 + *size, LEFT, namesize);
        }
        yinc += 40;
        int loopbackermax = (int)c->all, loopbacker = 0;
        bool duo = false;
        for (list = c->res; list != NULL; list = list->next)
        {
            if (loopbacker++ > loopbackermax)
                break;
            if (list->surname[0] == 'V' && list->surname[1] == 'a' && list->surname[2] == 'k' && list->surname[3] == 'a' && list->surname[4] == 'n' && list->surname[5] == 't' && list->surname[6] == 0)
                continue;
            if (duo)
            {
                duo = false;
                render_result(renderer, g, x, y + yinc + *size, w, 40, &c_result1, list, (int)c->res->t, nulltime);
            }
            else
            {
                duo = true;
                render_result(renderer, g, x, y + yinc + *size, w, 40, &c_result2, list, (int)c->res->t, nulltime);
            }

            yinc += 40;
        }
        if (y + *size < 0)
        {
            background(renderer, x, 0, w, 40, &c_top);
            text(cname, renderer, g, x + 30, 0, LEFT, namesize);
        }
        *size += yinc + 40;
    }
}
