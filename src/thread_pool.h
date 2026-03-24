#include "shred.h"
#include <pthread.h>

typedef struct {
    pthread_t thread[NUMBER_OF_THREADS];
    Shred    *queue[MAX_QUEUE];
} ThreadPool;
