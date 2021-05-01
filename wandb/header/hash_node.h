#ifndef HASHMAP_NODE_H
#define HASHMAP_NODE_H

class HashNode{
private:
    char* keyPointer;
    long offset;
    HashNode* next;
public:
    HashNode();
    ~HashNode();
    bool setNext(HashNode* next);
    HashNode* getNext();
    bool dropNext();
    bool pointTo(char* key);
    bool setOffset(long o);
    long getOffset();
    char* getKey();
};

#endif // HASHMAP_NODE_H
