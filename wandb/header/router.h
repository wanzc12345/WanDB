#ifndef ROUTER_H
#define ROUTER_H
#include "netinet/in.h"

class Router
{
private:
    char** ipList;
    int minSize;
    int minIndex;
    int ipAmount;
    int maxIpAmount;
    char currentIp[15];

    int sock;
    struct sockaddr_in addrto;
    struct sockaddr_in from;
    int nlen;
public:
    Router();
    ~Router();
    bool addIpAddress(char* newIp, int size);
    bool deleteIpAddress(char* Ip);
    int getIndex(char* Ip);
    int getSize();
    void show();
    bool broadCast(char* message="ping");
    bool sendMessage(char* message, char* ip);
    bool receiveMessage(char* result, char* ip);
    char* getRandomIp();
    char* getMinIp();
    int getMinSize();
    bool findingServer();
    char* getCurrentIp();
    int getIpAmount(){return ipAmount;}
};

#endif // ROUTER_H
