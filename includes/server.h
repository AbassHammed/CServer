#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define MAX_CONNECTIONS 10
#define CONFIG_FILE "server.conf"

int create_server_socket(int port);
int accept_connection(int server_fd);

#endif // SERVER_H
