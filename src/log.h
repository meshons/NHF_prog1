#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef logh
#define logh 1

FILE * loggerfile;
bool LOG_init();
void LOG_str(char * str);
void LOG_cstr(const char * str);
void LOG_close();

#endif