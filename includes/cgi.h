#ifndef CGI_H
#define CGI_H

#include <stdbool.h>

bool is_cgi_authorized(const char *script_path);
void handle_cgi_request(int client_fd, const char *script_path);

#endif // CGI_H
