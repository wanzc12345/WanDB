#include "../header/file.h"
#include "string.h"
#include "sys/stat.h"
#include <unistd.h>
#include "sys/types.h"
#include <stdlib.h>

File::File(bool m){
    modeIsArray = m;
    if(modeIsArray){
        hashArray = new HashArray[1];
        hashList = NULL;
    }else{
        hashList = new HashList[1];
        hashArray = NULL;
    }
    makeDirectory();
    //readHash();
}

File::~File(){
    if(modeIsArray){
        delete [] hashArray;
    }else{
        delete [] hashList;
    }
    //saveHash();
}

bool File::makeDirectory(){
    mkdir("wanData", 00777);
//  mkdir("wanData/instance", 00777);
    mkdir("wanData/database", 00777);
    mkdir("wanData/graph", 00777);
    mkdir("wanData/tenant", 00777);
}

bool File::insertGraphNode(Graph *graph, char *key, char *data){
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/graph/%s.txt", graph->getTitle());
    if((fp = fopen(filename, "rt+"))==NULL){
        //printf("Can't open file %s!\n", graphTitle);
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        fp = fopen(filename, "rt+");
    }

    long count;
    char countArray[10];
    fread(countArray, 10, 1, fp);
    count = atoi(countArray);

    if(modeIsArray){
        hashArray->save(key, count);
    }else{
        hashList->save(key, count);
    }

    fseek(fp, 10+count*210, SEEK_SET);
    fwrite(key, 10, 1, fp);
    fwrite(data, 200, 1, fp);

    fseek(fp, 0, SEEK_SET);
    count++;
    sprintf(countArray, "%d", count);
    fwrite(countArray, 10, 1, fp);
    fclose(fp);

    //update connection file

    sprintf(filename, "wanData/graph/%s_connection.txt", graph->getTitle());
    if((fp = fopen(filename, "wt"))==NULL){
        printf("Connection file not exists!\n");
        return false;
    }

    for(int i=0;i<count;i++){
        for(int j=0;j<count;j++){
            if(graph->hasConnectionBetween(graph->getNodeList()[i].getKey(), graph->getNodeList()[j].getKey()))
                fputs("1", fp);
            else
                fputs("0", fp);
        }
        fputs("\n", fp);
    }

    fclose(fp);
    return true;
}

bool File::updateGraphNodeData(Graph *graph, char *key, char *data){
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/graph/%s.txt", graph->getTitle());
    if((fp = fopen(filename, "rt+"))==NULL){
        //printf("Can't open file %s!\n", graphTitle);
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        fp = fopen(filename, "rt+");
    }

    long offset;
    if(modeIsArray){
        offset = hashArray->getOffset(key);
    }else{
        offset = hashList->getOffset(key);
    }

    fseek(fp, 10+offset*210+10, SEEK_SET);
    fwrite(data, 200, 1, fp);

    fclose(fp);
    return true;
}

//bool File::readGraphNode(char *graphTitle, GraphNode *graphNode, char *key){
//    return true;
//}

bool File::readGraphNodeData(char *graphTitle, char *key, char* data){
    char filename[20];
    sprintf(filename, "wanData/graph/%s.txt", graphTitle);
    FILE* fp;
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("No graph data!");
        fclose(fp);
        return false;
    }

    long offset;
    if(modeIsArray){
        offset = hashArray->getOffset(key);
    }else{
        offset = hashList->getOffset(key);
    }
    if(offset == -1){
        printf("No this node!");
        fclose(fp);
        return false;
    }

    fseek(fp, 10+offset*210+10, SEEK_SET);
    fread(data, 200, 1, fp);

    fclose(fp);
    return true;
}

bool File::deleteGraphNode(Database* db, Graph *graph, char* key){
    char filename[30];
    sprintf(filename, "wanData/graph/%s.txt", graph->getTitle());
    FILE* fp;
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("No graph data!");
        fclose(fp);
        return false;
    }

    long count;
    char countArray[10];
    fread(countArray, 10, 1, fp);
    count = atoi(countArray);

    long offset;
    if(modeIsArray){
        offset = hashArray->getOffset(key);
    }else{
        offset = hashList->getOffset(key);
    }
    if(offset == -1){
        printf("No this node!");
        fclose(fp);
        return false;
    }

    char data[200];
    fseek(fp, -210, SEEK_END);
    fread(data, 210, 1, fp);
    fseek(fp, 10+210*offset, SEEK_SET);
    fwrite(data, 210, 1, fp);

    fseek(fp, 0, SEEK_SET);
    count--;
    sprintf(countArray, "%d", count);
    fwrite(countArray, 10, 1, fp);

    fclose(fp);

    //update connection file

    sprintf(filename, "wanData/graph/%s_connection.txt", graph->getTitle());
    count = graph->getNodeAmount();
    if((fp = fopen(filename, "wt"))==NULL){
        return true;
    }

    for(int i=0;i<count;i++){
        for(int j=0;j<count;j++){
            if(graph->hasConnectionBetween(graph->getNodeList()[i].getKey(), graph->getNodeList()[j].getKey()))
                fputs("1", fp);
            else
                fputs("0", fp);
        }
        fputs("\n", fp);
    }

    fclose(fp);
    return true;
}

