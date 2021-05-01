#include "../header/hash_node.h"
#include <stdlib.h>

HashNode::HashNode(){
    keyPointer = NULL;
    offset = 0;
    next = NULL;
}

HashNode::~HashNode(){

}

bool HashNode::pointTo(char *key){
    keyPointer = key;
    return true;
}

bool HashNode::setNext(HashNode *n){
    next = n;
    return true;
}

bool HashNode::dropNext(){
    next = NULL;
    return true;
}

HashNode* HashNode::getNext(){
    return next;
}

long HashNode::getOffset(){
    return offset;
}

bool HashNode::setOffset(long o){
    offset = o;
    return true;
}

char* HashNode::getKey(){
    return keyPointer;
}
