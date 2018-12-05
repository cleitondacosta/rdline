// Ver 0.2
#ifndef STRMANIP_H
#define STRMANIP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Inverts the string
char *strrev(char *str)
{
    size_t STR_LEN = strlen(str);
    size_t i;
    size_t j = STR_LEN-1;
    char *reversed = malloc(sizeof(char) * (STR_LEN+1));

    for(i = 0; i < STR_LEN; i++, j--)
        reversed[i] = str[j];

    return reversed;
}

long int first_index_of(const char *str, const char pattern)
{
    const size_t STR_LEN = strlen(str);

    bool found = false;
    size_t i;

    for(i = 0; i < STR_LEN; i++)
    {
        if(str[i] == pattern)
        {
            found = true;
            break;
        }
    }

    return ((found) ? i : -1);
}

long int last_index_of(const char *str, const char pattern)
{
    const size_t STR_LEN = strlen(str);
    
    bool found = false;
    size_t i;

    for(i = STR_LEN-1; i > 0; i--)
    {
        if(str[i] == pattern)
        {
            found = true;
            break;
        }
    }

    if(!found)
    {
        if(str[0] == pattern)
        {
            i = 0;
            found = true;
        }
    }
    
    return ((found) ? i : -1);
}

// Check if pattern is in string
bool strisin(const char *str, char pattern)
{
    size_t STR_LEN = strlen(str);
    bool havePattern = false;

    for(size_t i = 0; i < STR_LEN; i++)
    {
        if(str[i] == pattern)
        {
            havePattern = true;
            break;
        }
    }

    return havePattern;
}

// Function based on cut command. If column is 0, then it returns the last col
char *strcut(char *str, const char sep, size_t column)
{
    if(!strisin(str, sep))
        return str;

    const size_t STR_LEN = strlen(str);
    char *cutted = malloc(0);

    if(column == 1)
    {
        for(size_t i = 0; (i < STR_LEN) && (str[i] != sep); i++)
        {
            cutted = realloc(cutted, sizeof(char));
            if(cutted == NULL)
            {
                fprintf(
                    stderr,
                    "rdline: Fatal error: Couldn't allocate memory.\n"
                );
                exit(EXIT_FAILURE);
            }
            cutted[i] = str[i];
        }
    }
    else if(column == 0) // when 0, get the last column
    {
        for(size_t i = STR_LEN-1; (i > 0) && (str[i] != sep); i--)
        {
            cutted = realloc(cutted, sizeof(char));
            if(cutted == NULL)
            {
                fprintf(
                    stderr,
                    "rdline: Fatal error: Couldn't allocate memory.\n"
                );
                exit(EXIT_FAILURE);
            }

            cutted[i] = str[i];
        }

        if(str[0] != sep)
        {
            cutted = realloc(cutted, sizeof(char));
            cutted[0] = str[0];
        }

        cutted = strrev(cutted);
    }
    else // nor 0 or 1
    {
        size_t currentColumn = 1;
        size_t j = 0;
        for(size_t i = 0; i < STR_LEN; i++)
        {
            if(str[i] == sep)
            {
                currentColumn++;
                if(currentColumn == column)
                {
                    for(i++; (i < STR_LEN) && (str[i] != sep); i++, j++)
                    {
                        cutted = realloc(cutted, sizeof(char));
                        if(cutted == NULL)
                        {
                            fprintf(
                                stderr,
                                "rdline: Fatal error: Couldn't allocate "
                                "memory.\n"
                            );
                            exit(EXIT_FAILURE);
                        }

                        cutted[j] = str[i];
                    }
                    break;
                }
            }
        }
    }

    return cutted;
}

// It counts how many patterns have in string
size_t strcnt(const char *str, const char pattern)
{
    const size_t STR_LEN = strlen(str);
    size_t count = 0;

    for(size_t i = 0; i < STR_LEN; i++)
    {
        if(str[i] == pattern)
            count++;
    }

    return count;
}

#endif // STRMANIP_H
