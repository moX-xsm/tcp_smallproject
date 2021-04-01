/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2021年03月16日 星期二 21时30分38秒
 ************************************************************************/

#include "../head/head.h"
#include "../head/tcp_server.h"
#include "../head/pthread_pool.h"
#define MAXCLIENT 2//一次进程处理最大的线程1024
#define MAXTASK 50

//将发来的消息变为大写 在server接受完后发送回去
void chstr(char *str){
    for(int i = 0; i < strlen(str); i++){
        if(str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
    }
    return;
}


void work(int fd){
    //close(server_listen);//不能关闭！！线程用的都是一片储存空间 只有一个server_listen
    int sockfd = fd;
    if(send(sockfd, "You Are Here!", sizeof("You Are Here!"), 0) < 0){
        perror("send");
        close(sockfd);
        return ;
    }
    while(1){
        char msg[512] = {0};
        if(recv(sockfd, msg, sizeof(msg), 0) <= 0){
            break;
        }
        printf("%s\n", msg);
        chstr(msg);
        send(sockfd, msg, strlen(msg), 0);
    }
    close(sockfd);
    return ;
}

void *thread_run(void *arg){
    pthread_t tid = pthread_self();
    pthread_detach(tid);

    TaskQueue *queue = (TaskQueue *)arg;
    while(1){
        int fd = TaskQueuePop(queue);
        work(fd);
    }
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
    TaskQueue queue;
    TaskQueueInit(&queue, MAXTASK);

    pthread_t *tid = calloc(MAXCLIENT, sizeof(int));
    for(int i = 0; i < MAXCLIENT; i++){
        pthread_create(&tid[i], NULL, thread_run, (void *)&queue);
    }


    while(1){
        struct sockaddr_in client_info;
        socklen_t addrlen = sizeof(client_info);
        int sub;
        if((sockfd = accept(server_listen, (struct sockaddr *)(&client_info), &addrlen)) < 0){
            perror("accept");
            close(sockfd);
            continue;
        }
        TaskQueuePush(&queue, sockfd);
        printf("New client Pushed in TaskQueue!\n");
    }
    close(server_listen);
    

    return 0;
}
