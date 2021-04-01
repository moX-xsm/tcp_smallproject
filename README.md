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

多个client会有竞争

# v4.0

用数组处理多个客户端的竞争问题

一个程序最多能处理的线程是1024

```c
#define MAXCLIENT 512//一次进程处理最大的线程1024

struct Client{
    int flag;
    int fd;
    pthread_t tid;
};

struct Client *client;

//寻找最小的可用的客户端下标
int find_sub(){
    for(int i = 0; i < MAXCLIENT; i++){
        if(client[i].flag == 0){
            return i;
        }
    }
    return -1;
}

		int sub;
        if((sub = find_sub()) < 0){
            fprintf(stderr, "Client Full\n");
            close(sockfd);
            continue;
        }
        client[sub].flag = 1;
        client[sub].fd = sockfd;
        pthread_create(&client[sub].tid, NULL, work, (void *)&sub);


```

# v5.0

使用线程池改进程序

之前的每来一个任务新创建一个线程，类似公司每有一个任务我就重新招聘一个人，用完了踢走，不合理

线程池：来任务了就进任务队列，每个线程认领一个任务，做完任务之后再拿下一个任务，直至全部任务完成

![image-20210326104656056](https://gitee.com/xsm970228/images2020.9.5/raw/master/20210326104658.png)

- 可能会有多个线程抢一个任务：需要做互斥
- 当有任务来时需要告知所有人：条件变量

任务队列 + 线程池

```c
//任务队列结构操作和结构定义
typedef struct{
    int sum;
    int *fd;
    int head, tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TaskQueue;

void TaskQueueInit(TaskQueue *queue, int sum){
    queue->sum = sum;
    queue->fd = calloc(sum, sizeof(int));
    queue->head = queue->tail = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

void TaskQueuePush(TaskQueue *queue, int fd){
    pthread_mutex_lock(&queue->mutex);
    queue->fd[queue->tail] = fd;
    
    if(++queue->tail == queue->sum) queue->tail = 0;
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

int TaskQueuePop(TaskQueue *queue){
    pthread_mutex_lock(&queue->mutex);
    while(queue->tail == queue->head){
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }
    int fd = queue->fd[queue->head];
    if(++queue->head == queue->sum) queue->head = 0;
    pthread_mutex_unlock(&queue->mutex);
    return fd;
}

//主函数在进行死循环accept之前需要建立任务队列和线程池
TaskQueue queue;
    TaskQueueInit(&queue, MAXTASK);

    pthread_t *tid = calloc(MAXCLIENT, sizeof(int));
    for(int i = 0; i < MAXCLIENT; i++){
        pthread_create(&tid[i], NULL, thread_run, (void *)&queue);
    }
//每次accept一个新的连接fd，需要将其压入任务队列
```

