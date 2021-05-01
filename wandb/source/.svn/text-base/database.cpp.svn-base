#include "../header/database.h"
#include <stdio.h>
#include <string.h>

Database::Database(char* t){
    title = new char[10];
    if(t != NULL){
        delete [] title;
        title = new char[strlen(t)];
        strcpy(title, t);
    }
    printf("\tCreating database \"%s\"...\n", title);
	graphAmount = 0;
	maxGraphAmount = 1;
    graphList = new Graph[maxGraphAmount];
    printf("\tCreating database done.\n");
}

Database::~Database(){
    printf("\tDeleting database \"%s\"...\n", title);
	delete [] graphList;
    delete [] title;
    printf("\tDeleting database done.\n");
}

bool Database::addGraph(char* t, char* graphData){

    printf("\tDatabase \"%s\" adding graph \"%s\"...\n", title, t);
	//if graph list is full, double its size
	if(graphAmount == maxGraphAmount){
		maxGraphAmount *= 2;
		Graph* tmp = new Graph[maxGraphAmount];
		for(int i=0;i<graphAmount;i++)
			tmp[i] = graphList[i];
		graphList = new Graph[maxGraphAmount];
		for(int i=0;i<graphAmount;i++)
			graphList[i] = tmp[i];
	}
    graphList[graphAmount].setTitle(t);
    graphList[graphAmount].insert(graphData);
	graphAmount++;
    printf("\tDatabase \"%s\" adding graph done.\n", title);
	return true;
}

bool Database::deleteGraph(char *title){
    printf("\tDeleting graph \"%s\"...\n", title);
    Graph* g = getGraph(title);
    g->clear();
    g->setTitle(graphList[graphAmount-1].getTitle());
    g->insert(graphList[graphAmount-1].getGraphData());
    graphList[graphAmount-1].clear();
    graphAmount--;
    printf("\tDeleting graph done.\n");
}

void Database::showGraphs(char* resultString, int start){

    printf("\tShowing graphs...\n");
    sprintf(resultString+strlen(resultString), "\tShowing graphs...\n");

    if(start<=0||graphAmount==0||start>graphAmount){
        printf("\tShowing graphs done.\n");
        sprintf(resultString+strlen(resultString), "\tShowing graphs done.\n");
        return;
    }

    for(int i=start-1;i<(start+4<graphAmount?start+4:graphAmount);i++){
        sprintf(resultString+strlen(resultString), "\t\"%s\"\n", graphList[i].getTitle());
        printf("\t\"%s\"\n", graphList[i].getTitle());
    }
    printf("\tShowing graphs done.\n");
    sprintf(resultString+strlen(resultString), "\tShowing graphs done.\n");
}

bool Database::copy(Database *db){
    this->setTitle(db->getTitle());
    this->clear();
    for(int i=0;i<db->getGraphAmount();i++){
        this->addGraph(db->getGraphList()[i].getTitle(), db->getGraphList()[i].getGraphData());
    }
    return true;
}

Graph* Database::getGraph(char *graphTitle){
    int index = getIndex(graphTitle);
    if(index!=-1)
        return &graphList[index];
    else
        return NULL;
}

int Database::getIndex(char* graphTitle){
    for(int i=0;i<graphAmount;i++)
        if(!strcmp(graphTitle, graphList[i].getTitle()))
            return i;
    return -1;
}

char* Database::getTitle(){
    return title;
}

int Database::getSize(){
    int sum=0;
    for(int i=0;i<graphAmount;i++){
        sum += graphList[i].getSize();
    }
    return sum;
}

bool Database::setTitle(char *newTitle){
    printf("Database changing title from \"%s\" to \"%s\"...", title, newTitle);
    if(newTitle != NULL){
        if(strlen(title) < strlen(newTitle)){
            delete [] title;
            title = new char[strlen(newTitle)];
        }
        strcpy(title, newTitle);
        printf("Database changing title done.\n");
        return true;
    }else{
        printf("Database changing title failed.\n");
        return false;
    }
}

bool Database::clear(){
    graphAmount = 0;
}
