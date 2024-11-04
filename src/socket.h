#ifndef SOCKET_H
#define SOCKET_H

#define PORT 8080
#define BUFFER_SIZE 1024

int create_server_socket();
void handle_client_request(int server_fd);
int accept_connection(int server_fd);

#endif
