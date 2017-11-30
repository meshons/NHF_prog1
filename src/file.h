#ifndef fileh
#define fileh 1

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "charset.h"
#include "results.h"
#include "log.h"

extern bool error;

struct thread_arg{
    char filename[namesize];
    R_list type;
    Category ** list;
    int refreshrate;
    R_format format;
    R_charset charset;
};

void * readerthread(void *arg);


R_charset TXT_getcharset(FILE *f);
Result * TXT_line(char * line,R_charset charset, R_list listtype);
void TXT_read(FILE * f,Category ** list, R_list listtype);

Result * IOF_read(FILE *f,R_charset charset, R_list listtype);

Result * CSV_line(char * line,R_charset charset,R_list listtype);
void CSV_read(FILE *f, Category **list,R_list listtype, R_charset charset);

#endif