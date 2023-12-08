#include "builtInCommands.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "shellCommands.h"
#include <ctype.h> 


static void builtin_cd(char **cmd_argv, int numCLIArgs)
{
    if (numCLIArgs == 1 || isspace(cmd_argv[1][0]))
    {
        // cmd_argv[1] = getenv("HOME");
        fprintf(stderr, "An error has occurred\n");
        return;
    }
    if (numCLIArgs > 2)
    {
        // ////////printf("this is an error for cd syntax\n");
        fprintf(stderr, "An error has occurred\n");
        return;
    }
    if (chdir(cmd_argv[1]) != 0)
    {
        perror("chdir() to  failed");
    }
}

static void builtin_path(char **cmd_argv, char *paths[PATHNAMESIZE])
{
    // printf("enetering path\n");

    // Free existing paths
    for (int i = 0; i < PATHSIZE; i++)
    {
        if (paths[i] != NULL)
        {
            paths[i] = NULL;
        }
    }

    for (int i = 1; cmd_argv[i] != NULL; i++)
    {
        if (i - 1 < PATHSIZE)
        {
            paths[i - 1] = strdup(cmd_argv[i]);
        }

        // printf("temp command: %s\n", paths[i - 1]);
    }
}

static void builtin_loop(char **cmd_argv, char *paths[PATHNAMESIZE])
{
    char **temp = cmd_argv + 2;
    // Execute the command directly in the loop
    if (checkIfShellCommand(temp, paths))
    {
        executeShellCommand(temp, atoi(cmd_argv[1]));
    }
}

static void builtin_exit(int numCLIArgs)
{
    if(numCLIArgs > 1)
    {
        fprintf(stderr, "An error has occurred\n");
    }
    else 
    {
        exit(0);
    }
}


void runBuiltInCommand(int numCLIArgs, char **cmd_argv, char *paths[PATHNAMESIZE])
{

    if (strcmp(cmd_argv[0], "cd") == 0)
    {
        builtin_cd(cmd_argv, numCLIArgs);
    }
    else if (strcmp(cmd_argv[0], "path") == 0)
    {
        builtin_path(cmd_argv, paths);
    }
    else if (strcmp(cmd_argv[0], "loop") == 0)
    {
        builtin_loop(cmd_argv, paths);
    }
    else if (strcmp(cmd_argv[0], "exit") == 0)
    {
        builtin_exit(numCLIArgs);
    }
else
    {
        // printf("\n\nERROR ---> command not found\n\n");
    }
}
