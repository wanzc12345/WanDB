#ifndef HASHMAP_H
#define HASHMAP_H
#include "hash_node.h"

class HashList{
private:
    HashNode nodeArray[100001];
    HashNode* conflictNodes;
    int cnodeAmount;
    int maxCnodeAmount;

    int hashFunction(char* key);
public:
    HashList();
    ~HashList();
    void show();
    bool save(char* key, long offset);
    bool drop(char* key);
    long getOffset(char* key);
};
#endif // HASHMAP_H
