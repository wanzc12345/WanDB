#ifndef HASHARRAY_H
#define HASHARRAY_H
#include "hash_node.h"
#define ARRAY_SIZE 100001

class HashArray{
private:
    HashNode* nodeArray[ARRAY_SIZE];
    int cnodeAmount[ARRAY_SIZE];
    int maxCnodeAmount[ARRAY_SIZE];

    int hashFunction(char* key);
public:
    HashArray();
    ~HashArray();
    void show();
    bool save(char* key, long offset);
    bool drop(char* key);
    long getOffset(char* key);
};

#endif // HASHARRAY_H
