#pragma once
#include "graph_node.h"
#include "file.h"

class File;

class Graph{

	private:
		GraphNode* nodeList;
		char* title;
		int nodeAmount;
		int maxNodeAmount;

	protected:
	public:
        Graph(char* title=NULL, char* graphInfo=NULL);
		Graph(int number, int** adjacentMatrix);
		~Graph();
        void show(char* resultString, int start=1);
        void clear();
		GraphNode* getNodeList();
        bool insert(char* graphInfo);
        bool insertToFile(File* file, char* graphInfo);
        bool deleteData(char* graphInfo);
        bool updateData(char* graphInfo);
        bool updateDataToFile(File* file, char* graphInfo);
        int getSize();
		char* getNodeData(char* key);
		int getNodeAmount();
		char* getTitle();
		int getIndex(char* key);
        char* getGraphData();
		bool setTitle(char* newTitle);
		bool showNode(char* key);
		bool deleteNode(char* key);
		bool updateNode(char* key, char* data);
		bool addNode(char* key, char* data);
		bool createConnection(char* fromKey, char* toKey);
		bool destroyConnection(char* fromKey, char* toKey);
        bool hasConnectionBetween(char* fromKey, char* toKey);
};
