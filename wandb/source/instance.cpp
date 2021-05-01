#include "../header/instance.h"
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

Instance::Instance(bool isFile, bool isArray, int grain)
{
    printf("Creating instance...\n");
    queryResult = new char[200];
    ip_address = new char[15];
    strcpy(ip_address, "localhost");

    port = 42391;
    databaseAmount = 0;
    maxDatabaseAmount = databaseAmount + 1;
    databaseList = new Database[maxDatabaseAmount];

    tenantAmount = 0;
    maxTenantAmount = 1;
    tenantList = new Tenant[maxTenantAmount];

    currentDatabase = NULL;
    currentTenant = NULL;

    switchOn = true;

    isPersistent = isFile;
    if(isPersistent){
        file = new File(isArray);
        file->readInstance(this);
    }else{
        file = NULL;
    }

    switch(grain){
    case 0:
        granularity = DATABASE;
    case 1:
        granularity = GRAPH;
    case 2:
        granularity = GRAPHDATA;
    default:
        granularity = GRAPH;
    }

    log = new Log;

    //addTenant("wan", "42391", 10000, 1);
    printf("Creating instance done.\n");
}

Instance::~Instance(){
    //file->saveInstance(this);
    delete [] ip_address;
    delete [] databaseList;
    delete [] queryResult;
    delete [] tenantList;
    if(isPersistent)
        delete file;
    delete log;
}

