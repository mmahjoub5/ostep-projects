#include "shellCommands.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>

void removeWhiteSpace(char *str)
{
    int readIndex = 0;
    int writeIndex = 0;

    while (str[readIndex])
    {
        if (!isspace((unsigned char)str[readIndex]))
        {
            str[writeIndex++] = str[readIndex];
        }
        readIndex++;
    }

    str[writeIndex] = '\0'; // Null-terminate the resulting string
}

/*
    need to refactor this we are doing two things in this function, updating the command and checking if it is valid,path

*/
bool checkIfShellCommand(char **command, char *paths[PATHNAMESIZE])
{
    char *tempCommand = NULL;
    for (int i = 0; i < PATHSIZE; i++)
    {
        if (asprintf(&tempCommand, "%s/%s", paths[i], *command) == -1)
        {
            // printf(stderr, "Memory allocation error\n");
            free(tempCommand);
            return false; // exit with an error code
        }

        if (access(tempCommand, F_OK) == 0)
        {
            // printf("add path to command %s\n", tempCommand);
            free(*command);
            printf("%s\n", tempCommand);
    
            *command = strdup(tempCommand);
            free(tempCommand);
            printf("%s\n", *command);
            // printf("add path to command %s\n", *command);
            return true;
        }
        free(tempCommand);

    }
    return false;
}

// TODO:
// char *updateCommandPath(char *commamd, char **paths[PATHNAMESIZE])
// {

// }

void executeShellCommand(char **cmd_argv, int loopNum)
{
    int rc;
    for (int i = 0; i < loopNum; i++)
    {
        rc = fork();

        if (rc == 0)
        {
            execv(cmd_argv[0], cmd_argv);
            printf("exec failed\n");
        }
        else if (rc > 0)
        {

            (void)wait(NULL);

            // printf("parent running\n");
            //  free(tofree);
        }
        else
        {
            // printf("failed\n");
        }
    }
}

// void printToFile(char *filename)
// {
//     // print to a file
//     printf("%d\n", STDOUT_FILENO);
//     (void)close(STDOUT_FILENO);
//     int fd = open("test.txt", O_WRONLY, S_IRWXU | O_CREAT | O_TRUNC);
//     printf("%d\n", fd);
// }