bool File::createGraphNodeConnection(Graph *graph, char *key1, char *key2){
    char filename[30];
    sprintf(filename, "wanData/graph/%s_connection.txt", graph->getTitle());
    FILE* fp;
    int count = graph->getNodeAmount();
    if((fp = fopen(filename, "rt+"))==NULL){
        fp = fopen(filename, "wt");
        for(int i=0;i<count;i++){
            for(int j=0;j<count;j++){
                fputs("0", fp);
            }
            fputs("\n", fp);
        }
        fclose(fp);
        fp = fopen(filename, "rt+");
    }

    int line, row;
    line = graph->getIndex(key1);
    row = graph->getIndex(key2);
    if(line==-1||row==-1){
        printf("No these keys!\n");
        return false;
    }

    fseek(fp, line*(count+1)+row, SEEK_SET);
    fwrite("1", 1, 1, fp);

    fclose(fp);
    return true;
}

bool File::destroyGraphNodeConnection(Graph* graph, char *key1, char *key2){
    char filename[30];
    sprintf(filename, "wanData/graph/%s_connection.txt", graph->getTitle());
    FILE* fp;
    int count = graph->getNodeAmount();
    if((fp = fopen(filename, "rt+"))==NULL){
        fp = fopen(filename, "wt");
        for(int i=0;i<count;i++){
            for(int j=0;j<count;j++){
                fputs("0", fp);
            }
            fputs("\n", fp);
        }
        fclose(fp);
        fp = fopen(filename, "rt+");
    }

    int line, row;
    line = graph->getIndex(key1);
    row = graph->getIndex(key2);
    if(line==-1||row==-1){
        printf("No these keys!\n");
        return false;
    }

    fseek(fp, line*(count+1)+row, SEEK_SET);
    fwrite("0", 1, 1, fp);

    fclose(fp);
    return true;
}

bool File::readGraphNodeConnection(Graph* graph, char *key1, char *key2){
    char filename[30];
    sprintf(filename, "wanData/graph/%s_connection.txt", graph->getTitle());
    FILE* fp;
    int count = graph->getNodeAmount();
    if((fp = fopen(filename, "rt+"))==NULL){
        fp = fopen(filename, "wt");
        for(int i=0;i<count;i++){
            for(int j=0;j<count;j++){
                fputs("0", fp);
            }
            fputs("\n", fp);
        }
        fclose(fp);
        return false;
    }

    int line, row;
    line = graph->getIndex(key1);
    row = graph->getIndex(key2);
    if(line==-1||row==-1){
        printf("No these keys!\n");
        return false;
    }

    char result[1];
    fseek(fp, line*(count+1)+row, SEEK_SET);
    fread(result, 1, 1, fp);

    fclose(fp);
    if(!strcmp(result, "1"))
        return true;
    else
        return false;
}

bool File::addGraph(Database *db, char *graphTitle){
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/database/%s.txt", db->getTitle());
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("Can't open file %s!\n", db->getTitle());
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        return false;
    }

    char countArray[10];
    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, 10+count*10, SEEK_SET);
    fwrite(graphTitle, 10, 1, fp);

    fseek(fp, 0, SEEK_SET);
    sprintf(countArray, "%d", count+1);
    fwrite(countArray, 10, 1, fp);
    fclose(fp);
    return true;
}

bool File::saveGraph(Graph *graph){
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/graph/%s.txt", graph->getTitle());
    if((fp = fopen(filename, "rt+"))==NULL){
        //printf("Can't open file %s!\n", graphTitle);
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
    }
    fclose(fp);

    for(int i=0;i<graph->getNodeAmount();i++)
        insertGraphNode(graph, graph->getNodeList()[i].getKey(), graph->getNodeList()[i].getData());

    return true;
}

