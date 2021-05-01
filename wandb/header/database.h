#pragma once
#include "graph.h"
class Graph;

class Database{
	private:
		Graph* graphList;
		int graphAmount;
		int maxGraphAmount;
        char* title;

        int getIndex(char* title);
	protected:
	public:
        Database(char* title=NULL);
		~Database();
        bool copy(Database* db);
        void showGraphs(char* resultString, int start=1);
        Graph* getGraph(char* title);
        bool setTitle(char* newTitle);
        char* getTitle();
        int getSize();
        int getGraphAmount(){return graphAmount;}
        Graph* getGraphList(){return graphList;}
        bool addGraph(char* title, char* graphInfo=NULL);
        bool deleteGraph(char* title);
        bool clear();
};
