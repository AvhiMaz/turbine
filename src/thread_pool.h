#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "shred.h"
#include <pthread.h>

typedef struct {
    pthread_t       thread[NUMBER_OF_THREADS];
    Shred          *queue[MAX_QUEUE];
    int             head, tail, count;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    pthread_cond_t  done_cond;
    int             pending;
    int             shutdown;

} ThreadPool;

void *worker(void *args);
void  tp_init(ThreadPool *tp);
void  tp_submit(ThreadPool *tp, Shred *s);
void  tp_start(ThreadPool *tp);
void  tp_wait(ThreadPool *tp);
void  tp_shutdown(ThreadPool *tp);

#endif
