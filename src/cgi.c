#include "cgi.h"
#include "response.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

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
    // Vérifie si le script CGI est autorisé
    if (!is_cgi_authorized(script_path))
    {
        // Si non autorisé, envoie une réponse 403 Forbidden
        send_response(client_fd, "403 Forbidden", "text/plain", "403 Accès Refusé");
        return;
    }

    int pipefd[2];
    // Crée un pipe pour la communication entre processus
    if (pipe(pipefd) == -1)
    {
        // Si la création du pipe échoue, envoie une réponse 500 Internal Server Error
        send_response(client_fd, "500 Internal Server Error", "text/plain", "500 Erreur Interne du Serveur");
        return;
    }

    const char *php_extension = ".php";
    size_t path_len = strlen(script_path);
    // Vérifie si le fichier est un script PHP
    bool is_php_file = path_len >= strlen(php_extension) &&
                       strcmp(script_path + path_len - strlen(php_extension), php_extension) == 0;

    pid_t pid = fork();
    // Crée un processus enfant
    if (pid < 0)
    {
        // Si la création du processus échoue, envoie une réponse 500 Internal Server Error
        send_response(client_fd, "500 Internal Server Error", "text/plain", "Fork failed");
        return;
    }

    if (pid == 0)
    {
        // Processus enfant
        close(pipefd[0]);               // Ferme l'extrémité de lecture du pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirige la sortie standard vers l'extrémité d'écriture du pipe
        close(pipefd[1]);               // Ferme l'extrémité d'écriture du pipe

        // Exécute le script PHP ou Python selon le type de fichier
        if (is_php_file)
        {
            execl("/usr/bin/php", "php", script_path, (char *)NULL);
        }
        else
        {
            execl("/usr/bin/python3", "python3", script_path, (char *)NULL);
        }
        // Si execl échoue, affiche une erreur et termine le processus
        perror("execl failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Processus parent
        close(pipefd[1]); // Ferme l'extrémité d'écriture du pipe
        char buffer[BUFFER_SIZE] = {0};
        // Lit la sortie du script depuis l'extrémité de lecture du pipe
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]); // Ferme l'extrémité de lecture du pipe

        // Envoie la réponse avec le contenu du script exécuté
        send_response(client_fd, "200 OK", "text/html", buffer);
    }
}
