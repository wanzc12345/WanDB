#include "../header/graph_node.h"
#include <stdio.h>
#include <string.h>

GraphNode::GraphNode(char* k, char* d){
    printf("\t\t\tCreating graph node \"%s\"...\n", k);
    key = new char[10];
    if(k != NULL)
        setKey(k);
    data = new char[50];
	if(d != NULL)
        setData(d);
	adjacentNodeAmount = 0;
	maxAdjacentNodeAmount = 1;
	adjacentNodeList = new GraphNode*[1];
	adjacentNodeList[0] = NULL;
    printf("\t\t\tCreating graph node done.\n");
}

GraphNode::~GraphNode(){
    printf("\t\t\tDeleting graph node: \"%s\"...\n", key);
	delete [] key;
	delete [] data;
	delete [] adjacentNodeList;
    printf("\t\t\tDeleting graph node done.\n");
}

void GraphNode::show(char* resultString){
    if(adjacentNodeAmount!=0){
        printf("{%s:%s}->[", key, data);
        sprintf(resultString+strlen(resultString), "{%s:%s}->[", key, data);
        printf("%s", adjacentNodeList[0]->getKey(), adjacentNodeList[0]->getData());
        sprintf(resultString+strlen(resultString), "%s", adjacentNodeList[0]->getKey(), adjacentNodeList[0]->getData());
        if(adjacentNodeAmount > 1)
            for(int i=1;i<adjacentNodeAmount;i++){
                printf(",%s", adjacentNodeList[i]->getKey(), adjacentNodeList[i]->getData());
                sprintf(resultString+strlen(resultString), ",%s", adjacentNodeList[i]->getKey(), adjacentNodeList[i]->getData());
            }
        printf("]");
        sprintf(resultString+strlen(resultString), "]");
    }else{
        printf("{%s:%s}", key, data);
        sprintf(resultString+strlen(resultString), "{%s:%s}", key, data);
    }
}

char* GraphNode::getInfo(){
    char *nodeInfo = new char[200];
    sprintf(nodeInfo, "{\"%s\":\"%s\"", key, data);
    if(adjacentNodeAmount!=0){
        sprintf(nodeInfo, "%s,\"%s\":[\"%s", nodeInfo, key, adjacentNodeList[0]->getKey());
        if(adjacentNodeAmount > 1)
            for(int i=1;i<adjacentNodeAmount;i++)
                sprintf(nodeInfo, "%s,\"%s\"", nodeInfo, key, adjacentNodeList[i]->getKey());
        sprintf(nodeInfo, "%s]", nodeInfo);
    }
    return nodeInfo;
}

bool GraphNode::setKey(char* newKey){
    printf("\t\t\tChanging node key from \"%s\" to \"%s\"...\n", key, newKey);
	if(strlen(key)<strlen(newKey)){
		key = new char[strlen(newKey)];
	}
	strcpy(key, newKey);
    printf("\t\t\tChanging node key done.\n");
	return true;
}

char* GraphNode::getKey(){
	return key;
}

bool GraphNode::setData(char* newData){
    printf("\t\t\tChanging node \"%s\" data from \"%s\" to \"%s\"...\n", key, data, newData);
	if(strlen(data)<strlen(newData)){
        data = new char[strlen(newData)];
    }
    strcpy(data, newData);
    printf("\t\t\tChanging node \"%s\" data done.\n", key);
    return true;
}

char* GraphNode::getData(){
	return data;
}

int GraphNode::getAdjacentNodeAmount(){
	return adjacentNodeAmount;
}

bool GraphNode::addAdjacentNode(GraphNode* n){

    printf("\t\t\tNode %s adding adjacent link to %s...\n", key, n->getKey());
	
	//if list is full, double its size
	if(adjacentNodeAmount == maxAdjacentNodeAmount){
		maxAdjacentNodeAmount *= 2;
		GraphNode** tmp = new GraphNode*[maxAdjacentNodeAmount];
		for(int i=0;i<adjacentNodeAmount;i++)
			tmp[i] = adjacentNodeList[i];
		adjacentNodeList = new GraphNode*[maxAdjacentNodeAmount];
		for(int i=0;i<adjacentNodeAmount;i++)
			adjacentNodeList[i] = tmp[i];
        //delete [] tmp;
	}
	
	adjacentNodeList[adjacentNodeAmount] = n;
	adjacentNodeAmount++;
    printf("\t\t\tNode %s adding adjacent link to %s done.\n", key, n->getKey());
	return true;
}

bool GraphNode::deleteAdjacentNode(GraphNode *n){
    printf("\t\t\tNode %s deleting adjacent link to %s...\n", key, n->getKey());
    int i = 0;
    for(;i<adjacentNodeAmount;i++){
        if(adjacentNodeList[i] == n)
            break;
    }
    if(i != adjacentNodeAmount){
        adjacentNodeList[i] = adjacentNodeList[adjacentNodeAmount-1];
        adjacentNodeAmount--;
    }
    printf("\t\t\tNode %s deleting adjacent link to %s done.\n", key, n->getKey());
    return true;
}

bool GraphNode::hasAdjacentNode(GraphNode *node){
    for(int i=0;i<adjacentNodeAmount;i++){
        if(adjacentNodeList[i] == node)
            return true;
    }
    return false;
}

GraphNode** GraphNode::getAdjacentNodeList(){
	return adjacentNodeList;
}
