# v1.0

1. tcp流程图

   ![image-20210317103347149](https://gitee.com/xsm970228/images2020.9.5/raw/master/20210317103352.png)

2. server端

   - accept到一个链接就创建一个子进程处理，父进程一直accept
   - 子进程不停的接受数据放在msg中，收到数据就打印出来
   - 为了防止server端先断开连接而使端口处在TIME_WAIT状态，使用延时关闭（0s）和端口复用

3. client端

   建立tcp链接后不停的发送数据

# v2.0

将v1.0代码进行封装

- head：放头文件
- src：放源文件
- bin：放编译好的可执行文件
- makefile：使用make进行编译

# v3.0

用线程代替进程来执行任务

- server：当有新用户连接 打印"New client login!"并向对端发送"You Are Here"表示你已经连接 之后等待接受client发送过来的数据，接收到数据打印并将接收到的数据转化为大写后回传
- client: 当连接成功后接受server发送的"You Are Here"并打印，发送数据后接受server回传的大写数据并打印

bug:

```cpp
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

```

tid只有一个，当多个客户端连接时只有一个客户端会工作