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
    printf("%s\n", script_path);
    FILE *allowlist = fopen("cgi_allowed.conf", "r");
    if (!allowlist)
    {
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), allowlist))
    {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, script_path) == 0)
        {
            fclose(allowlist);
            return true;
        }
    }

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

        execl("/usr/bin/python3", "python3", script_path, (char *)NULL);
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
