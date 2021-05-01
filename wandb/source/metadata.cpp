#include "../header/metadata.h"
#include "string.h"
#include "stdio.h"

Metadata::Metadata(){
    currentKeyAmount = 0;
    maxKeyAmount = 1;
    keys = new char*[maxKeyAmount];
    for(int i=0;i<maxKeyAmount;i++){
        keys[i] = new char[10];
    }
    addresses = new char*[maxKeyAmount];
    for(int i=0;i<maxKeyAmount;i++){
        addresses[i] = new char[15];
    }
}

Metadata::~Metadata(){
    for(int i=0;i<maxKeyAmount;i++){
        delete [] keys[i];
        delete [] addresses[i];
    }
    delete [] keys;
    delete [] addresses;
}

int Metadata::getSize(){
    return 25*maxKeyAmount;
}

bool Metadata::save(char *key, char *ip){
    if(maxKeyAmount == currentKeyAmount){
        maxKeyAmount *= 2;
        char** temp = new char*[currentKeyAmount];
        char** temp2= new char*[currentKeyAmount];
        for(int i=0;i<currentKeyAmount;i++){
            temp[i] = new char[10];
            temp2[i] = new char[15];
            strcpy(temp[i], keys[i]);
            strcpy(temp2[i], addresses[i]);
        }
        keys = new char*[maxKeyAmount];
        addresses = new char*[maxKeyAmount];
        for(int i=0;i<maxKeyAmount;i++){
            keys[i] = new char[10];
            addresses[i] = new char[15];
        }
        for(int i=0;i<currentKeyAmount;i++){
            strcpy(keys[i], temp[i]);
            strcpy(addresses[i], temp2[i]);
        }
    }
    if(key&&ip){
        strcpy(keys[currentKeyAmount], key);
        strcpy(addresses[currentKeyAmount], ip);
        currentKeyAmount++;
        return true;
    }else
        return false;
}

bool Metadata::drop(char *key){
    int i=0;
    for(;i<currentKeyAmount;i++){
        if(!strcmp(keys[i], key)){
            strcpy(keys[i], keys[currentKeyAmount-1]);
            currentKeyAmount--;
            break;
            return true;
        }
    }
    if(i == currentKeyAmount)
        return false;
}

char* Metadata::getIpByName(char *key){
    for(int i=0;i<currentKeyAmount;i++){
        if(!strcmp(keys[i], key)){
            return addresses[i];
        }
    }
    return NULL;
}

void Metadata::show(){
    printf("Metadata:\n");
    for(int i=0;i<currentKeyAmount;i++){
        printf("%d{\"%s\":\"%s\"}\n", i, keys[i], addresses[i]);
    }
}
