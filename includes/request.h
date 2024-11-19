#ifndef REQUEST_H
#define REQUEST_H

#include <stdbool.h>

#define BUFFER_SIZE 4096

typedef struct
{
    char method[16];
    char path[256];
    char protocol[16];
} HttpRequest;

typedef struct
{
    char method[8];
    char path[256];
    char host[256];
    char user_agent[512];
    char accept[256];
} Request;

bool parse_request(const char *request_buffer, HttpRequest *request);
Request parse_http_request(const char *request);
void print_request(const Request *req);

#endif // REQUEST_H
