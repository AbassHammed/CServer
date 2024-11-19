#include "request.h"
#include <stdio.h>
#include <string.h>

bool parse_request(const char *request_buffer, HttpRequest *request)
{
    if (sscanf(request_buffer, "%s %s %s", request->method, request->path, request->protocol) != 3)
    {
        return false;
    }

    if (strcmp(request->method, "GET") != 0)
    {
        return false;
    }

    if (strlen(request->path) >= sizeof(request->path))
    {
        return false;
    }

    return true;
}

Request parse_http_request(const char *request)
{
    Request parsed_request = {0};

    strcpy(parsed_request.host, "N/A");
    strcpy(parsed_request.user_agent, "N/A");
    strcpy(parsed_request.accept, "N/A");

    sscanf(request, "%s %s", parsed_request.method, parsed_request.path);

    const char *host_header = strstr(request, "Host: ");
    const char *user_agent_header = strstr(request, "User-Agent: ");
    const char *accept_header = strstr(request, "Accept: ");

    if (host_header)
        sscanf(host_header, "Host: %255[^\r\n]", parsed_request.host);

    if (user_agent_header)
        sscanf(user_agent_header, "User-Agent: %511[^\r\n]", parsed_request.user_agent);

    if (accept_header)
        sscanf(accept_header, "Accept: %255[^\r\n]", parsed_request.accept);

    return parsed_request;
}

// Function to print the parsed request
void print_request(const Request *req)
{
    printf("Requete recue:\n");
    printf("Method: %s\n", req->method);
    printf("Path: %s\n", req->path);
    printf("Host: %s\n", req->host);
    printf("User-Agent: %s\n", req->user_agent);
    printf("Accept: %s\n", req->accept);
}
