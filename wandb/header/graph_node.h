#pragma once
#include <stdio.h>

class GraphNode{
	private:
		char* key;
		char* data;
		int adjacentNodeAmount;
		int maxAdjacentNodeAmount;
		GraphNode** adjacentNodeList;
	protected:
	public:
		GraphNode(char* key=NULL, char* data=NULL);
		~GraphNode();
        void show(char* resultString);
		bool setKey(char* newKey);
		char* getKey();
		bool setData(char* newData);
		char* getData();
        char* getInfo();
		int getAdjacentNodeAmount();
		bool addAdjacentNode(GraphNode* node);
        bool deleteAdjacentNode(GraphNode* node);
        bool hasAdjacentNode(GraphNode* node);
		GraphNode** getAdjacentNodeList();
};
