#include "settings.h"

void Settings_default(Setting *s){
    s->charset=win1252;
    s->format=txt;
    strcpy(s->res_file,"result.txt");
    strcpy(s->start_file,"start.txt");
    s->speed=60;
    s->loglevel = 0;
    s->res_refreshrate = 10;
    s->start_refreshrate = 0;
}
void Settings_save(Setting * s){
    FILE *fp = fopen("settings.txt","wt");
    fprintf(fp,"charset=%s\nformat=%s\nresultfile=%s\nstartfile=%s\nspeed=%d\nloglevel=%d\nres_refreshrate=%d\nstart_refreshrate=%d\n",R_charset_ascii(s->charset),R_format_ascii(s->format),s->res_file, s->start_file,s->speed, s->loglevel, s->res_refreshrate, s->start_refreshrate);
    fclose(fp);
}
void Settings_load(Setting * s){
    Settings_default(s);
    FILE * fp = fopen("settings.txt","rt");
    if(fp==NULL)return;
    char type[20];
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
        if(strcmp(type, "speed") == 0)
            s->speed = atoi(value);
        if(strcmp(type, "loglevel") == 0)
            s->loglevel = atoi(value);
        if(strcmp(type, "res_refreshrate") == 0)
            s->res_refreshrate = atoi(value);
        if(strcmp(type, "start_refreshrate") == 0)
            s->start_refreshrate = atoi(value);
    }
    fclose(fp);
}

