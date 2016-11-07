#ifndef RESPONSE_H
#define RESPONSE_H

typedef enum {
  OK, /* 200 */
  NOT_FOUND, /* 404 */
  NOT_IMPLEMENTED /* 501 */
} response_status_t;

/* response headers */
static char ok_response[] =
  "HTTP/1.1 200 OK\r\n"
  "Content-type: text/html\r\n"
  "Server: Napoleon\r\n"
  "\r\n";

static char not_found_response[] =
  "HTTP/1.1 404 Not Found\r\n"
  "Content-type: text/html\r\n"
  "Server: Napoleon\r\n"
  "\r\n";

static char not_implemented_response[] =
  "HTTP/1.1 501 Not Implemented\r\n"
  "Content-type: text/html\r\n"
  "Server: Napoleon\r\n"
  "\r\n";

int respond_to(int cfd, const char* method, int method_len, const char* path, int path_len);

static void send_header(int fd, response_status_t stat);
static int do_get(int cfd, const char* path, int path_len);
static int do_unimplemented(int cfd);

#endif /* RESPONSE_H */
