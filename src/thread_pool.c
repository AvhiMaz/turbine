#include "thread_pool.h"
#include "defines.h"
#include "shred.h"

void tp_init(ThreadPool *tp) {
    tp->head = 0;
    tp->tail = 0;
    tp->count = 0;
    tp->pending = 0;
    tp->shutdown = 0;
    pthread_mutex_init(&tp->mutex, NULL);
    pthread_cond_init(&tp->cond, NULL);
    pthread_cond_init(&tp->done_cond, NULL);
}

void tp_submit(ThreadPool *tp, Shred *s) {
    pthread_mutex_lock(&tp->mutex);
    tp->queue[tp->tail] = s;
    tp->tail = (tp->tail + 1) % MAX_QUEUE;
    tp->count++;
    tp->pending++;
    pthread_cond_signal(&tp->cond);
    pthread_mutex_unlock(&tp->mutex);
}
