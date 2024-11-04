#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "src/socket.h"

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
        handle_client_request(server_fd);
    }

    close(server_fd);
    return 0;
}
