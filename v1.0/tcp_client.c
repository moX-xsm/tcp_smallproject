/*************************************************************************
	> File Name: tcp_client.c
	> Author: 
	> Mail: 
	> Created Time: 2021年03月17日 星期三 09时47分32秒
 ************************************************************************/

#include "head.h"

int socket_connect_tcp(char *host, int port){
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0){
        return -1;
    }
    printf("socket connected!\n");
    return sockfd;
    
}
