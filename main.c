#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "src/socket.h"
#include <sys/types.h>

int main()
{
    int server_fd = create_server_socket();
    if (server_fd < 0)
    {
        fprintf(stderr, "Failed to create server socket\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1)
    {
        int client_fd = accept_connection(server_fd);
        if (client_fd < 0)
        {
            fprintf(stderr, "Failed to accept connection\n");
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            close(server_fd);
            handle_client_request(client_fd);
            close(client_fd);
            exit(0);
        }
        else if (pid > 0)
        {
            close(client_fd);
        }
        else
        {
            fprintf(stderr, "Failed to fork\n");
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
