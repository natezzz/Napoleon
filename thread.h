#ifndef _THREAD_H_
#define _THREAD_H_

#include "constants.h"

#define MAX_THREADS 64
#define MAX_QUEUE 65536

typedef struct threadpool_t threadpool_t;

typedef enum
{
	threadpool_invalid = -1,
	threadpool_queue_full = -2,
	threadpool_lock_failure = -3
} threadpool_error_t;

threadpool_t *threadpool_create(int thread_count, int queue_size);
int threadpool_add_task(threadpool_t *pool, void (*func)(void*), void *arg);
void threadpool_join(threadpool_t *pool);

#endif
