#ifndef INSTANCE_H
#define INSTANCE_H
#include "database.h"
#include "router.h"
#include "metadata.h"
#include "tenant.h"
#include "log.h"
#include "file.h"
#include "cryptography.h"

class File;
class Database;
enum GranularityLevel{DATABASE, GRAPH, GRAPHDATA};

class Instance
{
private:
    char* ip_address;
    int port;
    Router router;
    Metadata metadata;

    Database* databaseList;
    Database* currentDatabase;
    int databaseAmount;
    int maxDatabaseAmount;

    Tenant* tenantList;
    Tenant* currentTenant;
    int tenantAmount;
    int maxTenantAmount;

    char* queryResult;

    bool isPersistent;
    bool switchOn;
    GranularityLevel granularity;

    File* file;
    Log* log;
    Cryptography crypto;

public:
    Instance(bool isFile = true, bool isArray = false, int grain = 1);
    ~Instance();
    void show(int start=1);
    char* execute(char* command);
    bool addTenant(char* n, char* p, int account = 10000, int isAdmin = false);
    Tenant* getTenantByName(char* n);
    Tenant* getTenant(int i){return &tenantList[i];}
    bool delTenant(char* n);
    char* addDatabase(char* databaseTitle);
    void showTenants(int start=1);
    Database* getDatabaseByTitle(char* databaseTitle);
    Database* getDatabase(int i){return &databaseList[i];}
    int getIndex(char* databaseTitle);
    char* delDatabase(char* databaseTitle);
    Metadata* getMetadata(){return &metadata;}
    Router* getRouter(){return &router;}
    int getSize();
    int getTenantAmount(){return tenantAmount;}
    int getDatabaseAmount(){return databaseAmount;}
    char* getQueryResult(){return queryResult;}
    bool setCurrentTenant(Tenant* tenant){currentTenant = tenant;}
    bool setCurrentDatabase(Database* db){currentDatabase = db;}
    void run();
    void test();
};

#endif // INSTANCE_H
