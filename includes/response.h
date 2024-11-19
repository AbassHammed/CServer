#ifndef RESPONSE_H
#define RESPONSE_H

#define BUFFER_SIZE 4096

void send_response(int client_fd, const char *status, const char *content_type, const char *body);

#endif // RESPONSE_H
