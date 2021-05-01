#include "../header/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Graph::Graph(char* t, char* graphInfo){

    if(t != NULL){
        title = new char[strlen(t)];
        strcpy(title ,t);
    }else
        title = new char[10];
    printf("\t\tCreating graph \"%s\"...\n", title);

	nodeAmount = 0;
    if(graphInfo != NULL)
        for(int i=0;i<strlen(graphInfo);i++){
            if(graphInfo[i] == ':')
                nodeAmount++;
        }
	maxNodeAmount = nodeAmount + 1;
	nodeList = new GraphNode[maxNodeAmount];
    insert(graphInfo);
    printf("\t\tCreating graph done.\n");

}

bool Graph::insert(char* graphData){

    char key[100] = "", value[200] = "";
    int start = -1, end = -1;
    bool isConnection = false;
    if(graphData){
        int len = strlen(graphData);
        for(int i=0;i<len;i++){
            if(graphData[i] == '\"' && !strcmp(key, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        key[s] = graphData[k];
                    start = end = -1;
                }
            }else if(graphData[i] == '\"'&&!strcmp(value, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        value[s] = graphData[k];
                    start = end = -1;
                    if(isConnection)
                        createConnection(key, value);
                    else
                        addNode(key, value);
                    int s = strlen(key);
                    if(!isConnection)
                        for(int k=0;k<s;k++)
                            key[k]='\0';
                    s = strlen(value);
                    for(int k=0;k<s;k++)
                        value[k]='\0';
                }
            }else if(graphData[i] == '[')
                isConnection = true;
            else if(graphData[i] == ']')
                isConnection = false;
        }
    }
    return true;
}

bool Graph::insertToFile(File *file, char *graphData){
    char key[100] = "", value[200] = "";
    int start = -1, end = -1;
    bool isConnection = false;
    if(graphData){
        int len = strlen(graphData);
        for(int i=0;i<len;i++){
            if(graphData[i] == '\"' && !strcmp(key, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        key[s] = graphData[k];
                    start = end = -1;
                }
            }else if(graphData[i] == '\"'&&!strcmp(value, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        value[s] = graphData[k];
                    start = end = -1;
                    if(isConnection)
                        file->createGraphNodeConnection(this, key, value);
                    else
                        file->insertGraphNode(this, key, value);
                    int s = strlen(key);
                    if(!isConnection)
                        for(int k=0;k<s;k++)
                            key[k]='\0';
                    s = strlen(value);
                    for(int k=0;k<s;k++)
                        value[k]='\0';
                }
            }else if(graphData[i] == '[')
                isConnection = true;
            else if(graphData[i] == ']')
                isConnection = false;
        }
    }
    return true;
}

bool Graph::deleteData(char *graphData){
    char key[100] = "", value[200] = "";
    int start = -1, end = -1;
    bool isConnection = false;
    if(graphData){
        int len = strlen(graphData);
        for(int i=0;i<len;i++){
            if(graphData[i] == '\"' && !strcmp(key, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        key[s] = graphData[k];
                    start = end = -1;
                }
            }else if(graphData[i] == '\"'&&!strcmp(value, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        value[s] = graphData[k];
                    start = end = -1;
                    if(isConnection)
                        destroyConnection(key, value);
                    else
                        deleteNode(key);
                    int s = strlen(key);
                    if(!isConnection)
                        for(int k=0;k<s;k++)
                            key[k]='\0';
                    s = strlen(value);
                    for(int k=0;k<s;k++)
                        value[k]='\0';
                }
            }else if(graphData[i] == '[')
                isConnection = true;
            else if(graphData[i] == ']')
                isConnection = false;
        }
    }
    return true;
}

