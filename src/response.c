#include "response.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void send_response(int client_fd, const char *status, const char *content_type, const char *body)
{
    char response[BUFFER_SIZE];
    // Prépare la réponse HTTP avec le statut, le type de contenu et le corps
    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "\r\n"
             "%s",
             status, content_type, body);

    // Envoie la réponse au client
    write(client_fd, response, strlen(response));
}
