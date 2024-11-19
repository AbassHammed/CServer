#include "response.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void send_response(int client_fd, const char *status, const char *content_type, const char *body)
{
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "\r\n"
             "%s",
             status, content_type, body);

    write(client_fd, response, strlen(response));
}
