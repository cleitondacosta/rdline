#ifndef _RDLINE_RDPRINT_
#define _RDLINE_RDPRINT_

#include <stdio.h>
#include <stdint.h>
#include "programinfo.h" // USAGE, VERSION

enum PRINT_FLAGS { DEFAULT=0, SHOWNAME=1, SHOWINDICE=2};

// Function to print the line 
void print_line(char *fileName, size_t line, const enum PRINT_FLAGS FLAGS)
{
    char *lineRead = read_line(fileName, line);

    if(FLAGS == 0b00000000)
        printf("%s", lineRead);
    else if(FLAGS == 0b00000001)
        printf("%s:%s", fileName, lineRead);
    else if(FLAGS == 0b00000010)
        printf("%ld:%s", line, lineRead);
    else if(FLAGS == 0b00000011)
        printf("%s %ld:%s", fileName, line, lineRead);

    free(lineRead);
}

// Function to print the total numbers of lines
void print_total(char *fileName, const enum PRINT_FLAGS FLAGS)
{
    if((FLAGS == 0b00000000) || (FLAGS == 0b00000010))
        printf("%ld\n", total_lines(fileName));
    else
        printf("%s total:%ld\n", fileName, total_lines(fileName));
}

// Neurotic function
void print_errno(int error, const char *fileName)
{
    switch(error)
    {
        case EACCES:
            fprintf(
                stderr,
                "rdline: Error: You haven't permission to "\
                "access the file \"%s\".\n",
                fileName
            );
            break;
        
        case ENAMETOOLONG:
            fprintf(
                stderr,
                "rdline: Error: The \"file\" \"%s\" has a very long name.\n",
                fileName
            );
            break;

        case ENOENT:
            fprintf(
                stderr,
                "rdline: Error: File \"%s\" doesn't exists.\n",
                fileName
            );
            break;

        // Holy
        case ENOTDIR:
            fprintf(
                stderr,
                "rdline: Error: A component of \"%s\" isn't a directory.\n",
                fileName
            );
            break;

        default:
           perror("rdline: Error");
    }
}

void print_help(void)
{
    printf(
        "NAME:\n"
        "  rdline is a command to read line(s) of file(s).\n"
        "\n"
        "SYNOPSIS:\n"
        "   %s\n"
        "\n"
        "DESCRIPTION:\n"
        "   rdline is a simple linux command to read specific line(s) of\n"
        "   file(s). It supports read range of lines. See the -l description\n"
        "   below.\n\n"
        "   -l, --line lnum\n"
        "       Read the specific line(s) of file(s). lnum is a number, but\n"
        "       not a common number. You can parse one or more numbers with\n"
        "       it. It can be numbers separated by comma, or a range of\n"
        "       numbers separated by colon. Or both.\n"
        "       Some examples:\n"
        "         10 - parses the number 10\n"
        "         1,2,3,4,5 - parses the numbers 1, 2, 3, 4 and 5.\n"
        "         500:1000 - parses the numbers 500 to 1000 (both inclusive)\n"
        "         1000:500 - parses the numbers 1000 to 500 (both inclusive)\n"
        "         1,4,5,10:20,49 - parses the numbers 1, 4, 5, 10 to 20 and\n"
        "                          49.\n"
        "         Note that, a lnum must be a non-zero natural number.\n"
        "         And 18 digits max.\n"
        "   -t, --total\n"
        "       Show the total lines of given file(s). But wc -l is faster.\n"
        "   --show-name\n"
        "       Show the file's name. (works with -l and -t)\n"
        "   --show-indice\n"
        "       Show the number of line (works only with -l).\n"
        "   --version\n"
        "       Show the version.\n"
        "   --help\n"
        "       Show this message.\n"
        "\n"
        "RETURN VALUE:\n"
        "   Returns 0 upon success.\n"
        "   Returns a non-zero value upon failure.\n",
        USAGE
    );
}

#endif // _RDLINE_RDPRINT_
