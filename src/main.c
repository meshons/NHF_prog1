#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include <math.h>

#include "text.h"
#include "log.h"
#include "settings.h"
#include "file.h"

bool error = false;

int main(int argc, char **argv)
{
    if (!LOG_init())
        return 1;

    Setting set;
    Settings_load(&set);

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    Category *list = NULL;

    int quit = 0;
    int width, height;

    char *left_cats;
    int left_size;
    char *right_cats;
    int right_size;
    left_cats = R_load_cats(set.left_file, &left_size);
    right_cats = R_load_cats(set.right_file, &right_size);

    pthread_t result_thread;
    pthread_t start_thread;
    struct thread_arg starter;
    struct thread_arg resulter;
    bool starter_en = false;
    bool resulter_en = false;

    if (set.res_refreshrate != 0)
    {
        strcpy(resulter.filename, set.res_file);
        resulter.type = result;
        resulter.list = &list;
        resulter.format = set.format;
        resulter.refreshrate = set.res_refreshrate;
        resulter.charset = set.charset;
        resulter_en = true;
        pthread_create(&result_thread, NULL, readerthread, &resulter);
    }
    else
    {
        FILE *resfile = fopen(set.res_file, "rt");
        if (resfile != NULL)
        {
            if (set.format == txt)
                TXT_read(resfile, &list, result);
            else if (set.format == csv)
                CSV_read(resfile, &list, result, set.charset);
            fclose(resfile);
        }
    }

    if (set.start_refreshrate != 0)
    {
        strcpy(starter.filename, set.start_file);
        starter.type = start;
        starter.list = &list;
        starter.format = set.format;
        starter.refreshrate = set.start_refreshrate;
        starter.charset = set.charset;
        starter_en = true;
        pthread_create(&start_thread, NULL, readerthread, &starter);
    }
    else
    {
        FILE *startfile = fopen(set.start_file, "rt");
        if (startfile != NULL)
        {
            if (set.format == txt)
                TXT_read(startfile, &list, start);
            else if (set.format == csv)
                CSV_read(startfile, &list, start, set.charset);
            fclose(startfile);
        }
    }

    Glyph ttf_normal[max_glyph];
    SDL_Color c_black = {0, 0, 0, 0};

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
        LOG_cstr(SDL_GetError());
    window = SDL_CreateWindow("NHF", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (window == NULL)
        LOG_str("ablak");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        LOG_str("renderer");
    SDL_GetWindowSize(window, &width, &height);
    TTF_Init();
    SDL_DisableScreenSaver();
    SDL_ShowCursor(SDL_DISABLE);

    TTF_Font *font = TTF_OpenFont("Open_Sans/OpenSans-Regular.ttf", width / 74);

    loadGlyphs(ttf_normal, font, c_black, renderer);

    quit = 0;

    clock_t timestart = clock();
    int i = 0;
    int j = 0;
    float fi = height / 4;
    float fr = height / 4;
    int left_k = 0, right_k = 0;

    Uint32 elapsedTime = 0;
    Uint32 lastFrameTimeElapsed = 0;
    float deltaTime;

    int left_cat = 0;
    int next_left = 0;
    int right_cat = 0;
    int next_right = 0;

    bool stop=false;

    while (!quit)
    {
        left_cat = next_left;
        right_cat = next_right;
        elapsedTime = SDL_GetTicks();
        i = roundf(fi);
        j = roundf(fr);
        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    fi = 0;
                    break;
                case SDLK_RIGHT:
                    fi = 100;
                    break;
                case SDLK_d:
                case SDLK_DOWN:
                    if(stop)stop=false;
                    else stop = true;
                    break;
                case SDLK_q:
                    set.leftspeed+=5;
                    if(set.leftspeed>200)set.leftspeed=200;
                    break;
                case SDLK_a:
                    set.leftspeed-=5;
                    if(set.leftspeed<0)set.leftspeed=0;
                    break;
                case SDLK_w:
                    set.rightspeed+=5;
                    if(set.rightspeed>200)set.rightspeed=200;
                    break;
                case SDLK_s:
                    set.rightspeed-=5;
                    if(set.rightspeed<0)set.rightspeed=0;
                    break;
                case SDLK_r:
                    left_cat = 0;
                    next_left = 0;
                    right_cat = 0;
                    next_right = 0;
                    fi = height / 4;
                    fr = height / 4;
                    break;
                case SDLK_e:
                    set.leftspeed = 90;
                    set.rightspeed = 90;
                    break;
                }
                break;
            case SDL_QUIT:
                quit = 1;
                break;

            default:
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
        SDL_RenderDrawLine(renderer, width / 2, 0, width / 2, height);
        SDL_RenderDrawLine(renderer, width / 2 - 1, 0, width / 2 - 1, height);

        left_k = 0;
        right_k = 0;
        int first_left = 0;
        int first_right = 0;

        int afas=0;
        while (left_k <= height + first_left && afas++<100)
        {
            if (left_cat >= left_size)
                left_cat = 0;
            Category *cl = R_category_find(list, left_cats + namesize * left_cat);
            render_category(renderer, ttf_normal, 0, i, &left_k, width / 2 - 1, cl,set.nulltime);
            if (first_left == 0)
            {
                if (fi < (float)~left_k)
                {
                    fi += left_k;
                    next_left = left_cat + 1;
                }
                first_left = left_k;
            }
            left_cat++;
        }
        afas=0;
        while (right_k <= height + first_right && afas++<100)
        {
            if (right_cat >= right_size)
                right_cat = 0;
            Category *cr = R_category_find(list, right_cats + namesize * right_cat);
            render_category(renderer, ttf_normal, width / 2 + 1, j, &right_k, width / 2 - 1, cr, set.nulltime);
            if (first_right == 0)
            {
                if (fr < (float)~right_k)
                {
                    fr += right_k;
                    next_right = right_cat + 1;
                }
                first_right = right_k;
            }
            right_cat++;
        }

        SDL_RenderPresent(renderer);

        deltaTime = (elapsedTime - lastFrameTimeElapsed) / 1000.0f;
        if(!stop){
            fi -= (float)set.leftspeed * deltaTime;
            fr -= (float)set.rightspeed * deltaTime;
        }
        lastFrameTimeElapsed = elapsedTime;

    }

    destroyGlyphs(ttf_normal);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    if (starter_en)
    {
        pthread_cancel(start_thread);
    }
    if (resulter_en)
    {
        pthread_cancel(result_thread);
    }

    free(left_cats);
    free(right_cats);

    R_category_deleteall(list);
    Settings_save(&set);
    LOG_close();

    return EXIT_SUCCESS;
}