char* Instance::execute(char* command){
    printf("Executing command \"%s\"...\n", command);

    char commandCopy[50];
    strcpy(commandCopy, command);
    char *curToken = strtok(command, " "), data[20], graph_name[20];
    if(!strcmp(curToken, "create")){
        curToken = strtok(NULL, " ");
        if(curToken && (strstr(curToken, "database")
                ||strstr(curToken, "db"))){
            curToken = strtok(NULL, " ");
            if(curToken&&strcmp(curToken, "")){
                addDatabase(curToken);
                currentDatabase = getDatabaseByTitle(curToken);
                if(isPersistent){
                    file->addDatabase(curToken);
                    file->grantDatabase(currentTenant, curToken);
                }
                strcpy(queryResult, "Done.\n\0");
                log->logCommand(commandCopy);
            }else{
                strcpy(queryResult, "Error:unknown database name.\n");
                printf("%s", queryResult);
            }
        }else if(curToken && (strstr(curToken, "graph"))){
            curToken = strtok(NULL, " ");
            if(curToken && strcmp(curToken, "")){
                strcpy(graph_name, curToken);
                curToken = strtok(NULL, " ");
                if(curToken && strcmp(curToken, "")){
                    if(currentDatabase){
                        if(currentDatabase->getSize() > router.getMinSize()&&router.getIpAmount()!=0){
                            char* temp = router.getMinIp();
                            router.sendMessage(temp, strcat("#create database ", currentDatabase->getTitle()));
                            char message[50];
                            sprintf(message, "#create graph %s %s", graph_name, curToken);
                            router.sendMessage(temp, message);

                            metadata.save(graph_name, temp);
                            currentDatabase->addGraph(graph_name);
                            if(isPersistent){
                                file->addGraph(currentDatabase, graph_name);
                                file->addMetadata(graph_name, temp);
                            }
                            strcpy(queryResult, "Done.\n\0");
                            log->logCommand(commandCopy);
                        }else{
                            currentDatabase->addGraph(graph_name, curToken);
                            if(isPersistent){
                                file->addGraph(currentDatabase, graph_name);
                                currentDatabase->getGraph(graph_name)->insertToFile(file, curToken);
                            }
                            strcpy(queryResult, "Done.\n\0");
                            log->logCommand(commandCopy);
                        }
                    }else{
                        strcpy(queryResult, "Error:unspecified database.\n");
                        printf("%s", queryResult);
                    }
                }else{
                    if(currentDatabase){
                        if(currentDatabase->getSize() > router.getMinSize()&&router.getIpAmount()!=0){
                            char* temp = router.getMinIp();
                            char message[50];
                            sprintf(message, "#create database %s", currentDatabase->getTitle());
                            router.sendMessage(temp, message);
                            sprintf(message, "#create graph %s", graph_name);
                            router.sendMessage(temp, message);

                            metadata.save(graph_name, temp);
                            currentDatabase->addGraph(graph_name);
                            if(isPersistent)
                                file->addMetadata(graph_name, temp);
                            strcpy(queryResult, "Done.\n\0");
                            log->logCommand(commandCopy);
                        }else{
                            currentDatabase->addGraph(graph_name);
                            if(isPersistent)
                                file->addGraph(currentDatabase, graph_name);
                            strcpy(queryResult, "Done.\n\0");
                            log->logCommand(commandCopy);
                        }
                    }else{
                        strcpy(queryResult, "Error:unspecified database.\n");
                        printf("%s", queryResult);
                    }
                }
            }else{
                strcpy(queryResult, "Error:unknown graph name.\n");
                printf("%s", queryResult);
            }
        }else{
            strcpy(queryResult, "Error:unknown command.\n");
            printf("%s", queryResult);
        }
    }else if(!strcmp(curToken, "delete")||!strcmp(curToken, "del")){
        curToken = strtok(NULL, " ");
        if(!strcmp(curToken, "db")||!strcmp(curToken, "database")){
            curToken = strtok(NULL, " ");
            if(curToken&&strcmp(curToken, "")){
                for(int i=0;i<getDatabaseByTitle(curToken)->getGraphAmount();i++){
                    char* tempIp;
                    tempIp = metadata.getIpByName(getDatabaseByTitle(curToken)->getGraphList()[i].getTitle());
                    if(tempIp){
                        char message[50];
                        sprintf(message, "#use %s", currentDatabase->getTitle());
                        router.sendMessage(message, tempIp);
                        sprintf(message, "#del graph %s", getDatabaseByTitle(curToken)->getGraphList()[i].getTitle());
                        router.sendMessage(message, tempIp);
                    }
                    metadata.drop(getDatabaseByTitle(curToken)->getGraphList()[i].getTitle());
                    if(isPersistent)
                        file->deleteMetadata(getDatabaseByTitle(curToken)->getGraphList()[i].getTitle());
                }
                delDatabase(curToken);
                if(isPersistent)
                    file->deleteDatabase(curToken);
                strcpy(queryResult, "Done.\n\0");
                log->logCommand(commandCopy);
            }else{
                strcpy(queryResult, "Error:unkown database name.\n");
                printf("%s", queryResult);
            }
        }else if(!strcmp(curToken, "graph")){
            curToken = strtok(NULL, " ");
            if(curToken&&strcmp(curToken, "")){
                if(metadata.getIpByName(curToken)){
                    char *tempIp, message[50];
                    tempIp = metadata.getIpByName(curToken);
                    sprintf(message, "#use %s", currentDatabase->getTitle());
                    router.sendMessage(message, tempIp);
                    sprintf(message, "#del graph %s", curToken);
                    router.sendMessage(message, tempIp);

                    metadata.drop(curToken);
                    if(isPersistent)
                        file->deleteMetadata(curToken);
                }else{
                    this->currentDatabase->deleteGraph(curToken);
                    if(isPersistent)
                        file->deleteGraph(currentDatabase->getTitle(), curToken);
                }

                strcpy(queryResult, "Done.\n\0");
                log->logCommand(commandCopy);
            }else{
                strcpy(queryResult, "Error:unkown graph name.\n");
                printf("%s", queryResult);
            }
        }else{
            strcpy(data, curToken);
            curToken = strtok(NULL, " ");
            if(!strcmp(curToken, "from")){
                curToken = strtok(NULL, " ");
                if(curToken&&strcmp(curToken, "")){
                    if(metadata.getIpByName(curToken)){
                        char *tempIp, message[50];
                        tempIp = metadata.getIpByName(curToken);
                        sprintf(message, "#use %s", currentDatabase->getTitle());
                        router.sendMessage(message, tempIp);
                        sprintf(message, "#del %s from %s", data, curToken);
                        router.sendMessage(message, tempIp);
                    }else{
                        this->currentDatabase->getGraph(curToken)->deleteData(data);
                        if(isPersistent){
                            file->deleteGraphNode(currentDatabase, currentDatabase->getGraph(curToken), data);
                        }
                    }
                    strcpy(queryResult, "Done.\n\0");
                    log->logCommand(commandCopy);
                }else{
                    strcpy(queryResult, "Error:unkown graph name.\n");
                    printf("%s", queryResult);
                }
            }else{
                strcpy(queryResult, "Error:unkown command.\n");
                printf("%s", queryResult);
            }
        }
    }else if(!strcmp(curToken, "show")){
        curToken = strtok(NULL, " ");
        if(curToken && !strcmp(curToken, "graphs")){
            if(currentDatabase)
                currentDatabase->showGraphs(queryResult);
            else{
                strcpy(queryResult, "Error:unspecified database.\n");
                printf("%s", queryResult);
            }
        }else if(curToken&&!strcmp(curToken, "graph")){
            curToken = strtok(NULL, " ");
            if(curToken&&strcmp(curToken, "")){
                if(currentDatabase&&currentDatabase->getGraph(curToken)){
                    if(metadata.getIpByName(curToken)){
                        char *tempIp, message[50];
                        tempIp = metadata.getIpByName(curToken);
                        sprintf(message, "#use %s", currentDatabase->getTitle());
                        router.sendMessage(message, tempIp);
                        sprintf(message, "#show graph %s", curToken);
                        router.sendMessage(message, tempIp);
                    }else
                        currentDatabase->getGraph(curToken)->show(queryResult);
                }else{
                    strcpy(queryResult, "Error:unknown graph name.\n");
                    printf("%s", queryResult);
                }
            }else{
                strcpy(queryResult, "Error:unknown graph name.\n");
                printf("%s", queryResult);
            }
        }else if(curToken&&!strcmp(curToken, "profile")){
            strcpy(queryResult, "Linux.\n");
            printf("%s", queryResult);
        }else if(curToken&&!strcmp(curToken, "size")){
            sprintf(queryResult, "%d B.\n", getSize());
            printf("%s", queryResult);
        }else if(curToken&&!strcmp(curToken, "tenants")){
            if(currentTenant->hasPermissionOnAll())
                showTenants();
            else{
                sprintf(queryResult, "Error:you don't have that priority.\n");
                printf("%s", queryResult);
            }
        }else if(curToken&&!strcmp(curToken, "logs")){
            log->show(queryResult);
        }else
            show();
    }else if(!strcmp(curToken, "insert")){
        curToken = strtok(NULL, " ");
        if(curToken && strcmp(curToken, "")){
            strcpy(data, curToken);
            curToken = strtok(NULL, " ");
            if(curToken && !strcmp(curToken, "into")){
                curToken = strtok(NULL, " ");
                if(curToken && strcmp(curToken, "")){
                    if(currentDatabase&&currentDatabase->getGraph(curToken)){
                        if(metadata.getIpByName(curToken)){
                            char *tempIp, message[50];
                            tempIp = metadata.getIpByName(curToken);
                            sprintf(message, "#use %s", currentDatabase->getTitle());
                            router.sendMessage(message, tempIp);
                            sprintf(message, "#insert %s into %s", data, curToken);
                            router.sendMessage(message, tempIp);
                        }else{
                            this->currentDatabase->getGraph(curToken)->insert(data);
                            if(isPersistent){
                                currentDatabase->getGraph(curToken)->insertToFile(file, data);
                            }
                        }
                        strcpy(queryResult, "Done.\n\0");
                        log->logCommand(commandCopy);
                    }else{
                        strcpy(queryResult, "Error:unkown database or graph.\n");
                        printf("%s", queryResult);
                    }
                }else{
                    strcpy(queryResult, "Error:unknown graph name.\n");
                    printf("%s", queryResult);
                }
            }else{
                strcpy(queryResult, "Error:unknown command.\n");
                printf("%s", queryResult);
            }
        }else{
            strcpy(queryResult, "Error:unknown data type.\n");
            printf("%s", queryResult);
        }
    }else if(!strcmp(curToken, "use")){
        curToken = strtok(NULL, " ");
        if(curToken&&strcmp(curToken, "")&&getDatabaseByTitle(curToken)){
            this->currentDatabase = getDatabaseByTitle(curToken);
            strcpy(queryResult, "Done.\n\0");
            log->logCommand(commandCopy);
        }else{
            strcpy(queryResult, "Error:unknown database.\n\0");
            sprintf("%s", queryResult);
        }
    }else if(!strcmp(curToken, "update")){
        curToken = strtok(NULL, " ");
        strcpy(data, curToken);
        curToken = strtok(NULL, " ");
        if(!strcmp(curToken, "from")){
            curToken = strtok(NULL, " ");
            if(curToken&&strcmp(curToken, "")){
                if(metadata.getIpByName(curToken)){
                    char *tempIp, message[50];
                    tempIp = metadata.getIpByName(curToken);
                    sprintf(message, "#use %s", currentDatabase->getTitle());
                    router.sendMessage(message, tempIp);
                    sprintf(message, "#update %s from %s", data, curToken);
                    router.sendMessage(message, tempIp);
                }else{
                    this->currentDatabase->getGraph(curToken)->updateData(data);
                    if(isPersistent)
                        currentDatabase->getGraph(curToken)->updateDataToFile(file, data);
                }
                strcpy(queryResult, "Done.\n\0");
                log->logCommand(commandCopy);
            }else{
                strcpy(queryResult, "Error:unkown graph name.\n");
                printf("%s", queryResult);
            }
        }else{
            strcpy(queryResult, "Error:unkown command.\n");
            printf("%s", queryResult);
        }
    }else if(!strcmp(curToken, "shutdown")){
        switchOn = false;
        strcpy(queryResult, "Shutting down...\n");
        printf("%s", queryResult);
        exit(0);
    }else if(!strcmp(curToken, "login")){
        curToken = strtok(NULL, " ");
        if(curToken&&getTenantByName(curToken)){
            char username[10];
            strcpy(username, curToken);
            curToken = strtok(NULL, " ");
            if(curToken&&getTenantByName(username)->checkPassword(curToken)){
                currentTenant = getTenantByName(username);
                strcpy(queryResult, "ok");
                printf("%s", queryResult);
            }else{
                strcpy(queryResult, "Wrong password!\n");
                printf("%s", queryResult);
            }
        }else{
            char username[10];
            strcpy(username, curToken);
            curToken = strtok(NULL, " ");
            addTenant(username, curToken, 10000, true);
            currentTenant = getTenantByName(username);
            strcpy(queryResult, "ok");
            printf("%s", queryResult);
        }
    }else{
        strcpy(queryResult, "Error:unknown command.\n");
        printf("%s", queryResult);
    }
    printf("Executing command done.\n");
    return queryResult;
}

