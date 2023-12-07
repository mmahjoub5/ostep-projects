#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

// My stuff
#include "linkedlist.h"
#include "cliHelpers.h"
#include "constants.h"
#include "shellCommands.h"
#include "builtInCommands.h"

char *defaultPath = "";

/*
    TODO:

    - add built-in commands, loop and parallel
    - check for edge cases
    - add ability for read in a bash file that has mulitple commands, and execute those commands
    - reroute outputs to a file
    - add your own make file to practice compiling multiple files and linking them together
*/

/*
    In this project, you should implement exit, cd, and path as built-in commands.

    exit: When the user types exit, your shell should simply call the exit system call with 0 as a parameter. It is an error to pass any arguments to exit.

    cd: cd always take one argument (0 or >1 args should be signaled as an error). To change directories, use the chdir() system call with the argument supplied by the user; if chdir fails, that is also an error.
    cd cannot be a program, must be coded in shell

    path: The path command takes 0 or more arguments, with each argument separated by whitespace from the others.
    A typical usage would be like this: wish> path /bin /usr/bin, which would add /bin and /usr/bin to the search path of the shell.
    If the user sets path to be empty, then the shell should not be able to run any programs (except built-in commands).
    The path command always overwrites the old path with the newly specified path
*/

// put these in a new file

int main(int agrc, char *argv[])
{
    printf("are we working here??, %d\n", agrc);
    for (int i = 0; i < agrc; i++)
    {

        printf("cmd inputs: %s\n", argv[i]);
    }

    // char *b = malloc(4 * sizeof(char *));
    char *paths[PATHSIZE] = {NULL};
    int numCLIArgs = 0;

    /* Now, lets work on b */
    for (int i = 0; i < PATHSIZE; i++)
    {
        if ((paths[i] = malloc(sizeof(char) * PATHNAMESIZE)) == NULL)
        {
            // printf("unable to allocate memory \n");
            return -1;
        }
    }

    paths[0] = "/bin";

    char **cmd_argv = malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        cmd_argv[i] = malloc((892 + 1) * sizeof(char));
    }
    char *orginalCommand;

    if (agrc > 1)
    {
        FILE *fp;

        fp = fopen(argv[agrc - 1], "r+");

        char *buffer;
        size_t bufsize = 0;
        size_t characters;

        buffer = (char *)malloc(bufsize * sizeof(char));
        if (buffer == NULL)
        {
            perror("Unable to allocate buffer\n");
            exit(1);
        }

        while ((characters = getline(&buffer, &bufsize, fp) != -1))
        {
            numCLIArgs = parseInput(cmd_argv, buffer);
            printf("cmd args%s", cmd_argv[0]);
            orginalCommand = strdup(cmd_argv[0]);
            if (checkIfShellCommand(&cmd_argv[0], paths))
            {
                // printf("\nthis is the command we want to run %s", cmd_argv[0]);
                free(orginalCommand);
                executeShellCommand(cmd_argv, 1);
            }
            else
            {
                cmd_argv[0] = orginalCommand;

                runBuiltInCommand(numCLIArgs, cmd_argv, paths);
            }
        }
        if (buffer)
        {
            free(buffer);
        }

        return 0;
    }

    while (1)
    {
        printf("are we working here??");
        for (int i = 0; i < 10; i++)
        {
            cmd_argv[i] = strdup("");
        }
        char *line = NULL;
        size_t len = 0; // size of the buffer

        printf("wish>\t");

        // getline will allocate memory for the line
        ssize_t read = getline(&line, &len, stdin);

        if (read != -1)
        {
            if (strcmp("exit\n", line) == 0)
            {
                exit(0);
            }
            numCLIArgs = parseInput(cmd_argv, line);
            if (numCLIArgs == -1)
            {
                perror("cli parse error");
            }
            orginalCommand = strdup(cmd_argv[0]);

            if (checkIfShellCommand(&cmd_argv[0], paths))
            {
                free(orginalCommand);
                executeShellCommand(cmd_argv, 1);
            }
            else
            {
                cmd_argv[0] = orginalCommand;
                runBuiltInCommand(numCLIArgs, cmd_argv, paths);
            }
        }
        else
        {
            // printf("Error reading input.\n");
        }

        // Don't forget to free the allocated memory
        free(line);
    }
    free(cmd_argv);
}

/*

    print to a file
    (void)close(STDOUT_FILENO);
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
    printf("%d\n", fd);

*/

// change to a do while

/*
    do
    {
        read in shell
        run comands
        get status
    } while(status);
*/