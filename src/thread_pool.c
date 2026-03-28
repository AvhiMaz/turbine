#include "thread_pool.h"
#include "defines.h"
#include "shred.h"
#include <stdio.h>

void *worker(void *args) {
    ThreadPool *tp = (ThreadPool *)args;

    while (1) {
        pthread_mutex_lock(&tp->mutex);

        while (tp->count == 0 && !tp->shutdown) {
            pthread_cond_wait(&tp->cond, &tp->mutex);
        }
        if (tp->shutdown) {
            pthread_mutex_unlock(&tp->mutex);
            return NULL;
        }

        Shred *s = tp->queue[tp->head];

        tp->head = (tp->head + 1) % MAX_QUEUE;

        tp->count--;

        pthread_mutex_unlock(&tp->mutex);

        int ok = validate_shred(s);

        if (ok == 1) {
            printf("shred[%d] OK", s->index);
        } else {
            printf("shred[%d] CORRUPT", s->index);
        }

        pthread_mutex_lock(&tp->mutex);

        tp->pending--;

        if (tp->pending == 0) {
            pthread_cond_signal(&tp->done_cond);
        }
        pthread_mutex_unlock(&tp->mutex);
    }
}

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

void tp_start(ThreadPool *tp) {
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_create(&tp->thread[i], NULL, worker, tp);
    }
}

void tp_wait(ThreadPool *tp) {
    pthread_mutex_lock(&tp->mutex);
    while (tp->pending > 0)
        pthread_cond_wait(&tp->done_cond, &tp->mutex);
    pthread_mutex_unlock(&tp->mutex);
}

void tp_shutdown(ThreadPool *tp) {

    pthread_mutex_lock(&tp->mutex);
    tp->shutdown = 1;
    pthread_cond_broadcast(&tp->cond);
    pthread_mutex_unlock(&tp->mutex);

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(tp->thread[i], NULL);
    }
}
