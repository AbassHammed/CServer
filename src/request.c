#include "request.h"
#include <stdio.h>
#include <string.h>

bool parse_request(const char *request_buffer, HttpRequest *request)
{
    if (sscanf(request_buffer, "%s %s %s", request->method, request->path, request->protocol) != 3)
    {
        return false;
    }

    // Validate HTTP method
    if (strcmp(request->method, "GET") != 0)
    {
        return false;
    }

    // Ensure path is within reasonable limits
    if (strlen(request->path) >= sizeof(request->path))
    {
        return false;
    }

    return true;
}