bool Graph::updateData(char *graphData){
    char key[100] = "", value[200] = "";
    int start = -1, end = -1;
    bool isConnection = false;
    if(graphData){
        int len = strlen(graphData);
        for(int i=0;i<len;i++){
            if(graphData[i] == '\"' && !strcmp(key, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        key[s] = graphData[k];
                    start = end = -1;
                }
            }else if(graphData[i] == '\"'&&!strcmp(value, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        value[s] = graphData[k];
                    start = end = -1;
                    if(isConnection)
                        createConnection(key, value);
                    else
                        nodeList[getIndex(key)].setData(value);
                    int s = strlen(key);
                    if(!isConnection)
                        for(int k=0;k<s;k++)
                            key[k]='\0';
                    s = strlen(value);
                    for(int k=0;k<s;k++)
                        value[k]='\0';
                }
            }else if(graphData[i] == '[')
                isConnection = true;
            else if(graphData[i] == ']')
                isConnection = false;
        }
    }
    return true;
}

bool Graph::updateDataToFile(File *file, char *graphData){
    char key[100] = "", value[200] = "";
    int start = -1, end = -1;
    bool isConnection = false;
    if(graphData){
        int len = strlen(graphData);
        for(int i=0;i<len;i++){
            if(graphData[i] == '\"' && !strcmp(key, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        key[s] = graphData[k];
                    start = end = -1;
                }
            }else if(graphData[i] == '\"'&&!strcmp(value, "")){
                if(start == -1)
                    start = i;
                else{
                    end = i;
                    for(int k=start+1, s=0;k<end;k++, s++)
                        value[s] = graphData[k];
                    start = end = -1;
                    if(isConnection)
                        file->createGraphNodeConnection(this, key, value);
                    else
                        file->updateGraphNodeData(this, key, value);
                    int s = strlen(key);
                    if(!isConnection)
                        for(int k=0;k<s;k++)
                            key[k]='\0';
                    s = strlen(value);
                    for(int k=0;k<s;k++)
                        value[k]='\0';
                }
            }else if(graphData[i] == '[')
                isConnection = true;
            else if(graphData[i] == ']')
                isConnection = false;
        }
    }
    return true;
}

Graph::Graph(int n, int** m){
	
    printf("\t\tCreating graph from adjacent matrix...\n");
	nodeAmount = n;
	maxNodeAmount = n;
	nodeList = new GraphNode[nodeAmount];

        //assign initial keys to all nodes
        for(int i=0;i<nodeAmount;i++){
                sprintf(nodeList[i].getKey(), "%d", i);
        }

	//creating connections
	for(int i=0;i<nodeAmount;i++){
		for(int j=0;j<nodeAmount;j++){
			if(m[i][j] != 0 && i != j){
				char from[10], to[10];
				sprintf(from, "%d", i);
				sprintf(to, "%d", j);
				createConnection(from, to);
			}
		}
	}

    printf("\t\tCreating graph done.\n");

}

Graph::~Graph(){
	
    printf("\t\tDeleting graph...\n");
	delete [] nodeList;
	delete [] title;
    printf("\t\tDeleting graph Done.\n");

}

int Graph::getSize(){
    return maxNodeAmount*210 + 10;
}

void Graph::clear(){
    nodeAmount = 0;
}

void Graph::show(char* resultString, int start){

    printf("\t\tShowing graph \"%s\"...\n", title);
    sprintf(resultString+strlen(resultString), "\t\tShowing graph \"%s\"...\n", title);
    printf("\t\t{");
    sprintf(resultString+strlen(resultString), "\t\t{");
    if(nodeAmount != 0)
        nodeList[0].show(resultString);

    if(nodeAmount == 0||start<=0||start>nodeAmount){
        printf("}\n\t\tShowing graph done.\n");
        sprintf(resultString+strlen(resultString), "}\n\t\tShowing graph done.\n");
        return;
    }
    for(int i=start-1;i<(start+4<nodeAmount?start+4:nodeAmount);i++){
        printf(",");
        sprintf(resultString+strlen(resultString), ",");
        nodeList[i].show(resultString);
    }
    printf("}\n\t\tShowing graph done.\n");
    sprintf(resultString+strlen(resultString), "}\n\t\tShowing graph done.\n");
}

