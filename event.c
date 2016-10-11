#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <ev.h>

#include "event.h"
#include "thread.h"
#include "error.h"
#include "http.h"

void process_client(void *arg);

void
init_event_loop(ev_io *watcher,
	            void (*cb)(struct ev_loop *loop, ev_io *watcher, int revents),
	            int fd)
{
    struct ev_loop *loop = EV_DEFAULT;
    ev_io_init(watcher, cb, fd, EV_READ);
    ev_io_start(loop, watcher);
    ev_run(loop, 0);
}

void
accept_cb(struct ev_loop *loop, ev_io *sock_w, int revents)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addrlen;
    napo_io *io;

    if (EV_ERROR & revents) {
        perror("invalid event");
        return;
    }

    io = (napo_io *)sock_w;

    client_fd = accept(io->watcher.fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
        return;
    }

    worker_arg_t *arg = (worker_arg_t *)malloc(sizeof(worker_arg_t));
    if (arg == NULL) {
        die("malloc");
    }

    arg->client_fd = client_fd;

    if (threadpool_add_task(io->threadpool, process_client, (void *)arg) != 0) {
        die("add_task");
    }
}

void
process_client(void *arg)
{
    worker_arg_t *worker_arg;
    int client_fd;

    if (arg == NULL) {
        perror("No arg");
        return;
    }

    worker_arg = (worker_arg_t *)arg;
    client_fd = worker_arg->client_fd;
    printf("Client fd: %d\n", client_fd);
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    getpeername(client_fd, (struct sockaddr *)&addr, &addrlen);
    printf("Connected: %s\n", inet_ntoa(addr.sin_addr));

    if (handle_http_request(client_fd) < 0) {
        perror("failed handling http request.");
    }

    close(client_fd);
}