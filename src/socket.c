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
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_fd < 0)
    {
        perror("Accept failed");
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

void handle_cgi_request(int client_fd, const char *script_path)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        send_res(client_fd, "500 Internal Server Error", "text/plain", "Internal Server Error");
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        send_res(client_fd, "500 Internal Server Error", "text/plain", "Fork failed");
        return;
    }

    if (pid == 0)
    {
        close(pipefd[0]);

        setenv("REQUEST_METHOD", "GET", 1);
        setenv("SCRIPT_NAME", script_path, 1);
        setenv("QUERY_STRING", "", 1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execl(script_path, script_path, NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipefd[1]);

        char buffer[BUFFER_SIZE] = {0};
        read(pipefd[0], buffer, BUFFER_SIZE);
        close(pipefd[0]);

        send_res(client_fd, "200 OK", "text/html", buffer);
    }
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

void handle_client_request(int client_fd)
{
    char buffer[BUFFER_SIZE] = {0};
    read(client_fd, buffer, BUFFER_SIZE);

    char method[5], path[BUFFER_SIZE];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(method, "GET") != 0)
    {
        send_res(client_fd, "405 Method Not Allowed", "text/plain", "Method Not Allowed");
        return;
    }

    if (strncmp(path, "/cgi-bin/", 9) == 0)
    {
        printf("Something here is wierd %s\n", path);
        handle_cgi_request(client_fd, path + 1);
        return;
    }

    char *file_path = path + 1;

    if (strlen(file_path) == 0)
    {
        file_path = "index.html";
    }

    if (fileExists(file_path))
    {
        FILE *file = fopen(file_path, "r");
        if (file == NULL)
        {
            send_res(client_fd, "500 Internal Server Error", "text/plain", "Internal Server Error");
            return;
        }

        char *line = NULL;
        size_t len = 0;
        char body[BUFFER_SIZE] = {0};

        while (getline(&line, &len, file) != -1)
        {
            strcat(body, line);
        }

        send_res(client_fd, "200 OK", "text/html", body);
        fclose(file);
    }
    else
    {
        send_res(client_fd, "404 Not Found", "text/plain", "Not Found");
    }
}
