#ifndef REQUEST_H
#define REQUEST_H

#include <stdbool.h>

#define BUFFER_SIZE 4096

// Définition de la structure Request
typedef struct
{
    char method[8];       // Méthode HTTP (GET, POST, etc.)
    char path[256];       // Chemin de la ressource demandée
    char host[256];       // Hôte de la requête
    char user_agent[512]; // (navigateur, client HTTP, etc.)
    char accept[256];     // Types de contenu acceptés par le client
} Request;

bool check_request(Request request);
Request parse_http_request(const char *request);
void print_request(const Request *req);

#endif // REQUEST_H
