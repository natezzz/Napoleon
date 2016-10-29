#ifndef _HTTP_H_
#define _HTTP_H_

typedef enum
{
	parse_http_failure = -1,
  io_failure = -2
} http_error;

int handle_http_request(int cfd);

#endif
