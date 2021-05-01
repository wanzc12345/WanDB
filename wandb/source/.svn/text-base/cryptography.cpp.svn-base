#include "../header/cryptography.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include <time.h>
#include <string.h>

Cryptography::Cryptography(){
    FILE* fp;
    if((fp = fopen("wanData/seed.txt", "rt+"))==NULL){
        seed = time(0);
        //srand(seed);
    }else{
        char seedArray[50];
        fscanf(fp, "%s", seedArray);
        seed = atoi(seedArray);
        //srand(seed);
        fclose(fp);
    }
}

Cryptography::~Cryptography(){
    FILE* fp;
    fp = fopen("wanData/seed.txt", "wt");
    char seedArray[50];
    sprintf(seedArray, "%d", seed);
    fprintf(fp, "%s", seedArray);
    fclose(fp);
}

bool Cryptography::encrypt(char *inText, char *outText){
    if(strlen(inText)!=strlen(outText)){
        return false;
    }
    srand(seed);
    for(int i=0;i<strlen(inText);i++){
        outText[i] = inText[i]-(rand()%30);
    }
    return true;
}

bool Cryptography::decrypt(char *inText, char *outText){
    if(strlen(inText)!=strlen(outText)){
        return false;
    }
    srand(seed);
    for(int i=0;i<strlen(inText);i++){
        outText[i] = inText[i]+(rand()%30);
    }
    return true;
}

bool Cryptography::encrypt(char *filename){
    srand(seed);
    FILE* fp;
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("Can't open file \"%s\"!", filename);
        return false;
    }

    char fuck;
    while((fuck = fgetc(fp))!=EOF){
        fuck = fuck-rand()%30;
        fseek(fp, -1, SEEK_CUR);
        fputc(fuck, fp);
    }
    fclose(fp);
    return true;
}

bool Cryptography::decrypt(char *filename){
    srand(seed);
    FILE* fp;
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("Can't open file \"%s\"!", filename);
        return false;
    }

    char fuck;
    while((fuck = fgetc(fp))!=EOF){
        fuck = fuck+rand()%30;
        fseek(fp, -1, SEEK_CUR);
        fputc(fuck, fp);
    }
    fclose(fp);
    return true;
}