void Instance::show(int start){

    printf("Instance information:\n"
           "IP:%s\nPort:%d\nDatabases(%d-%d):\n", ip_address, port
           , start, (start+4<databaseAmount?start+4:databaseAmount));
    sprintf(queryResult, "Instance information:\nIP:"
            "%s\nPort:%d\nDatabases(%d-%d):\n", ip_address, port
            , start, (start+4<databaseAmount?start+4:databaseAmount));
    if(start<=0||databaseAmount == 0||start>databaseAmount){
        sprintf(queryResult+strlen(queryResult), "(null)\n");
        return;
    }
    for(int i=start-1;i<(start+4<databaseAmount?start+4:databaseAmount);i++){
        printf("\"%s\"\n", databaseList[i].getTitle());
        sprintf(queryResult+strlen(queryResult), "\"%s\"\n", databaseList[i].getTitle());
    }
    if(currentDatabase){
        printf("Current database:%s\n", currentDatabase->getTitle());
        sprintf(queryResult+strlen(queryResult), "Current database:%s\n", currentDatabase->getTitle());
    }

}

void Instance::showTenants(int start){

    printf("All tenants(%d-%d):\n", start, (start+4<tenantAmount?start+4:tenantAmount));
    sprintf(queryResult, "All tenants(%d-%d):\n", start, (start+4<tenantAmount?start+4:tenantAmount));

    if(start<=0||tenantAmount == 0||start>tenantAmount){
        sprintf(queryResult+strlen(queryResult), "(null)\n");
        return;
    }
    for(int i=start-1;i<(start+4<tenantAmount?start+4:tenantAmount);i++){
        printf("\"%s\"\n", tenantList[i].getUsername());
        sprintf(queryResult+strlen(queryResult), "\"%s\"\n", tenantList[i].getUsername());
    }
    if(currentTenant){
        printf("Current tenant:%s\n", currentTenant->getUsername());
        sprintf(queryResult+strlen(queryResult), "Current tenant:%s\n", currentTenant->getUsername());
    }

}

