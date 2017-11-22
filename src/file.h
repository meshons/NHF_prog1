#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdatomic.h>
#include <pthread.h>
#include <errno.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


#include "charset.h"
#include "results.h"
#include "log.h"

#ifndef fileh
#define fileh

extern bool error;
extern unsigned char loglevel;

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

/* IOF */
Result * IOF_block(char * line,R_charset charset, R_list listtype);

/* csv */
Result * CSV_line(char * line,R_charset charset,R_list listtype);
void CSV_read(FILE *f, Category **list,R_list listtype, R_charset charset);

#endif