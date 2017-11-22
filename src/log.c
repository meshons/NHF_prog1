#include "log.h"

bool LOG_init(){
    loggerfile=fopen("log.txt","a+t");
    if(loggerfile!=NULL)
        return true;
    else
        return false;
}
void LOG_str(char * str){
    time_t timer;
    char buffer[26];
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    //if(str[strlen(str)-1] == '\n')str[strlen(str)-1] = '\0';
    if(str[strlen(str)-1] == '\n')
    fprintf(loggerfile,"%s: %s",buffer,str);
    else    
    fprintf(loggerfile,"%s: %s\n",buffer,str);
}
void LOG_cstr(const char * str){
    char * str2 = malloc(strlen(str)+1);
    int i;
    for(i=0;str[i]!=0;i++)str2[i]=str[i];
    str2[i]=0;
    LOG_str(str2);
    free(str2);
}

void LOG_close(){
    fclose(loggerfile);
}