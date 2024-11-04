#include "socket.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

bool fileExists(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    bool exists = file != NULL;
    if (exists)
    {
        fclose(file);
    }
    return exists;
}

int accept_connection(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockeaddr *)&client_addr, &client_addr_len);

    if (client_fd < 0)
    {
        perror('Accept failed');
        return -1;
    }

    return client_fd;
}

void send_res(int client_fd, const char *status, const char *content_type, const char *body)
{
    char res[BUFFER_SIZE];
    snprintf(res, sizeof(res),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "\r\n"
             "%s",
             status, content_type, body);
    send(client_fd, res, strlen(res), 0);
}

int create_server_socket()
{
    int server_fd;
    struct sockaddr_in address;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        return -1;
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failed");
        return -1;
    }

    return server_fd;
}

void handle_client_request(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_fd < 0)
    {
        perror("Accept failed");
        return;
    }

    char buffer[BUFFER_SIZE] = {0};
    read(client_fd, buffer, BUFFER_SIZE);

    // Basic response
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>Hello, World!</h1></body></html>";

    send(client_fd, response, strlen(response), 0);
    close(client_fd);
}