bool Instance::addTenant(char *n, char *p, int a, int isAdmin){

    printf("Adding tenant \"%s\"...\n", n);
    if(tenantAmount == maxTenantAmount){
        maxTenantAmount *= 2;
        Tenant* tmp = new Tenant[maxTenantAmount];
        for(int i=0;i<tenantAmount;i++)
            tmp[i] = tenantList[i];
        tenantList = new Tenant[maxTenantAmount];
        for(int i=0;i<tenantAmount;i++)
            tenantList[i] = tmp[i];
    }
    tenantList[tenantAmount].setUsername(n);
    tenantList[tenantAmount].updatePassword(p);
    tenantList[tenantAmount].setAccount(a);
    if(isAdmin)
        tenantList[tenantAmount].upLevel();
    tenantAmount++;

    if(isPersistent){
        file->addTenant(n, p, a, isAdmin);
    }
    printf("Adding tenant done.\n");
    return true;
}

Tenant* Instance::getTenantByName(char *name){
    for(int i=0;i<tenantAmount;i++){
        if(!strcmp(tenantList[i].getUsername(), name)){
            return &tenantList[i];
        }
    }
    return NULL;
}

bool Instance::delTenant(char *name){
    printf("Deleting tenant \"%s\"...\n", name);
    Tenant* tenant = getTenantByName(name);
    if(tenantAmount!=0 && tenant!=NULL){
        tenant->copy(&tenantList[tenantAmount-1]);
        tenantList[tenantAmount-1].clear();
        tenantAmount--;

        strcpy(queryResult, "Success.");
        printf("%s", queryResult);
    }else{
        strcpy(queryResult, "Error:unkown tenant.");
        printf("%s", queryResult);
    }
    printf("Deleting tenant done.");
    return queryResult;
}