bool File::readGraph(Graph *graph, char *graphTitle){
    graph->setTitle(graphTitle);
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/graph/%s.txt", graph->getTitle());
    if((fp = fopen(filename, "rt+"))==NULL){
        //printf("Can't open file %s!\n", graphTitle);
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        return false;
    }

    long count;
    char countArray[10];
    fread(countArray, 10, 1, fp);
    count = atoi(countArray);

    char key[10];
    char data[200];
    for(int i=0;i<count;i++){
        fread(key, 10, 1, fp);
        fread(data, 200, 1, fp);
        graph->addNode(key, data);
    }

    fclose(fp);
    return true;
}

bool File::deleteGraph(char* dbTitle, char *graphTitle){
    char filename[30], countArray[10], titleArray[10];
    FILE* fp;

    sprintf(filename, "wanData/database/%s.txt", dbTitle);
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("No data for database %s!", dbTitle);
        return false;
    }

    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, -10, SEEK_END);
    fread(countArray, 10, 1, fp);

    fseek(fp, 10, SEEK_SET);
    for(int i=0;i<count;i++){
        fread(titleArray, 10, 1, fp);
        if(!strcmp(titleArray, graphTitle)){
            fseek(fp, -10, SEEK_CUR);
            fwrite(countArray, 10, 1, fp);

            fseek(fp, 0, SEEK_SET);
            sprintf(countArray, "%d", count-1);
            fwrite(countArray, 10, 1, fp);
            fflush(fp);
            break;
        }
    }

    sprintf(filename, "wanData/graph/%s.txt", graphTitle);
    if((access(filename, 0))!=-1){
        //printf("Can't open file %s!\n", graphTitle);
        remove(filename);
    }else
        return false;

    sprintf(filename, "wanData/graph/%s_connection.txt", graphTitle);
    if(access(filename, 0)!=-1){
        //printf("Can't open file %s!\n", graphTitle);
        remove(filename);
    }else
        return false;
    return true;
}

bool File::addDatabase(char *dbTitle){
    char filename[30];
    FILE* fp;
    sprintf(filename, "wanData/databases.txt", dbTitle);
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("Can't open file %s!\n", dbTitle);
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        return false;
    }

    char countArray[10];
    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, 10+count*10, SEEK_SET);
    sprintf(countArray, "%s", dbTitle);
    fwrite(countArray, 10, 1, fp);

    fseek(fp, 0, SEEK_SET);
    sprintf(countArray, "%d", count+1);
    fwrite(countArray, 10, 1, fp);

    fclose(fp);
    return true;
}

bool File::grantDatabase(Tenant *tenant, char *dbTitle){
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/tenant/%s.txt", tenant->getUsername());
    if((fp = fopen(filename, "rt"))==NULL){
        printf("Can't open file %s!\n", dbTitle);
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        return false;
    }

    char countArray[10];
    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, 0, SEEK_END);
    fwrite(dbTitle, 10, 1, fp);

    fseek(fp, 0, SEEK_SET);
    sprintf(countArray, "%d", count+1);
    fwrite(countArray, 10, 1, fp);
    return true;
}

bool File::saveDatabase(Database *db){
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/database/%s.txt", db->getTitle());
    if((fp = fopen(filename, "wt"))==NULL){
        printf("Can't open file %s!\n", db->getTitle());
        return false;
    }

    char countArray[10];
    sprintf(countArray, "%d", db->getGraphAmount());
    fwrite(countArray, 10, 1, fp);

    for(int i=0;i<db->getGraphAmount();i++){
        fprintf(fp, "%s\n", db->getGraphList()[i].getTitle());
    }

    fclose(fp);
    return true;
}

bool File::readDatabase(Database *db, char *dbTitle){
    db->setTitle(dbTitle);
    char filename[20];
    FILE* fp;
    sprintf(filename, "wanData/database/%s.txt", db->getTitle());
    if((fp = fopen(filename, "rt"))==NULL){
        //printf("Can't open file %s!\n", graphTitle);
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        return false;
    }

    char countArray[10];
    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    char graphTitle[100];
    for(int i=0;i<count;i++){
        fscanf(fp, "%s", graphTitle);
        db->addGraph(graphTitle);
    }

    fclose(fp);
    return true;
}

