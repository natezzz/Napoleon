#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "http.h"

#define BUFSIZE 4096
#define MAXLINE 256

static ssize_t
read_line(int fd, char *buf, size_t buf_size)
{
	if (buf_size <= 0 ||fd == NULL) {
		errno = EINVAL;
		return -1;
	}

	char *buf_start = buf;
	static char *bp;
	static int cnt = 0;
	static char b[1500];
	char c;

	while (--buf_size > 0) {
		if (--cnt <= 0) {
			cnt = recv(fd, b, sizeof(b), 0);
			if (cnt < 0) {
				if (errno == EINTR) {
					buf_size++;   /* while loop will decrement */
					continue;
				}
				return -1;
			}

			if (cnt == 0) {
				return 0;
			}
			bp = b;
		}
		c = *(bp++);
		*(buf++) = c;
		if (c == '\n') {
			*(buf) = '\0';
			return buf - buf_start;
		}
	}
	errno = EMSGSIZE;
	return -1;
}

static http_request_t *
parse_http_request(cfd)
{
	char buf[BUFSIZE];  /* http buf */
	ssize_t read;

	char method[MAXLINE];
	char path[MAXLINE];
	char version[MAXLINE];

	http_request_t *req;


	read = read_line(cfd, buf, sizeof(buf));
	if (read <= 0) {
		return NULL;
	} else {
		sscanf(buf, "%s %s %s\r", method, path, version);
		req = (http_request_t *)(malloc(sizeof(http_request_t)));

		if (strcasecmp(method, "GET") == 0) {
			req->method = GET;
		} else if (strcasecmp(method, "POST") == 0) {
			req->method = POST;
		} else {
			req->method = UNSUPPORTED;
		}

		strncpy(req->path, path, strlen(path));
		return req;
	}
}

static void
delete_http_request(http_request_t *req)
{
	if (req == NULL)
		return;
	free(req);
}

int
handle_http_request(int cfd)
{
	http_request_t *req;
	int err = 0;

	req = parse_http_request(cfd);
	if (req == NULL) {
		return parse_http_failure;
	}

	/* test */
	switch (req->method) {
		case GET:
			printf("Method: %s\n", "get");
			break;
		case POST:
			printf("Method: %s\n", "post");
			break;
		default:
			printf("Method: UNSUPPORTED\n");
	}
	printf("Path: %s\n", req->path);

	
	delete_http_request(req);

	return err;
}