#include "thread_pool.h"

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
