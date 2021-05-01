#ifndef FILE_H
#define FILE_H
#include "hash_array.h"
#include "hash_list.h"
#include "instance.h"

class Instance;
class Tenant;
class Database;
class Graph;

class File{
private:
    bool modeIsArray;//true for array, false for list
    HashArray* hashArray;
    HashList* hashList;

public:
    File(bool m = true);
    ~File();
    bool makeDirectory();
    bool addDatabase(char* dbTitle);
    bool addGraph(Database* db, char* graphTitle);
    bool addTenant(char* name, char* pass, int account, bool isAdmin);
    bool addMetadata(char* name, char* ip);
    bool grantDatabase(Tenant* tenant, char* dbTitle);
    bool revokeDatabase(Tenant* tenant, Database* db);
    bool insertGraphNode(Graph* graph, char* key, char* data);
    bool updateGraphNodeData(Graph* graph, char* key, char* data);
    bool createGraphNodeConnection(Graph* graph, char* key1, char* key2);
    bool saveDatabase(Database* db);
    bool saveGraph(Graph* graph);
    bool saveInstance(Instance* instance);
    bool saveTenant(Tenant* tenant);
    bool deleteGraphNode(Database* db, Graph* graph, char* key);
    bool destroyGraphNodeConnection(Graph* graph, char* key1, char* key2);
    bool deleteDatabase(char* dbTitle);
    bool deleteMetadata(char* name);
    bool deleteGraph(char* dbTitle, char* graphTitle);
    bool deleteTenant(char* name);
    bool deleteInstance(Instance* instance);
    //bool readGraphNode(char* graphTitle, GraphNode* graphNode, char* key);
    bool readGraphNodeData(char* graphTitle, char* key, char* data);
    bool readGraphNodeConnection(Graph* graph, char* key1, char* key2);
    bool readGraph(Graph* graph, char* graphTitle);
    bool readDatabase(Database* db, char* dbTitle);
    bool readTenant(Tenant* tenant, char* name);
    bool readInstance(Instance* instance);
};
#endif // FILE_H
