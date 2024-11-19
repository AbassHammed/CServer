#ifndef ERRORS_H
#define ERRORS_H

const char *get_error_message(int error_code);

enum ErrorCodes
{
    ERROR_404 = 404,
    ERROR_500 = 500,
    ERROR_403 = 403,
    ERROR_405 = 405
};

#endif // ERRORS_H
