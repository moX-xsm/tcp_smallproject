/*************************************************************************
	> File Name: pthread_pool.h
	> Author: 
	> Mail: 
	> Created Time: 2021年04月01日 星期四 14时22分08秒
 ************************************************************************/

#ifndef _PTHREAD_POOL_H
#define _PTHREAD_POOL_H
typedef struct{
    int sum;
    int *fd;
    int head, tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TaskQueue;
void TaskQueueInit(TaskQueue *queue, int sum);
void TaskQueuePush(TaskQueue *, int);
int TaskQueuePop(TaskQueue *);

#endif
