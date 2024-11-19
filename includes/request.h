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

bool parse_request(const char *request_buffer, HttpRequest *request);

#endif // REQUEST_H