bool File::deleteDatabase(char *dbTitle){
    char filename[30];
    FILE* fp;
    char countArray[10], titleArray[10], title2Array[10];

    sprintf(filename, "wanData/databases.txt");
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("No databases.txt");
        return false;
    }

    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, -10, SEEK_END);
    fread(title2Array, 10, 1, fp);

    fseek(fp, 10, SEEK_SET);
    for(int i=0;i<count;i++){
        fread(titleArray, 10, 1, fp);
        if(!strcmp(titleArray, dbTitle)){
            fseek(fp, -10, SEEK_CUR);
            fwrite(title2Array, 10, 1, fp);

            fseek(fp, 0, SEEK_SET);
            sprintf(countArray, "%d", count-1);
            fwrite(countArray, 10, 1, fp);
            fflush(fp);
            break;
        }
    }

    sprintf(filename, "wanData/database/%s.txt", dbTitle);
    if((fp = fopen(filename, "rt"))==NULL){
        printf("No data for database %s!\n", dbTitle);
        return false;
    }

    fread(countArray, 10, 1, fp);
    count = atoi(countArray);

    char graphTitle[100];
    for(int i=0;i<count;i++){
        fscanf(fp, "%s", graphTitle);
        deleteGraph(dbTitle, graphTitle);
    }

    remove(filename);
    fclose(fp);
    return true;
}

bool File::saveInstance(Instance* instance){
    char filename[30], countArray[10];
    FILE* fp;

    //save metadata
    sprintf(filename, "wanData/metadata.txt");
    if((fp = fopen(filename, "wt"))==NULL){
        printf("Can't open file %s!", filename);
        fclose(fp);
    }else{
        int count = instance->getMetadata()->getKeyAmount();
        sprintf(countArray, "%d", count);
        fwrite(countArray, 10, 1, fp);

        for(int i=0;i<count;i++){
            fwrite(instance->getMetadata()->getKey(i), 10, 1, fp);
            fwrite(instance->getMetadata()->getIp(i), 15, 1, fp);
        }
    }

    //save tenants
    sprintf(filename, "wanData/tenants.txt");
    if((fp = fopen(filename, "wt"))==NULL){
        printf("Can't open file %s!", filename);
        fclose(fp);
    }else{
        int count = instance->getTenantAmount();
        sprintf(countArray, "%d", count);
        fwrite(countArray, 10, 1, fp);

        for(int i=0;i<count;i++){
            fwrite(instance->getTenant(i)->getUsername(), 10, 1, fp);
            fwrite(instance->getTenant(i)->getPassword(), 10, 1, fp);
            sprintf(countArray, "%d", instance->getTenant(i)->getAccount());
            fwrite(countArray, 10, 1, fp);
            fputc(instance->getTenant(i)->hasPermissionOnAll()+48, fp);
            saveTenant(instance->getTenant(i));
        }
    }

    //save databases
    sprintf(filename, "wanData/databases.txt");
    if((fp = fopen(filename, "wt"))==NULL){
        printf("Can't open file %s!", filename);
        fclose(fp);
    }else{
        int count = instance->getDatabaseAmount();
        sprintf(countArray, "%d", count);
        fwrite(countArray, 10, 1, fp);

        for(int i=0;i<count;i++){
            fwrite(instance->getDatabase(i)->getTitle(), 10, 1, fp);
            saveDatabase(instance->getDatabase(i));
        }
    }

    fclose(fp);
    return true;
}

bool File::readInstance(Instance *instance){
    char filename[30], countArray[10];
    FILE* fp;

    //read metadata
    sprintf(filename, "wanData/metadata.txt");
    if((fp = fopen(filename, "rt"))==NULL){
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        //fclose(fp);
    }else{
        char ipArray[15];
        fread(countArray, 10, 1, fp);
        int count = atoi(countArray);

        for(int i=0;i<count;i++){
            fread(countArray, 10, 1, fp);
            fread(ipArray, 15, 1, fp);
            instance->getMetadata()->save(countArray, ipArray);
        }
    }

    //read tenants
    sprintf(filename, "wanData/tenants.txt");
    if((fp = fopen(filename, "rt"))==NULL){
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
//      fclose(fp);
    }else{
        fread(countArray, 10, 1, fp);
        int count = atoi(countArray);
        char passArray[10], accountArray[10], isAdminArray;

        for(int i=0;i<count;i++){
            fread(countArray, 10, 1, fp);
            fread(passArray, 10, 1, fp);
            fread(accountArray, 10, 1, fp);
            isAdminArray = fgetc(fp);
            instance->addTenant(countArray, passArray, atoi(accountArray), isAdminArray-48);
            readTenant(instance->getTenantByName(countArray), countArray);
            instance->setCurrentTenant(instance->getTenantByName(countArray));
        }
    }

    //read databases
    sprintf(filename, "wanData/databases.txt");
    if((fp = fopen(filename, "rt"))==NULL){
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        //fclose(fp);
    }else{
        fread(countArray, 10, 1, fp);
        int count = atoi(countArray);

        for(int i=0;i<count;i++){
            fread(countArray, 10, 1, fp);
            instance->addDatabase(countArray);
            readDatabase(instance->getDatabaseByTitle(countArray), countArray);
            instance->setCurrentDatabase(instance->getDatabaseByTitle(countArray));
        }
    }

    fclose(fp);
    return true;
}

