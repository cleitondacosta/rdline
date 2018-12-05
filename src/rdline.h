#ifndef _RDLINE_RDLINE_H
#define _RDLINE_RDLINE_H

#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

bool is_valid_lnum(const char *strRangeNumber)
{
    const char *exp =
        "^[1-9][0-9]{0,17}(:[1-9][0-9]{0,17})?"\
        "(,[1-9][0-9]{0,17}(:[1-9][0-9]{0,17})?)*$";

    regex_t reg;
    if(regcomp(&reg, exp, REG_EXTENDED|REG_NOSUB) != 0)
    {
        fprintf(stderr, "Couldn't compile regex.\n");
        exit(EXIT_FAILURE);
    }

    if(regexec(&reg, strRangeNumber, 0, NULL, 0) != REG_NOMATCH)
        return true;
    else
        return false;
}

char *read_line(const char *fileName, uintmax_t lineToRead)
{
    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        fprintf(stderr, "rdline: Error: \"%s\" became NULL.\n", fileName);
        exit(EXIT_FAILURE);
    }

    char *buffer = NULL;
    uintmax_t len = 0;
    uintmax_t i;

    for(i = 1; (getline(&buffer, &len, file) != -1) && (i != lineToRead); i++)
    {
        if(file == NULL)
        {
            fclose(file);
            fprintf(stderr, "rdline: Error: \"%s\" became NULL.\n", fileName);
            exit(EXIT_FAILURE);
        }
    }

    switch(errno)
    {
        case EINVAL:
            fprintf(
                stderr,
                "rdline: Error: getline error. n or lineptr is NULL, "\
                "or stream is not valid.\n"
            );
            free(buffer);
            exit(EXIT_FAILURE);

        case ENOMEM:
            fprintf(
                stderr,
                "rdline: Fatal error: Failed to allocate memory.\n"
            );
            free(buffer);
            exit(EXIT_FAILURE);
    }

    fclose(file);
    return buffer;
}

uintmax_t total_lines(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char *line = NULL;
    uintmax_t len = 0;
    uintmax_t i;

    for(i = 0; getline(&line, &len, file) != -1; i++);

    switch(errno)
    {
        case EINVAL:
            fprintf(
                stderr,
                "rdline: Error: getline error. n or lineptr is NULL, "\
                "or stream is not valid.\n"
            );
            exit(EXIT_FAILURE);

        case ENOMEM:
            fprintf(
                stderr,
                "rdline: Fatal error: Failed to allocate memory.\n"
            );
            exit(EXIT_FAILURE);
    }

    fclose(file);
    return i;
}

#endif // _RDLINE_RDLINE_H
