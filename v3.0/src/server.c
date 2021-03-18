/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2021年03月16日 星期二 21时30分38秒
 ************************************************************************/

#include "../head/head.h"
#include "../head/tcp_server.h"

//将发来的消息变为大写 在server接受完后发送回去
void chstr(char *str){
    for(int i = 0; i < strlen(str); i++){
        if(str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
    }
    return;
}


void *work(void *arg){
    //close(server_listen);//不能关闭！！线程用的都是一片储存空间 只有一个server_listen
    int *sockfd = (int *)arg;
    if(send(*sockfd, "You Are Here!", sizeof("You Are Here!"), 0) < 0){
        perror("send");
        close(*sockfd);
        return NULL;
    }
    while(1){
        char msg[512] = {0};
        if(recv(*sockfd, msg, sizeof(msg), 0) <= 0){
            break;
        }
        printf("%s\n", msg);
        chstr(msg);
        send(*sockfd, msg, strlen(msg), 0);
    }
    close(*sockfd);
    return NULL;
}

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
    pthread_t tid;
    while(1){
        struct sockaddr_in client;
        socklen_t addrlen = sizeof(client);
        if((sockfd = accept(server_listen, (struct sockaddr *)(&client), &addrlen)) < 0){
            perror("accept");
            close(sockfd);
            continue;
        }
        printf("New client login!\n");
        pthread_create(&tid, NULL, work, (void *)&sockfd);
    }
    close(server_listen);
    

    return 0;
}
