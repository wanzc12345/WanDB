#include "../header/hash_list.h"
#include "string.h"
#include "stdio.h"

HashList::HashList(){
    maxCnodeAmount = 1;
    cnodeAmount = 0;
    conflictNodes = new HashNode[maxCnodeAmount];
}

HashList::~HashList(){
    delete [] conflictNodes;
}

int HashList::hashFunction(char *key){
    int sum = 0;
    for(int i=0;i<strlen(key);i++){
        sum += key[i];
    }
    return sum % 100001;
}

void HashList::show(){
    printf("showing hash table...\n");
    HashNode* p;
    for(int i=0;i<100001;i++){
        if(nodeArray[i].getOffset()){
            printf("%d [%s:%d]", i, nodeArray[i].getKey(), nodeArray[i].getOffset());
            p = nodeArray[i].getNext();
            while(p){
                printf("->[%s:%d]", p->getKey(), p->getOffset());
                p = p->getNext();
            }
            printf("\n");
        }
    }
    printf("showing hash table done.\n");
}

bool HashList::save(char *key, long offset){
    int index = hashFunction(key);

    if(nodeArray[index].getOffset()!=0){//conflicts happens here
        if(cnodeAmount == maxCnodeAmount){
            maxCnodeAmount *= 2;
            HashNode* tmp = new HashNode[cnodeAmount];
            for(int i=0;i<cnodeAmount;i++)
                tmp[i] = conflictNodes[i];
            conflictNodes = new HashNode[maxCnodeAmount];
            for(int i=0;i<cnodeAmount;i++)
                conflictNodes[i] = tmp[i];
        }
        conflictNodes[cnodeAmount].pointTo(key);
        conflictNodes[cnodeAmount].setOffset(offset);
        conflictNodes[cnodeAmount].setNext(nodeArray[index].getNext());
        nodeArray[index].setNext(&conflictNodes[cnodeAmount]);
        cnodeAmount++;
    }else{
        nodeArray[index].pointTo(key);
        nodeArray[index].setOffset(offset);
    }

    return true;
}

bool HashList::drop(char *key){
    int index = hashFunction(key);
    HashNode* p = &nodeArray[index];
    if(p->getOffset()!=0){
        if(!strcmp(p->getKey(), key)){
            if(p->getNext()){
                p->pointTo(p->getNext()->getKey());
                p->setOffset(p->getNext()->getOffset());
                p->setNext(p->getNext()->getNext());
                return true;
            }else{
                p->pointTo(NULL);
                p->setOffset(0);
                return true;
            }
        }else{
            while(p->getNext()){
                if(!strcmp(p->getNext()->getKey(), key)){
                    p->getNext()->pointTo(NULL);
                    p->getNext()->setOffset(0);
                    p->setNext(p->getNext()->getNext());
                    return true;
                }
            }
            return false;
        }
    }else{
        return false;
    }
    return true;
}

long HashList::getOffset(char *key){
    int index = hashFunction(key);
    HashNode* p = &nodeArray[index];
    if(p->getOffset()!=0){
        while(p&&strcmp(p->getKey(), key)){
            p = p->getNext();
        }
        if(p)
            return p->getOffset();
        else return -1;
    }else
        return -1;
}
