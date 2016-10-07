#include <stdlib.h>
#include <pthread.h>

#include "thread.h"
#include "error.h"

typedef struct {
	void (*func)(void *);
	void *arg;
} threadpool_task_t;

struct threadpool_t {
	pthread_t *threads;
	threadpool_task_t *queue;
	pthread_mutex_t lock;
	pthread_cond_t notify;
	int thread_count;
	int q_size;
	int q_count;
	int q_head;
	int q_tail;
	int started;
};

static void *thread_start(void *threadpool);

threadpool_t *
threadpool_create(int thread_count, int queue_size)
{
	threadpool_t *pool;
	int i;

	if (thread_count <= 0 || thread_count > MAX_THREADS ||
		queue_size   <= 0 || queue_size   > MAX_QUEUE) {
		return NULL;
	}

	if ((pool = (threadpool_t *) malloc(sizeof(threadpool_t))) == NULL) {
		die("malloc");
	}

	pool->thread_count = 0;
	pool->started = 0;
	pool->q_size = queue_size;
	pool->q_head = pool->q_tail = 0;
	pool->q_count = 0;

	pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * thread_count);
	if (pool->threads == NULL) {
		die("malloc");
	}

	pool->queue = (threadpool_task_t *) malloc(sizeof(threadpool_task_t) * queue_size);

	if ((pthread_mutex_init(&(pool->lock), NULL) != 0) ||
		(pthread_cond_init(&(pool->notify), NULL) != 0)) {
		die("mutex");
	}

	/* start threads */
	for (i = 0; i < thread_count; i++) {
		if (pthread_create(&(pool->threads[i]), NULL, thread_start, (void *)pool) != 0) {
			die("pthread_create");
		}
		pool->thread_count++;
		pool->started++;
	}

	return pool;
}

int
threadpool_add_task(threadpool_t *pool, void (*func)(void*), void *arg)
{
	int next;
	int err = 0;

	if (pool == NULL || func == NULL) {
		return threadpool_invalid;
	}

	if (pthread_mutex_lock(&(pool->lock)) != 0) {
		return threadpool_lock_failure;
	}

	next = (pool->q_tail + 1) % pool->q_size;

	do {
		if (pool->q_count == pool->q_size) {
			err = threadpool_queue_full;
			break;
		}

		pool->queue[pool->q_tail].func = func;
		pool->queue[pool->q_tail].arg  = arg;
		pool->q_tail = next;
		pool->q_count += 1;

		if (pthread_cond_signal(&(pool->notify)) != 0) {
			err = threadpool_lock_failure;
			break;
		}
	} while(0);

	if (pthread_mutex_unlock(&(pool->lock)) != 0) {
		err = threadpool_lock_failure;
	}

	return err;
}

void
threadpool_join(threadpool_t *pool)
{
	int i;
	for (i = 0; i < pool->thread_count; i++) {
		pthread_join(pool->threads[i], NULL);
	}
}


static void *
thread_start(void *threadpool)
{
	threadpool_t *pool = (threadpool_t *)threadpool;
	threadpool_task_t task;

	for (;;) {
		pthread_mutex_lock(&(pool->lock));

		while ((pool->q_count == 0)) {
			pthread_cond_wait(&(pool->notify), &(pool->lock));
		}

		task.func = pool->queue[pool->q_head].func;
		task.arg  = pool->queue[pool->q_head].arg;
		pool->q_head = (pool->q_head + 1) % pool->q_size;
		pool->q_count -= 1;

		pthread_mutex_unlock(&(pool->lock));

		(*(task.func))(task.arg);
	}

	pool->started--;
	return(NULL);
}