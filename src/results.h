#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdatomic.h>

#include "charset.h"
#include "log.h"

#ifndef resultsh
#define resultsh 1

#define namesize 50

extern bool error;

typedef enum R_format {txt,xml,csv,undefined_format} R_format; 
typedef enum R_charset {ascii,utf8,uni,win1252,win1250,latin2,undefined_charset} R_charset;
typedef enum R_list {result,start} R_list;

typedef struct Time{
    int h,m,s;
}Time;

typedef struct Result{
    atomic_short pos,prev;
    atomic_ushort vorname[namesize],  surname[namesize];
    atomic_ushort club[namesize];
    atomic_char category[namesize];
    atomic_int t;
    atomic_bool shown,poschanged;
    atomic_int starttime;
    struct Result *next;
}Result;

typedef struct Category{
    char name[namesize];
    atomic_int in,all;
    Result * res;
    struct Category * next;
}Category;

void R_result_add(Category * list,Result *r,R_list listtype);
Result * R_result_new(Category * list,R_list listtype);
void R_result_delete(Category * list, Result * del);
Result * R_result_search(Category * list,Result * rel, char level);

void R_result_sort(Category * list);
Result * R_new();// kell?

//void R_result_sort(Category * list);

Category * R_category_new(Category ** list);
Category * R_category_find(Category * list,char * name);
Category * R_category_find_a(Category * list,atomic_char * name);
void R_category_deleteresults(Category * list);
void R_category_delete(Category ** list,Category * del);
void R_category_deleteall(Category *list);

char * R_charset_ascii(R_charset charset);
R_charset R_ascii_charset(char * str);    
char * R_format_ascii(R_format format);    
R_format R_ascii_format(char * str);    


char * R_load_cats(char * filename,int * size);
void R_clear_cats(char * cats,int size);
/*
void R_startlist_load(Category * list,char * filename,R_format format,R_charset charset);*/

int pow_dec(int n);
#endif
