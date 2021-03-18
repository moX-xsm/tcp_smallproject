/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2021年03月17日 星期三 09时53分33秒
 ************************************************************************/

#include "../head/head.h"
#include "../head/tcp_client.h"

int main(int argc, char **argv){
    if(argc != 3){
        fprintf(stderr, "Usage : %s ip port\n", argv[0]);
        return -1;
    }
    int port, sockfd;
    port = atoi(argv[2]);
    
    if((sockfd = socket_connect_tcp(argv[1], port)) == -1){
        perror("socket connect");
        return -1;
    }
    char msg[20];
    while(1){
        memset(msg, 0, sizeof(msg));
        scanf("%s", msg);
        if(send(sockfd, msg, sizeof(msg), 0) < 0){
            perror("send");
            continue;
        }
    }
    close(sockfd);
    return 0;
}
