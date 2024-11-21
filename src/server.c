#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int create_server_socket(int port)
{
    int server_fd;
    struct sockaddr_in address;

    // Create a socket file descriptor for the server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Failed to create socket");
        return -1;
    }

    // Set up the address structure for the server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
    address.sin_port = htons(port);       // Convert port number to network byte order

    // Bind the socket to the specified port so the server can listen for incoming connections
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Failed to bind socket");
        close(server_fd);
        return -1;
    }

    // we limit the number of simultaenous connection to avoid overheading the memory (probably would never happen)
    // but in a real life situation we will have something like load balancer in place
    if (listen(server_fd, MAX_CONNECTIONS) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int accept_connection(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_fd < 0)
    {
        perror("Accept failed");
    }

    return client_fd;
}