char* Instance::addDatabase(char *databaseTitle){

    printf("Creating database \"%s\"...\n", databaseTitle);

    if(databaseAmount == maxDatabaseAmount){
        maxDatabaseAmount *= 2;
        Database* tmp = new Database[maxDatabaseAmount];
        for(int i=0;i<databaseAmount;i++)
            tmp[i] = databaseList[i];
        databaseList = new Database[maxDatabaseAmount];
        for(int i=0;i<databaseAmount;i++)
            databaseList[i] = tmp[i];
    }

    databaseList[databaseAmount].setTitle(databaseTitle);
    databaseAmount++;

    sprintf(queryResult, "Done.");
    int i=strlen(queryResult);
    while(i!=200){
        queryResult[i] = ' ';
        i++;
    }
    printf("Creating database done.\n");
    return queryResult;

}

char* Instance::delDatabase(char *databaseTitle){
    printf("Deleting database \"%s\"...\n", databaseTitle);
    int index = getIndex(databaseTitle);
    if(databaseAmount!=0 && index!=-1){
        databaseList[index].copy(&databaseList[databaseAmount-1]);
        databaseList[databaseAmount-1].setTitle("");
        databaseList[databaseAmount-1].clear();
        databaseAmount--;
        this->currentDatabase = &databaseList[0];
        strcpy(queryResult, "Success.\n");
        printf("%s", queryResult);
    }else{
        strcpy(queryResult, "Error:unkown database.\n");
        printf("%s", queryResult);
    }
    printf("Deleting database done.\n");
    return queryResult;
}

