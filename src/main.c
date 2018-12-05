#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "rdline.h" // is_valid_lnum(), read_line(), total_lines()
#include "strmanip.h" // strisin(), strcut(), strcnt()
#include "rdprint.h" // print_total(), print_line(), print_help()
#include "programinfo.h" // USAGE, VERSION

struct option const longOptions[] = {
    {"line", required_argument, NULL, 'l'},
    {"total", no_argument, NULL, 't'},
    {"show-name", no_argument, NULL, 1},
    {"show-indice", no_argument, NULL, 2},
    {"version", no_argument, NULL, 3},
    {"help", no_argument, NULL, 4},
    {0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
    unsigned int paramLine = 0;
    unsigned int paramTotal = 0;
    unsigned int paramShowName = 0;
    unsigned int paramShowIndice = 0;
    enum PRINT_FLAGS FLAGS = DEFAULT;
    char *argLine;
    char **argFile; // The mandatory argument (FILE...)

    char op;
    while((op = getopt_long(argc, argv, ":l:t", longOptions, NULL)) != -1)
    {
        switch(op)
        {
            case 3:
                printf("rdline: Version: %s\n", VERSION);
                return EXIT_SUCCESS;
            case 4:
                print_help();
                return EXIT_SUCCESS;

            case 1: // --show-name
                paramShowName++;
                break;

            case 2: // --show-indice
                paramShowIndice++;
                break;

            case 'l':
                paramLine++;
                argLine = optarg;
                break;

            case 't':
                paramTotal++;
                break;

            case '?': // unknown option
                fprintf(
                    stderr,
                    "rdline: Error: Unknown option \'%c\'. "\
                    "Try rdline -- help.\n",
                    optopt
                );
                return EXIT_FAILURE;

            case ':': // missing argument for option
                fprintf(
                    stderr,
                    "rdline: Error: Option \'%c\' requires argument.\n",
                    optopt
                );
                return EXIT_FAILURE;
        }
    }

    // Checking if missing the mandatory argument.
    if(optind >= argc)
    {
        fprintf(stderr, "rdline: Error: Missing FILE... argument.\n");
        fprintf(stderr, "rdline: Usage: %s\n", USAGE);
        return EXIT_FAILURE;
    }

    // Allocating memory to store the mandatories arguments
    argFile = malloc((argc - optind) * sizeof(char*));
    if(argFile == NULL)
    {
        fprintf(stderr, "rdline: Fatal error: Couldn't allocate memory.\n");
        return EXIT_FAILURE;
    }

    // Searching for non option arguments (the argument FILE...)
    uintmax_t nonOptionalArguments = 0;
    for(uintmax_t i = optind; i < argc; i++, nonOptionalArguments++)
    {
        argFile[nonOptionalArguments] =
            malloc((strlen(argv[i]) * sizeof(char)) + sizeof(char));
            if(argFile[nonOptionalArguments] == NULL)
            {
                fprintf(
                    stderr,
                    "rdline: Fatal error: Couldn't allocate memory.\n"
                );
                return EXIT_FAILURE;
            }
        argFile[nonOptionalArguments] = argv[i];
    }
    
    // Checking file type and if it's readable
    for(size_t i = 0; i < nonOptionalArguments; i++)
    {
        if(access(argFile[i], F_OK) == 0)
        {
            if(access(argFile[i], R_OK) == 0)
            {
                struct stat buffer;

                if(stat(argFile[i], &buffer) != 0)
                {
                    print_errno(errno, argFile[i]);
                    exit(EXIT_FAILURE);
                }

                if((buffer.st_mode & S_IFMT) != S_IFREG)
                {
                    fprintf(stderr,
                        "rdline: Error: \"%s\" isn't a regular file.\n",
                        argFile[i]
                    );
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                print_errno(errno, argFile[i]);
                return EXIT_FAILURE;

            }
        }
        else
        {
            //print_errno(errno, argFile[i]);
            perror("revbin: Error");
            return EXIT_FAILURE;
        }
    }

    if(
        paramLine > 1
        || paramTotal > 1
        || paramShowName > 1
        || paramShowIndice > 1
    )
    {
        fprintf(stderr, "rdline: Error: You cannot repeat options.\n");
        fprintf(stderr, "rdline: Usage: %s\n", USAGE);
        return EXIT_FAILURE;
    }

    // Checking if lnum is correct, in case of -l
    if(paramLine == 1)
    {
        if(!is_valid_lnum(argLine))
        {
            fprintf(
                stderr,
                "rdline: Error: Invalid lnum: \"%s\". Try \"rdline --help\"\n",
                argLine
            );
            return EXIT_FAILURE;
        }
    }

    // Flags for --show-name and --show-line
    if((paramShowName == 1) && (paramShowIndice == 0))
        FLAGS = SHOWNAME;
    else if((paramShowIndice == 1) && (paramShowName == 0))
        FLAGS = SHOWINDICE;
    else if((paramShowName == 1) && (paramShowIndice == 1))
        FLAGS = SHOWNAME | SHOWINDICE;

    // Running
    if(paramTotal == 1) // -t
    {
        for(size_t i = 0; i < nonOptionalArguments; i++)
            print_total(argFile[i], FLAGS);
    }
    if(paramLine == 1) // -l
    {
        const size_t commaCount = strcnt(argLine, ',');

        if(commaCount >= 1) // contains ,
        {
            for(size_t i = 0; i < nonOptionalArguments; i++)
            {
                for(size_t j = 1; j <= commaCount+1; j++)
                {
                    char *lineStr = strcut(argLine, ',', j);

                    if(strisin(lineStr, ':')) // range num
                    {
                        const size_t leftnum = atol(strcut(lineStr, ':', 1));
                        const size_t rightnum = atol(strcut(lineStr, ':', 2));

                        if(leftnum < rightnum) // crescent
                            for(size_t k = leftnum; k <= rightnum; k++)
                                print_line(argFile[i], k, FLAGS);
                        else if(leftnum > rightnum) // decrescent
                            for(size_t k = leftnum; k >= rightnum; k--)
                                print_line(argFile[i], k, FLAGS);
                        else if(leftnum == rightnum) // whatever
                            print_line(argFile[i], leftnum, FLAGS);
                    }
                    else // non range
                    {
                        const size_t line = atol(lineStr);
                        print_line(argFile[i], line, FLAGS); 
                    }

                    free(lineStr);
                }
            }
        }
        else // doesn't contains ,
        {
            for(size_t i = 0; i < nonOptionalArguments; i++)
            {
                if(strisin(argLine, ':')) // range num
                {
                    const size_t leftnum = atol(strcut(argLine, ':', 1));
                    const size_t rightnum = atol(strcut(argLine, ':', 2));

                    if(leftnum < rightnum) // crescent
                        for(size_t k = leftnum; k <= rightnum; k++)
                            print_line(argFile[i], k, FLAGS);
                    else if(leftnum > rightnum) // decrescent
                        for(size_t k = leftnum; k >= rightnum; k--)
                            print_line(argFile[i], k, FLAGS);
                    else if(leftnum == rightnum) // whatever
                        print_line(argFile[i], leftnum, FLAGS);
                }
                else // non range
                    print_line(argFile[i], atol(argLine), FLAGS);
            }
        }
    }

    return EXIT_SUCCESS;
}
