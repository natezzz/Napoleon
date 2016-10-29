#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "http.h"
#include "picohttpparser.h"

#define BUFSIZE 4096
#define MAXLINE 256
#define MAXHEADER 100

int
handle_http_request(int cfd)
{
  int err = 0;
  char buf[BUFSIZE];
  struct phr_header headers[MAXHEADER];
  char *method, *path;
  ssize_t recved;
  size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
  int pico_ret, minor_version;

  while (1) {
    recved = recv(cfd, buf + buflen, sizeof(buf) - buflen, 0);
    if (recved < 0) {
      return io_failure;
    }

    prevbuflen = buflen;
    buflen += recved;
    /* parser the request */
    num_headers = sizeof(headers) / sizeof(headers[0]);
    pico_ret = phr_parse_request(buf, buflen, &method, &method_len,
                                 &path, &path_len,
                                 &minor_version,
                                 headers,
                                 &num_headers, prevbuflen);
    if (pico_ret > 0)
      break;
    else if (pico_ret == -1)
      return parse_http_failure;
    /* else just loop */
  }

	return err;
}
