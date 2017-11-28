#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include <math.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "text.h"
#include "charset.h"
#include "results.h"
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
    FILE *resultfile = fopen(set.res_file, "rt");
    if (resultfile == NULL)
    {
        perror("fopen");
        LOG_str("result");
        LOG_close();
        return 1;
    }
    //TXT_read_result(resultfile,&list);

    int quit;
    int width, height;

    int f;
    atomic_ushort teszti[20]; // = malloc(sizeof(atomic_ushort)*20);
    for (f = 0; f < 20; f++)
        teszti[f] = 0;

    pthread_t result_thread;
    pthread_t start_thread;

    struct thread_arg starter;
    struct thread_arg resulter;
    bool starter_en = false;
    bool resulter_en = false;

    char *left_cats;
    int left_size;
    char *right_cats;
    int right_size;
    left_cats = R_load_cats(set.left_file, &left_size);
    right_cats = R_load_cats(set.right_file, &right_size);
/*
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
    }*/

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
    SDL_Color c_white = {255, 255, 255, 0};

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
    //int FPS = set.speed;
    long FrameStartTimeMs = 0;

    int x_top = 0, x_bot = height;
    bool boot = true;

    clock_t timestart = clock();
    int i = 0;
    int j = 0;
    float fi = height / 2;
    float fr = height / 2;
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
            /*case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        quit = 1;
                        break;*/
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    fi = 0;
                    break;
                case SDLK_RIGHT:
                    fi = 100;
                    break;
                case SDLK_UP:
                    stop=false;
                    break;
                case SDLK_DOWN:
                    stop=true;
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

        SDL_SetRenderDrawColor(renderer, 130, 130, 130, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, width / 2, 0, width / 2, height);
        SDL_RenderDrawLine(renderer, width / 2 - 1, 0, width / 2 - 1, height);

        left_k = 0;
        right_k = 0;
        int first_left = 0;
        int first_right = 0;
        /*for (l_c = 0; l_c < left_size; l_c++)
        {
            Category *cl = R_category_find(list, left_cats+namesize*l_c);
           
         render_category(renderer, ttf_normal, 0, i, &left_k, width / 2 - 1, cl);
        }*/
        int afas=0;
        while (left_k <= height + first_left && afas++<100)
        {
            if (left_cat >= left_size)
                left_cat = 0;
            Category *cl = R_category_find(list, left_cats + namesize * left_cat);
            //if (cl != NULL){
            render_category(renderer, ttf_normal, 0, i, &left_k, width / 2 - 1, cl,set.nulltime);
            if (first_left == 0)
            {
                if (fi < (float)~left_k)
                {
                    fi += left_k;
                    next_left = left_cat + 1;
                }
                first_left = left_k;
            }/*}else
            if(first_left==0){
                first_left==1;
            }else{
                if(left_cat == 0)left_k=height*2;
            }*/
            left_cat++;
        }
        afas=0;
        while (right_k <= height + first_right && afas++<100)
        {
            if (right_cat >= right_size)
                right_cat = 0;
            Category *cr = R_category_find(list, right_cats + namesize * right_cat);
            //if (cr != NULL){
            render_category(renderer, ttf_normal, width / 2 + 1, j, &right_k, width / 2 - 1, cr, set.nulltime);
            if (first_right == 0)
            {
                if (fr < (float)~right_k)
                {
                    fr += right_k;
                    next_right = right_cat + 1;
                }
                first_right = right_k;
            }/*}else
            if(first_right==0){
                first_right==1;
            }else{
                if(right_cat == 0)right_k=height*2;
            }*/
            right_cat++;
        }
        //render_category(renderer, ttf_normal, width / 2 + 1, j, &right_k, width / 2 - 1, c->next);

        /*render_category(renderer, ttf_normal, 0, i,&left_k, width / 2 - 1, list);
        render_category(renderer, ttf_normal, 1081, i,&right_k, width / 2 - 1, list->next);
        render_category(renderer, ttf_normal, 0, i,&left_k, width / 2 - 1, list->next->next);
*/
        //texta(teszti, renderer, ttf_normal, 10, i, LEFT);
        /*texta(list->res->surname,renderer,ttf_normal,10,i+100,LEFT);    
        texta(list->res->vorname,renderer,ttf_normal,110,i+100,LEFT);        
        texta(list->res->next->surname,renderer,ttf_normal,10,i+200,LEFT);    
        texta(list->res->next->vorname,renderer,ttf_normal,110,i+200,LEFT);*/
        SDL_RenderPresent(renderer);
        //i++;

        /*if (x_top == 0 && boot == true)
        {
            Sleep(10000);
            boot = false;
        }*/
        x_top++;
        x_bot++;

        deltaTime = (elapsedTime - lastFrameTimeElapsed) / 1000.0f;
        if(!stop){
        fi -= 100.0f * deltaTime;
        fr -= 100.0f * deltaTime;
        }
        /*if (fi < -height)
            fi = height;
        if (fr < -height)
            fr = height;*/
        lastFrameTimeElapsed = elapsedTime;
        //valami

        /*FrameStartTimeMs = FPS - (SDL_GetTicks() - FrameStartTimeMs);
        if (FrameStartTimeMs > 0)
        {
            #ifdef _WIN32
            Sleep(FrameStartTimeMs);
            #else
            sleep(FrameStartTimeMs);
            #endif
        }*/
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
    /*R_clear_cats(left_cats,left_size);
    R_clear_cats(right_cats,right_size);*/

    R_category_deleteall(list);
    Settings_save(&set);
    LOG_close();

    return EXIT_SUCCESS;
}
