#ifndef _HTTP_H_
#define _HTTP_H_

typedef enum
{
	GET,
	POST,
	UNSUPPORTED
} http_method_t;

typedef struct
{
	http_method_t method;
	char path[512];   /* requested file */
} http_request_t;

typedef enum
{
	parse_http_failure = -1
} http_error;

int handle_http_request(int cfd);

#endif
