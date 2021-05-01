#ifndef ROUTER_H
#define ROUTER_H
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"

class Client
{
private:
    char** ipList;
    int ipAmount;
    int maxIpAmount;

    int sock_fd, recvbytes;
    char command[50], result[200];
    struct hostent *host;
    struct sockaddr_in serv_addr;
public:
    Client();
    ~Client();
    bool addIpAddress(char* newIp);
    bool deleteIpAddress(char* Ip);
    int getIndex(char* Ip);
    void show();
    bool broadCast(char* message);
    bool run(char* ip=NULL);
};

#endif // ROUTER_H
