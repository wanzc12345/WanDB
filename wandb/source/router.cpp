#include "../header/router.h"
#include <stdio.h>
#include <stdlib.h>
#include "sys/socket.h"
#include "sys/types.h"
#include <sys/ioctl.h>
#include "netinet/in.h"
#include <net/if.h>
#include "arpa/inet.h"
#include "unistd.h"
#include "string.h"
#include <netdb.h>
#include <pthread.h>

Router::Router()
{
    printf("Initiating router...\n");
    ipAmount = 0;
    maxIpAmount = 10;
    ipList = new char*[maxIpAmount];
    for(int i=0;i<maxIpAmount;i++)
        ipList[i] = new char[15];

    sock = -1;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket error\n");
        //return false;
    }

    const int opt = 1;
    //设置该套接字为广播类型，
    int nb = 0;
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if(nb == -1)
    {
        printf("set socket error...\n");
        //return false;
    }

    bzero(&addrto, sizeof(struct sockaddr_in));
    addrto.sin_family=AF_INET;
    addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
    addrto.sin_port=htons(12091);
    nlen=sizeof(addrto);

    // 广播地址
    bzero(&from, sizeof(struct sockaddr_in));
    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    from.sin_port = htons(12091);

    if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1)
    {
        printf("bind error...\n");
        //return -1;
    }

    minSize = 10000;
    minIndex = -1;
    getCurrentIp();

    printf("Initiating router done.\n");
}

char* Router::getCurrentIp(){
    //获取ip
    struct sockaddr_in sin;
    struct ifreq ifr;
    strncpy(ifr.ifr_name, "eth1", IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ   -   1]   =   0;
    if   (ioctl(sock,   SIOCGIFADDR,   &ifr)   ==  0)
    {
        memcpy(&sin,   &ifr.ifr_addr,   sizeof(sin));
        strcpy(currentIp, inet_ntoa(sin.sin_addr));
    }
    printf("currentIp: %s\n",   currentIp);
    return currentIp;
}

//bool Router::start(){
//    pthread_t id;
//    int ret = pthread_create(&id, NULL, findingServer, NULL);
//    if(ret!=0){
//        printf ("Create pthread error!\n");
//        exit (1);
//    }
//    broadCast();
//    pthread_join(id,NULL);
//}

Router::~Router(){
    for(int i=0;i<maxIpAmount;i++)
        delete [] ipList[i];
    delete [] ipList;
}

void Router::show(){
    printf("IP list:\n");
    for(int i=0;i<ipAmount;i++)
        printf("%d. %s\n", i+1, ipList[i]);
}

int Router::getSize(){
    return 15*maxIpAmount;
}

bool Router::addIpAddress(char *newIp, int size){
    printf("Router adding ip address...\n");
    if(getIndex(newIp) == -1){
        if(ipAmount == maxIpAmount){
            maxIpAmount *= 2;
            char** tmp = new char*[maxIpAmount];
            for(int i=0;i<maxIpAmount;i++)
                tmp[i] = new char[15];
            for(int i=0;i<ipAmount;i++)
                strcpy(tmp[i], ipList[i]);
            ipList = new char*[maxIpAmount];
            for(int i=0;i<maxIpAmount;i++)
                ipList[i] = new char[15];
            for(int i=0;i<ipAmount;i++)
                strcpy(ipList[i], tmp[i]);
        }
        strcpy(ipList[ipAmount], newIp);
        if(minSize>size){
            minSize = size;
            minIndex = ipAmount;
        }
        ipAmount++;
        printf("Router adding ip address done.\n");
        return true;
    }
    printf("Router adding ip address fail.\n");
    return false;
}

bool Router::deleteIpAddress(char *Ip){
    strcpy(ipList[getIndex(Ip)], ipList[ipAmount-1]);
    ipAmount--;
    return true;
}

char* Router::getMinIp(){
    return ipList[minIndex];
}

int Router::getMinSize(){
    return minSize;
}

int Router::getIndex(char *Ip){
    for(int i=0;i<ipAmount;i++)
        if(!strcmp(Ip, ipList[i]))
            return i;
    return -1;
}

bool Router::broadCast(char *message){

    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);

//    struct sockaddr_in from;
//    bzero(&from, sizeof(struct sockaddr_in));
//    from.sin_family = AF_INET;
//    from.sin_addr.s_addr = htonl(INADDR_ANY);
//    from.sin_port = htons(12091);
//    int flen=sizeof(from);

//    while(1)
//    {
        //从广播地址发送消息
        int ret=sendto(sock, message, strlen(message), 0, (sockaddr*)&addrto, nlen);
        if(ret<0)
        {
            printf("send error....%d\n", ret);
        }
        else
        {
            //printf("broadcast %s ok.\n", message);
        }
        sleep(2);
//    }
}

bool Router::sendMessage(char *ip, char *message){

    printf("Sending \"%s\" to \"%s\"...\n", message, ip);
        int sock_fd, recvbytes;
        char command[50];
        struct hostent *host;
        if(ip)
            host = gethostbyname(ip);
        else
            host = gethostbyname(getRandomIp());
        struct sockaddr_in serv_addr;

        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(42391);
        serv_addr.sin_addr = *((struct in_addr*)host->h_addr);
        bzero(&(serv_addr.sin_zero), 8);
        connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr));

        send(sock_fd, message, strlen(message), 0);

        close(sock_fd);
        printf("Sending \"%s\" to \"%s\" done.\n", message, ip);
}

bool Router::findingServer(){

        setvbuf(stdout, NULL, _IONBF, 0);
        fflush(stdout);

        char smsg[100];

        while(1)
        {
            //从广播地址接受消息
            int ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&from,(socklen_t*)&nlen);
            char* ip = inet_ntoa(from.sin_addr);
            if(ret<=0)
            {
                printf("read error....\n");
            }
            else
            {
                printf("receive \"%s\" from %s\n", smsg, ip);
                if(strcmp(ip,"localhost")&&strcmp(ip, "127.0.0.1")&&strcmp(ip, currentIp)&&getIndex(ip)==-1){
                    this->addIpAddress(ip, atoi(smsg));
                    this->show();
                }
            }

            sleep(1);
        }



    //return true;
}

char* Router::getRandomIp(){
    srand((unsigned int)time(0));
    int i = rand()%ipAmount;
    return ipList[1];
}
