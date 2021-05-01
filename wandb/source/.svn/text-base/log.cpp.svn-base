#include "../header/log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

Log::Log(){
    FILE* fp;
    if((fp = fopen("wanData/logs.txt", "rt"))==NULL){
        logAmount = 0;
        fp = fopen("wanData/logs.txt", "wt");
        fprintf(fp, "         0");
        fclose(fp);
    }else{
        char countArray[10];
        fread(countArray, 10, 1, fp);
        logAmount = atoi(countArray);
        fclose(fp);
    }
}

Log::~Log(){

}

bool Log::logCommand(char *command, int t){
    if(t == 0)
        t = time(0);

    FILE* fp;
    char timeArray[10], countArray[10];
    sprintf(timeArray, "%d", t);

    if((fp = fopen("wanData/logs.txt", "rt+"))==NULL){
        fp = fopen("wanData/logs.txt", "wt");
        fprintf(fp, "         1");
        fwrite(timeArray, 10, 1, fp);
        fprintf(fp, "%s\n", command);
        fclose(fp);
        return true;
    }

    sprintf(countArray, "%d", logAmount+1);
    fwrite(countArray, 10, 1, fp);
    fseek(fp, 0, SEEK_END);
    fwrite(timeArray, 10, 1, fp);
    fwrite(command, 50, 1, fp);
    logAmount++;
    fclose(fp);
    return true;
}

void Log::show(char* resultString){
    FILE* fp;
    if((fp = fopen("wanData/logs.txt", "rt"))==NULL){
        printf("No log history!\n");
        sprintf(resultString, "No log history!\n");
        return;
    }
    char timeArray[10], command[50];
    fread(timeArray, 10, 1, fp);
    timeArray[10] = '\0';
    printf("There are %s logs:\n", timeArray);
    sprintf(resultString, "There are %s logs:\n", timeArray);
    char ch;
    int count = atoi(timeArray);
    for(int i=0;i<(count<5?count:5);i++){
        fread(timeArray, 10, 1, fp);
        printf("%s ", timeArray);
        sprintf(resultString+strlen(resultString), "%s ", timeArray);
        fread(command, 50, 1, fp);
        printf("\"%s\"\n", command);
        sprintf(resultString+strlen(resultString), "\"%s\"\n", command);
    }
    fclose(fp);
}

bool Log::clear(){
    FILE* fp;
    fp = fopen("wanData/logs.txt", "wt");
    fprintf(fp, "         0");
    fclose(fp);
    return true;
}
