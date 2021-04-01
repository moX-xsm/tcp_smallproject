/*************************************************************************
	> File Name: pthread_pool.c
	> Author: 
	> Mail: 
	> Created Time: 2021年04月01日 星期四 14时11分30秒
 ************************************************************************/

#include "../head/head.h"
#include "../head/pthread_pool.h"

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

