#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include "includes/server.h"
#include "includes/request.h"
#include "includes/response.h"
#include "includes/cgi.h"
#include "includes/files.h"
#include "includes/errors.h"

#define DEFAULT_PORT 8080
#define DOCUMENT_ROOT "."

int server_fd;

// Gestionnaire pour le signal SIGINT (Ctrl+C)
void handle_sigint()
{
    printf("\nArrêt du serveur...\n");
    close(server_fd);
    exit(0);
}

// Gestion de la requête client
void handle_client_request(int client_fd)
{
    char buffer[BUFFER_SIZE] = {0};
    read(client_fd, buffer, sizeof(buffer));

    // Analyse de la requête HTTP
    Request req = parse_http_request(buffer);
    print_request(&req);

    if (!check_request(req))
    {
        // Réponse en cas de requête incorrecte
        send_response(client_fd, "400 Bad Request", "text/plain", "400 Requête Incorrecte");
        return;
    }

    // Gestion des scripts CGI
    if (strncmp(req.path, "/cgi-bin/", 9) == 0)
    {
        handle_cgi_request(client_fd, req.path + 1);
        return;
    }

    // Servir les fichiers statiques
    char file_path[BUFFER_SIZE];
    snprintf(file_path, sizeof(file_path), "%s%s", DOCUMENT_ROOT, req.path);

    if (file_exists(file_path))
    {
        char *content = read_file_content(file_path);
        if (!content)
        {
            // Réponse en cas d'erreur interne du serveur
            send_response(client_fd, "500 Internal Server Error", "text/plain", get_error_message(ERROR_500));
        }
        else
        {
            // Réponse avec le contenu du fichier
            send_response(client_fd, "200 OK", "text/html", content);
            free(content);
        }
    }
    else
    {
        // Réponse en cas de fichier non trouvé
        send_response(client_fd, "404 Not Found", "text/plain", get_error_message(ERROR_404));
    }
}

int main()
{
    signal(SIGINT, handle_sigint);

    // Load configuration
    printf("Starting server on port %d...\n", DEFAULT_PORT);

    // Create server socket
    server_fd = create_server_socket(DEFAULT_PORT);
    if (server_fd < 0)
    {
        fprintf(stderr, "Échec de la création du socket serveur\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on http://localhost:%d\n", DEFAULT_PORT);

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
