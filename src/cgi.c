#include "cgi.h"
#include "response.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * @brief Vérifie si un script CGI est autorisé à s'exécuter.
 *
 * Cette fonction ouvre un fichier de configuration nommé "cgi_allowed.conf"
 * et vérifie si le chemin du script CGI fourni est présent dans ce fichier.
 * Si le chemin est trouvé dans le fichier, le script est considéré comme autorisé.
 *
 * @param script_path Le chemin du script CGI à vérifier.
 * @return true si le script est autorisé, false sinon.
 */
bool is_cgi_authorized(const char *script_path)
{
    // Ouvre le fichier de configuration en mode lecture
    FILE *allowlist = fopen("cgi_allowed.conf", "r");
    if (!allowlist)
    {
        // Si le fichier ne peut pas être ouvert, retourner false
        return false;
    }

    char line[256];
    // Lire chaque ligne du fichier
    while (fgets(line, sizeof(line), allowlist))
    {
        // Supprimer le caractère de nouvelle ligne à la fin de la ligne
        line[strcspn(line, "\n")] = '\0';
        // Comparer la ligne avec le chemin du script
        if (strcmp(line, script_path) == 0)
        {
            // Si le chemin est trouvé, fermer le fichier et retourner true
            fclose(allowlist);
            return true;
        }
    }

    // Fermer le fichier et retourner false si le chemin n'est pas trouvé
    fclose(allowlist);
    return false;
}

void handle_cgi_request(int client_fd, const char *script_path)
{
    if (!is_cgi_authorized(script_path))
    {
        send_response(client_fd, "403 Forbidden", "text/plain", "403 Accès Refusé");
        return;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        send_response(client_fd, "500 Internal Server Error", "text/plain", "500 Erreur Interne du Serveur");
        return;
    }

    const char *php_extension = ".php";
    size_t path_len = strlen(script_path);
    bool is_php_file = path_len >= strlen(php_extension) &&
                       strcmp(script_path + path_len - strlen(php_extension), php_extension) == 0;

    pid_t pid = fork();
    if (pid < 0)
    {
        send_response(client_fd, "500 Internal Server Error", "text/plain", "Fork failed");
        return;
    }

    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (is_php_file)
        {
            setenv("REQUEST_METHOD", "GET", 1);
            setenv("SCRIPT_NAME", script_path, 1);
            setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
            setenv("QUERY_STRING", "", 1);
            setenv("REMOTE_ADDR", "127.0.0.1", 1);
            setenv("SERVER_ADDR", "127.0.0.1", 1);
            setenv("SERVER_PORT", "8080", 1);
            setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
            setenv("CONTENT_TYPE", "", 1);
            setenv("CONTENT_LENGTH", "0", 1);

            execl("/usr/bin/php", "php", script_path, (char *)NULL);
        }
        else
        {
            execl("/usr/bin/python3", "python3", script_path, (char *)NULL);
        }
        perror("execl failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        close(pipefd[1]);
        char buffer[BUFFER_SIZE] = {0};
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);

        send_response(client_fd, "200 OK", "text/html", buffer);
    }
}
