#include "request.h"
#include <stdio.h>
#include <string.h>

bool check_request(Request request)
{

    // Check if the method is GET
    if (strcmp(request.method, "GET") != 0)
    {
        return false; // Only GET method is supported
    }

    // Check if the path length is within the allowed limit
    if (strlen(request.path) >= sizeof(request.path))
    {
        return false; // Path is too long
    }

    return true;
}

// Function to parse the HTTP request and extract headers
Request parse_http_request(const char *request)
{
    Request parsed_request = {0};

    // Initialize default values for headers
    strcpy(parsed_request.host, "N/A");
    strcpy(parsed_request.user_agent, "N/A");
    strcpy(parsed_request.accept, "N/A");

    // Parse the request line (e.g., "GET /path HTTP/1.1")
    sscanf(request, "%s %s", parsed_request.method, parsed_request.path);

    // Look for specific headers in the request
    const char *host_header = strstr(request, "Host: ");
    const char *user_agent_header = strstr(request, "User-Agent: ");
    const char *accept_header = strstr(request, "Accept: ");

    // Extract the Host header if present
    if (host_header)
        sscanf(host_header, "Host: %255[^\r\n]", parsed_request.host);

    // Extract the User-Agent header if present
    if (user_agent_header)
        sscanf(user_agent_header, "User-Agent: %511[^\r\n]", parsed_request.user_agent);

    // Extract the Accept header if present
    if (accept_header)
        sscanf(accept_header, "Accept: %255[^\r\n]", parsed_request.accept);

    return parsed_request; // Return the parsed request structure
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