int Instance::getIndex(char *databaseTitle){
    int i=0;
    for(;i<databaseAmount;i++){
        if(!strcmp(databaseTitle, databaseList[i].getTitle()))
            return i;
    }
    if(i==databaseAmount)
        return -1;
}

Database* Instance::getDatabaseByTitle(char *databaseTitle){
    int i=0;
    for(;i<databaseAmount;i++){
        if(!strcmp(databaseTitle, databaseList[i].getTitle()))
            return &databaseList[i];
    }
    if(i==databaseAmount)
        return NULL;
}

int Instance::getSize(){
    int sum = 0;
    for(int i=0;i<databaseAmount;i++){
        sum += databaseList[i].getSize();
    }
    for(int i=0;i<tenantAmount;i++){
        sum += tenantList[i].getSize();
    }
    sum += router.getSize();
    sum += router.getSize();
    return sum;
}

void* receiveCastThread(void* router){
    Router* fuck = (Router*)router;
    fuck->findingServer();
}

void* broadCastThread(void* instance){
    Instance* fuck = (Instance*)instance;
    while(1){
        char message[10];
        sprintf(message, "%d", fuck->getSize());
        fuck->getRouter()->broadCast(message);
    }
}

void Instance::run(){

    printf("Starting wanDB server at %s:%d...\n", ip_address, port);

    pthread_t id;
    int ret = pthread_create(&id, NULL, receiveCastThread, &router);
    if(ret!=0){
        printf ("Create pthread error!\n");
        exit (1);
    }

    ret = pthread_create(&id, NULL, broadCastThread, this);
    if(ret!=0){
        printf ("Create pthread2 error!\n");
        exit (1);
    }

    struct sockaddr_in my_addr, remote_addr;
    int sock_fd, client_fd;
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket initialization error!");
        exit(1);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(42391);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(my_addr.sin_zero), 8);
    if(bind(sock_fd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("bind error!");
        exit(1);
    }
    if(listen(sock_fd, 10) == -1){
        perror("listen error!");
        exit(1);
    }
    socklen_t sin_size;
    while(switchOn){
        sin_size = sizeof(struct sockaddr_in);
        if((client_fd = accept(sock_fd, (struct sockaddr*)&remote_addr, &sin_size)) == -1){
            perror("accept error!");
            exit(1);
        }
        printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));

        if(!vfork()){
            char buf[50];
            int bufLength;
            while(true){
                bufLength = recv(client_fd, buf, 50, 0);
                buf[bufLength] = '\0';
                if(bufLength > 0){
                    printf("received command:\"%s\"\n", buf);
                    if(buf[0] == '#'){
                        execute(buf+1);
                        break;
                    }
                    if(!strcmp(buf, "exit")){
                        break;
                    }else{
                        queryResult[0] = '\0';
                        execute(buf);
                        buf[strlen(buf)] = '\0';
                        send(client_fd, queryResult, strlen(queryResult), 0);
                    }
                }else{
                    perror("Client break");
                    exit(1);
                }
                sleep(1);
            }
            printf("Closed connection from %s.\n", inet_ntoa(remote_addr.sin_addr));
            close(client_fd);
            exit(0);
        }

        close(client_fd);
    }

    pthread_join(id,NULL);
}

void Instance::test(){

    pthread_t id;
    int ret = pthread_create(&id, NULL, receiveCastThread, &router);
    if(ret!=0){
        printf ("Create pthread error!\n");
        exit (1);
    }

    char command[50];
    while(strcmp(command, "exit")){
        gets(command);
        execute(command);
    }
}
