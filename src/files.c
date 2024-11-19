#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool file_exists(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

char *read_file_content(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);
    return content;
}
