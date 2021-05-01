#include "../header/hash_array.h"
#include "string.h"
#include "stdio.h"

HashArray::HashArray(){
    for(int i=0;i<ARRAY_SIZE;i++){
        cnodeAmount[i] = 0;
        maxCnodeAmount[i] = 1;
        nodeArray[i] = new HashNode[maxCnodeAmount[i]];
    }
}

HashArray::~HashArray(){
    for(int i=0;i<ARRAY_SIZE;i++)
        delete [] nodeArray[i];
}

int HashArray::hashFunction(char *key){
    int sum = 0;
    for(int i=0;i<strlen(key);i++){
        sum += key[i];
    }
    return sum % ARRAY_SIZE;
}

void HashArray::show(){
    printf("showing hash table...\n");
    for(int i=0;i<ARRAY_SIZE;i++){
        if(nodeArray[i][0].getOffset()!=0){
            printf("%d ", i);
            for(int j=0;j<cnodeAmount[i];j++)
                printf("[%s:%d]", nodeArray[i][j].getKey(), nodeArray[i][j].getOffset());
            printf("\n");
        }
    }
    printf("showing hash table done.\n");
}

bool HashArray::save(char *key, long offset){
    int index = hashFunction(key);

    if(cnodeAmount[index]!=0){//conflicts happens here
        if(cnodeAmount[index] == maxCnodeAmount[index]){
            maxCnodeAmount[index] *= 2;
            HashNode* tmp = new HashNode[cnodeAmount[index]];
            for(int i=0;i<cnodeAmount[index];i++)
                tmp[i] = nodeArray[index][i];
            nodeArray[index] = new HashNode[maxCnodeAmount[index]];
            for(int i=0;i<cnodeAmount[index];i++)
                nodeArray[index][i] = tmp[i];
        }
        nodeArray[index][cnodeAmount[index]].pointTo(key);
        nodeArray[index][cnodeAmount[index]].setOffset(offset);
        cnodeAmount[index]++;
    }else{
        nodeArray[index][0].pointTo(key);
        nodeArray[index][0].setOffset(offset);
        cnodeAmount[index]++;
    }

    return true;
}

bool HashArray::drop(char *key){
    int index = hashFunction(key);
    for(int j=0;j<cnodeAmount[index];j++){
        if(!strcmp(nodeArray[index][j].getKey(), key)){
            nodeArray[index][j].pointTo(nodeArray[index][cnodeAmount[index]-1].getKey());
            nodeArray[index][j].setOffset(nodeArray[index][cnodeAmount[index]-1].getOffset());
            cnodeAmount[index]--;
            return true;
        }
    }
    return false;
}

long HashArray::getOffset(char *key){
    int index = hashFunction(key);
    for(int i=0;i<cnodeAmount[index];i++){
        if(!strcmp(nodeArray[index][i].getKey(), key)){
            return nodeArray[index][i].getOffset();
        }
    }
    return -1;
}

