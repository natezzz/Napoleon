#ifndef _EVENT_H_
#define _EVENT_H_

#include <ev.h>
#include "thread.h"

typedef struct
{
    ev_io watcher;
    threadpool_t *threadpool;
} napo_io;

typedef struct
{
	int client_fd;
} worker_arg_t;

void
init_event_loop(ev_io *watcher,
	            void (*cb)(struct ev_loop *loop, ev_io *watcher, int revents),
	            int fd);

void
accept_cb(struct ev_loop *loop, ev_io *sock_w, int revents);

#endif	
