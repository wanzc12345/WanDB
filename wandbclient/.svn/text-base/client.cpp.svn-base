#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <netdb.h>
#include <unistd.h>
#include <termios.h>

Client::Client()
{
    //printf("Initiating client...\n");
    ipAmount = 0;
    maxIpAmount = 10;
    ipList = new char*[maxIpAmount];
    for(int i=0;i<maxIpAmount;i++)
        ipList[i] = new char[15];
    //printf("Initiating client done.\n");
}

Client::~Client(){
    for(int i=0;i<maxIpAmount;i++)
        delete [] ipList[i];
    delete [] ipList;
}

void Client::show(){
    printf("IP list:\n");
    for(int i=0;i<ipAmount;i++)
        printf("%d. %s\n", i+1, ipList[i]);
}

bool Client::addIpAddress(char *newIp){
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
        ipAmount++;
        return true;
    }
    return false;
}

bool Client::deleteIpAddress(char *Ip){
    strcpy(ipList[getIndex(Ip)], ipList[ipAmount-1]);
    ipAmount--;
    return true;
}

int Client::getIndex(char *Ip){
    for(int i=0;i<ipAmount;i++)
        if(!strcmp(Ip, ipList[i]))
            return i;
    return -1;
}

bool Client::broadCast(char *message){
    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);

    int sock = -1;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket error\n");
        return false;
    }

    const int opt = 1;
    //设置该套接字为广播类型，
    int nb = 0;
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if(nb == -1)
    {
        printf("set socket error...\n");
        return false;
    }

    struct sockaddr_in addrto;
    bzero(&addrto, sizeof(struct sockaddr_in));
    addrto.sin_family=AF_INET;
    addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
    addrto.sin_port=htons(12091);
    int nlen=sizeof(addrto);

//    struct sockaddr_in from;
//    bzero(&from, sizeof(struct sockaddr_in));
//    from.sin_family = AF_INET;
//    from.sin_addr.s_addr = htonl(INADDR_ANY);
//    from.sin_port = htons(12091);
//    int flen=sizeof(from);

    while(1)
    {
        sleep(1);
        //从广播地址发送消息
        int ret=sendto(sock, message, strlen(message), 0, (sockaddr*)&addrto, nlen);
        if(ret<0)
        {
            printf("send error....%d\n", ret);
        }
        else
        {
            printf("ok ");
        }
    }
}

bool Client::run(char *ip){

    if(!ip||!strcmp(ip, "")){
        host = gethostbyname("localhost");
    }else
        host = gethostbyname(ip);

    char helpText[1500];
    strcpy(helpText, "");
    strcat(helpText, "List of all wanDB commands:\n");
    strcat(helpText, "create db/database <database_name>       create database\n");
    strcat(helpText, "create graph <graph_name> [<data>]       create graph\n");
    strcat(helpText, "delete/del <data> from <graph_name>      delete data from graph\n");
    strcat(helpText, "delete/del graph <graph_name>            delete graph\n");
    strcat(helpText, "delete/del db/database <database_name>   delete database\n");
    strcat(helpText, "exit                                     quit the shell\n");
    strcat(helpText, "help                                     help on commands\n");
    strcat(helpText, "insert <data> into <graph_name>          insert data into graph\n");
    strcat(helpText, "update <data> from <graph_name>          update data from graph\n");
    strcat(helpText, "use <database_name>                      set current database\n");
    strcat(helpText, "select <key> from <graph_name>           get node data by key\n");
    strcat(helpText, "show [dbs/databases]                     show database names\n");
    strcat(helpText, "show graphs                              show graphs in current database\n");
    strcat(helpText, "show graph <graph_name>                  prints out the the target graph info\n");
    strcat(helpText, "show tenants                             show all tenants\n");
    strcat(helpText, "show profile                             show most recent system\n");
    strcat(helpText, "show logs                                show the accessible logger names\n");
    strcat(helpText, "show size                                show the instance size\n\n");

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(42391);
    serv_addr.sin_addr = *((struct in_addr*)host->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
    if(connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr))==-1){
        printf("Cannot connect to %s!\n", host->h_name);
        exit(1);
    }

    char username[10], password[10];
    strcpy(username, getlogin());
    for(int j=0;j<3;j++){
    printf("\"%s\"", username);
    printf("password:");
    for(int i=0;i<10;i++)
        password[i] = '\0';
    char ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    fflush(stdin);
    while(true){
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        if((ch=getchar())=='\n'){
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            break;
        }
        password[strlen(password)] = ch;
        printf("*");
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    for(int i=0;i<50;i++)
        command[i] = '\0';
    strcpy(command, "login ");
    sprintf(command+strlen(command), "%s %s", username, password);
    send(sock_fd, command, strlen(command), 0);
    for(int i=0;i<200;i++)
        result[i] = '\0';
    strcpy(result, "");
    recv(sock_fd, result, 200, 0);
    if(!strcmp(result, "ok")){
        break;
    }else{
        printf("\n%s\n", result);
        if(j == 2){
            printf("You've tried too many times!\n");
            exit(1);
        }
    }
    }

    printf("%c[2J", 0x1b);
    printf("\nWelcome, %s! Thank you for using wanDB. Hope you like it!\n(Command should end with \";\", and type \"help\" for more information.)\n", username);

    while((strstr(command, "exit") == NULL)&&(strstr(command, "shutdown")==NULL)){
        strcpy(command, "");
        while(strchr(command, ';') == NULL){
            printf("wanDB>");
            gets(command+strlen(command));
            strcat(command, " ");
        }
        *strchr(command, ';') = '\0';
        if(strstr(command, "help")){
            printf("%s", helpText);
            continue;
        }
        send(sock_fd, command, strlen(command), 0);
        for(int i=0;i<200;i++)
            result[i] = '\0';
        //strcpy(result, "");
        if(recv(sock_fd, result, 200, 0)<0){
            perror("Receiving error");
            exit(1);
        }
        printf("%s\n", result);
    }

    close(sock_fd);

}
