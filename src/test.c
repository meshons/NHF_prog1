#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main(){
    FILE * fp;
    int i;
    while(1){
        fp = fopen("result.txt","wt");
        if(fp==NULL) continue;
        if(i>100)i=0;
        fprintf(fp,"%d",i++);
        fclose(fp);
        Sleep(200);
    }
    return 0;
}