#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Verify if a file exist by opening it, if the file exits the `fopen`
function will return a stream to read the file contents or else null
*/
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

/*
Read the content of a file and return it as a string.
If the file cannot be opened, return NULL.
*/
char *read_file_content(const char *file_path)
{
    // Open the file in read mode
    FILE *file = fopen(file_path, "r");
    if (!file)
        return NULL;

    // Move the file pointer to the end of the file
    fseek(file, 0, SEEK_END);
    // Get the size of the file
    size_t size = ftell(file);
    // Move the file pointer back to the beginning of the file
    rewind(file);

    char *content = malloc(size + 1);
    // Read the file content into the allocated memory
    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);
    return content;
}
