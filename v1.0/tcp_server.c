/*************************************************************************
	> File Name: tcp_server.c
	> Author: 
	> Mail: 
	> Created Time: 2021年03月16日 星期二 20时45分33秒
 ************************************************************************/


#include "head.h"

int socket_create_tcp(int port){
    int server_listen;
    if((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return -1;
    }
    printf("Socket created!\n");
    //暴力关闭 延时0
    struct linger m_linger;
    m_linger.l_onoff = 1;
    m_linger.l_linger = 0;
    if((setsockopt(server_listen, SOL_SOCKET, SO_LINGER, &m_linger, (socklen_t)sizeof(m_linger))) < 0){
        return -1;
    }
    //地址复用
    int flag = 1;
    if((setsockopt(server_listen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int))) < 0){
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    if((bind(server_listen, (struct sockaddr *)(&server), sizeof(server))) < 0){
        return -1;
    }
    printf("Socket bind!\n");
    if(listen(server_listen, 20) < 0){
        return -1;
    }

    printf("Socket listened!\n");
    return server_listen;
}