bool File::deleteInstance(Instance* instance){//reinitialization

    return true;
}

bool File::readTenant(Tenant* tenant, char *name){
    char filename[30];
    FILE* fp;

    sprintf(filename, "wanData/tenant/%s.txt", name);
    if((fp = fopen(filename, "rt"))==NULL){
        fp = fopen(filename, "wt");
        fprintf(fp, "         0");
        fclose(fp);
        return false;
    }

    char countArray[10];
    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    for(int i=0;i<count;i++){
        fread(countArray, 10, 1, fp);
        tenant->grantDatabase(countArray);
    }

    fclose(fp);
    return true;
}

bool File::addTenant(char *name, char *pass, int account, bool isAdmin){
    char filename[30];
    FILE* fp;
    char countArray[10];

    sprintf(filename, "wanData/tenants.txt", name);
    if((fp = fopen(filename, "rt"))==NULL){
        //printf("Can't open file %s!\n", name);
        fp = fopen(filename, "wt");
        fprintf(fp, "         1");
        fwrite(name, 10, 1, fp);
        fwrite(pass, 10, 1, fp);
        sprintf(countArray, "%d", account);
        fwrite(countArray, 10, 1, fp);
        fputc(isAdmin+48, fp);
        fclose(fp);
        return false;
    }

    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, 0, SEEK_END);
    fwrite(name, 10, 1, fp);
    fwrite(pass, 10, 1, fp);
    sprintf(countArray, "%d", account);
    fwrite(countArray, 10, 1, fp);
    fputc(isAdmin+48, fp);

    fseek(fp, 0, SEEK_SET);
    sprintf(countArray, "%d", count+1);
    fwrite(countArray, 10, 1, fp);

    fflush(fp);
    fclose(fp);
    return true;
}

bool File::saveTenant(Tenant* tenant){
    char filename[30];
    FILE* fp;
    sprintf(filename, "wanData/tenant/%s.txt", tenant->getUsername());
    if((fp = fopen(filename, "wt"))==NULL){
        printf("Can't open file %s!\n", tenant->getUsername());
        return false;
    }

    char countArray[10];
    sprintf(countArray, "%d", tenant->getDatabaseAmount());
    fwrite(countArray, 10, 1, fp);

    for(int i=0;i<tenant->getDatabaseAmount();i++){
        fprintf(fp, "%s\n", tenant->getDatabaseTitle(i));
    }

    fclose(fp);
    return true;
}

bool File::addMetadata(char *name, char *ip){
    char filename[30];
    FILE* fp;
    char countArray[10];

    sprintf(filename, "wanData/metadata.txt", name);
    if((fp = fopen(filename, "rt"))==NULL){
        //printf("Can't open file %s!\n", name);
        fp = fopen(filename, "wt");
        fprintf(fp, "         1");
        fwrite(name, 10, 1, fp);
        fwrite(ip, 15, 1, fp);
        fclose(fp);
        return false;
    }

    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, 0, SEEK_END);
    fwrite(name, 10, 1, fp);
    fwrite(ip, 15, 1, fp);

    fseek(fp, 0, SEEK_SET);
    sprintf(countArray, "%d", count+1);
    fwrite(countArray, 10, 1, fp);

    fflush(fp);
    fclose(fp);
    return true;
}

bool File::deleteMetadata(char *name){
    char filename[30];
    FILE* fp;
    char countArray[10], titleArray[10], title2Array[10], ipArray[15], ip2Array[15];

    sprintf(filename, "wanData/metadata.txt");
    if((fp = fopen(filename, "rt+"))==NULL){
        printf("No metadata.txt");
        return false;
    }

    fread(countArray, 10, 1, fp);
    int count = atoi(countArray);

    fseek(fp, 10+25*(count-1), SEEK_SET);
    fread(title2Array, 10, 1, fp);
    fread(ip2Array, 15, 1, fp);

    fseek(fp, 10, SEEK_SET);
    for(int i=0;i<count;i++){
        fread(titleArray, 10, 1, fp);
        fread(ipArray, 15, 1, fp);
        if(!strcmp(titleArray, name)){
            fseek(fp, -25, SEEK_CUR);
            fwrite(title2Array, 10, 1, fp);
            fwrite(ipArray, 15, 1, fp);

            fseek(fp, 0, SEEK_SET);
            sprintf(countArray, "%d", count-1);
            fwrite(countArray, 10, 1, fp);
            fflush(fp);
            break;
        }
    }

    return true;
}
