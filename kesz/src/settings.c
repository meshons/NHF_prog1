#include "settings.h"

void Settings_default(Setting *s){
    s->charset=win1252;
    s->format=csv;
    strcpy(s->res_file,"oe.csv");
    strcpy(s->start_file,"oerajt.csv");
    strcpy(s->left_file,"left.txt");
    strcpy(s->right_file,"right.txt");
    s->res_refreshrate = 10;
    s->start_refreshrate = 600;
    s->leftspeed = 60;
    s->rightspeed = 60;
    s->nulltime = 43200; // 12:00:00
}
void Settings_save(Setting * s){
    FILE *fp = fopen("settings.txt","wt");
    fprintf(fp,"charset=%s\n\
format=%s\n\
resultfile=%s\n\
startfile=%s\n\
leftfile=%s\n\
rightfile=%s\n\
res_refreshrate=%d\n\
start_refreshrate=%d\n\
leftspeed=%d\n\
rightspeed=%d\n\
nulltime=%d\n",
    R_charset_ascii(s->charset),
    R_format_ascii(s->format),
    s->res_file,
    s->start_file,
    s->left_file,
    s->right_file,
    s->res_refreshrate,
    s->start_refreshrate,
    s->leftspeed,
    s->rightspeed,
    s->nulltime);
    fclose(fp);
}
void Settings_load(Setting * s){
    Settings_default(s);
    FILE * fp = fopen("settings.txt","rt");
    if(fp==NULL)return;
    char type[30];
    char value[30];
    while(fscanf(fp,"%[^=]%*c%[^\n]%*c",type,value)==2){
        if(strcmp(type,"charset") == 0)
            s->charset = R_ascii_charset(value);
        if(strcmp(type, "format") == 0)
            s->format = R_ascii_format(value);
        if(strcmp(type, "resultfile") == 0)
            strcpy(s->res_file,value);
        if(strcmp(type, "startfile") == 0)
            strcpy(s->start_file,value);
        if(strcmp(type, "leftfile") == 0)
            strcpy(s->left_file,value);
        if(strcmp(type, "rightfile") == 0)
            strcpy(s->right_file,value);
        if(strcmp(type, "res_refreshrate") == 0)
            s->res_refreshrate = atoi(value);
        if(strcmp(type, "start_refreshrate") == 0)
            s->start_refreshrate = atoi(value);
        if(strcmp(type, "leftspeed") == 0)
            s->leftspeed = atoi(value);
        if(strcmp(type, "rightspeed") == 0)
            s->rightspeed = atoi(value);
        if(strcmp(type, "nulltime") == 0)
            s->nulltime = atoi(value);
    }
    fclose(fp);
}

