#ifndef FILES_H
#define FILES_H

#include <stdbool.h>

bool file_exists(const char *file_path);
char *read_file_content(const char *file_path);

#endif // FILES_H
