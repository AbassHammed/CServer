#ifndef SERVER_H
#define SERVER_H

#define MAX_CONNECTIONS 10

int create_server_socket(int port);
int accept_connection(int server_fd);

#endif // SERVER_H
