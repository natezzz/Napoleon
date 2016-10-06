#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <ev.h>

#include "event.h"

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
accept_cb(struct ev_loop *loop, ev_io *sock_watcher, int revents)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addrlen;

    if (EV_ERROR & revents) {
        perror("invalid event");
        return;
    }

    client_fd = accept(sock_watcher->fd,
                       (struct sockaddr *) &client_addr,
                       &addrlen);
    if (client_fd == -1) {
        perror("accept");
        return;
    }

    printf("Connected: %s\n", inet_ntoa(client_addr.sin_addr));
}