GraphNode* Graph::getNodeList(){
	return nodeList;
}

int Graph::getNodeAmount(){
	return nodeAmount;
}

char* Graph::getNodeData(char* k){
	return nodeList[getIndex(k)].getData();
}

char* Graph::getGraphData(){
    char* graphData = new char[200];
    sprintf(graphData, "{");
    if(nodeAmount != 0)
        strcat(graphData, nodeList[0].getInfo());
    printf("%s", graphData);
    if(nodeAmount > 1){
    for(int i=1;i<nodeAmount;i++){
        strcat(graphData, ",");
        strcat(graphData, nodeList[i].getInfo());
    }}
    strcat(graphData, "}");
    printf("%s", graphData);
    return graphData;
}

char* Graph::getTitle(){
	return title;
}

bool Graph::setTitle(char* t){
    printf("\t\tChanging graph title from \"%s\" to \"%s\"...\n", title, t);
    if(t != NULL){
        if(strlen(title) < strlen(t)){
            delete [] title;
            title = new char[strlen(t)];
        }
        strcpy(title, t);
        printf("\t\tChanging graph title done.\n");
        return true;
    }else{
        printf("\t\tChanging graph title fail.\n");
        return false;
    }
}

bool Graph::addNode(char* k, char* d){
	
    printf("\t\tAdding node %s: data{%s}\n", k, d);

	//if node list is full, double size
	if(nodeAmount == maxNodeAmount){
		maxNodeAmount *= 2;
		GraphNode* tmp = new GraphNode[maxNodeAmount];
		for(int i=0;i<nodeAmount;i++)
			tmp[i] = nodeList[i];
		nodeList = new GraphNode[maxNodeAmount];
		for(int i=0;i<nodeAmount;i++)
			nodeList[i] = tmp[i];
	}
	nodeList[nodeAmount].setKey(k);
	nodeList[nodeAmount].setData(d);
	nodeAmount++;
    printf("\t\tAdding node %s done.\n", k);
	return true;

}

bool Graph::deleteNode(char *k){
    printf("\t\tDeleting node %s from graph %s...\n", k, title);
    GraphNode* gn = NULL;
    for(int i=0;i<nodeAmount;i++){
        if(!strcmp(nodeList[i].getKey(), k))
            gn = &nodeList[i];
    }
    if(gn != NULL){
        for(int i=0;i<nodeAmount;i++)
            nodeList[i].deleteAdjacentNode(gn);
        gn->setKey(nodeList[nodeAmount-1].getKey());
        gn->setData(nodeList[nodeAmount-1].getData());
        nodeAmount--;
    }
    printf("\t\tDeleting node %s from graph %s done.\n", k, title);
    return true;
}

bool Graph::createConnection(char* fromKey, char* toKey){
	
    printf("\t\tCreating connection from %s to %s...\n", fromKey, toKey);
	nodeList[getIndex(fromKey)].addAdjacentNode(&nodeList[getIndex(toKey)]);
    printf("\t\tCreating connection done.\n");
	return true;

}

bool Graph::destroyConnection(char* fromKey, char* toKey){
    printf("\t\tDestroying connection from %s to %s...\n", fromKey, toKey);
    nodeList[getIndex(fromKey)].deleteAdjacentNode(&nodeList[getIndex(toKey)]);
    printf("\t\tDestroying connection done.\n");
	return true;
}

bool Graph::hasConnectionBetween(char *fromKey, char *toKey){
    return nodeList[getIndex(fromKey)].hasAdjacentNode(&nodeList[getIndex(toKey)]);
}

int Graph::getIndex(char* k){
    int i = k[0] % nodeAmount;
    while(strcmp(nodeList[i].getKey(), k)){
        i = ++i % nodeAmount;
    }
    return i;
}
