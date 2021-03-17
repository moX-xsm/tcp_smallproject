/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2021年03月16日 星期二 21时30分38秒
 ************************************************************************/

#include "./head.h"
#include "./tcp_server.h"

int main(int argc, char **argv){
    
    int server_listen, sockfd, port;
    if(argc != 2){
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        return -1;
    }
    port = atoi(argv[1]);
    if((server_listen = socket_create_tcp(port)) == -1){
        perror("Creat socket failed!");
        return -1;
    }
    signal(SIGCHLD, SIG_IGN);
    while(1){
        struct sockaddr_in client;
        socklen_t addrlen = sizeof(client);
        if((sockfd = accept(server_listen, (struct sockaddr *)(&client), &addrlen)) < 0){
            perror("accept");
            close(sockfd);
            continue;
        }
        pid_t pid;
        if((pid = fork()) < 0){
            perror("fork");
            continue;
        }
        if(pid == 0){
            close(server_listen);
            char msg[20] = {0};
            while(1){
                memset(msg, 0, sizeof(msg));
                if(recv(sockfd, msg, sizeof(msg), 0) <= 0){
                    perror("recv");
                    close(sockfd);
                    exit(0);
                }
                printf("[%s : %d]msg = %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), msg);
            }
            exit(0);
        }
    }
    close(server_listen);
    

    return 0;
}
