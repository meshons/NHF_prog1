#include <stdio.h>
#include <string.h>

#include "results.h"

#ifndef settingsh
#define settingsh 1
typedef struct Setting{
    R_charset charset;
    R_format format;
    char res_file[30];
    char start_file[30];
    char left_file[30];
    char right_file[30];
    int leftspeed;
    int rightspeed;
    int res_refreshrate;
    int start_refreshrate;
    int nulltime;
}Setting;

extern bool error;

void Settings_default(Setting *s);
void Settings_save(Setting * s);
void Settings_load(Setting * s);

#endif