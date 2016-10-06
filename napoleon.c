#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <ev.h>

#include "napoleon.h"
#include "error.h"
#include "event.h"
#include "thread.h"

static int sock_fd;
static struct sockaddr_in addr;

static ev_io sock_watcher;
static threadpool_t *pool;

void
init_listen_sock(void)
{
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        die("socket");
    }

    int one = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == -1) {
        die("setsockopt");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (inet_aton(HOST, &addr.sin_addr) == 0) {
        die("inet_aton");
    }

    if (bind(sock_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        die("bind");
    }

    if (listen(sock_fd, BACKLOG) == -1) {
        die("listen");
    }
}

int
main(int argc, char const *argv[])
{
    init_listen_sock();
    pool = threadpool_create(THREAD_NUM, QUEUE_NUM);
    init_event_loop(&sock_watcher, accept_cb, sock_fd);

    close(sock_fd);
    printf("Interrupted: closed\n");

    return 0;
}
