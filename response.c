#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "response.h"

int
respond_to(int cfd, const char *method, int method_len, const char *path, int path_len)
{
  /* we only support GET method for now */
  if (strncasecmp(method, "GET", method_len) == 0) {
    return do_get(cfd, path, path_len);
  } else {
    return do_unimplemented(cfd);
  }
}

static void
send_header(int fd, response_status_t stat)
{
  switch (stat) {
  case OK:
    send(fd, ok_response, sizeof(ok_response), 0);
    break;
  case NOT_FOUND:
    send(fd, not_found_response, sizeof(not_found_response), 0);
    break;
  case NOT_IMPLEMENTED:
    send(fd, not_implemented_response, sizeof(not_implemented_response), 0);
    break;
  }
}

static int
do_get(int cfd, const char *path, int path_len)
{
  send_header(cfd, OK);

  return 0;
}

static int
do_unimplemented(int cfd)
{
  send_header(cfd, NOT_IMPLEMENTED);
  return 0;
}
