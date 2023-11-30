#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "linkedlist.h"
#include "cliHelpers.h"

char *defaultPath = "/bin";
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

void checkCommand(char *command, List *list)
{

    Node *iter = list->header;
    for (int i = 0; iter != NULL; i++)
    {
        if (asprintf(&command, "%s/%s", iter->filename, command) == -1)
        {
            fprintf(stderr, "Memory allocation error\n");
            return 1; // exit with an error code
        }

        if (!access(command, F_OK))
        {
            return;
        }
        iter = iter->next;
    }
}

void runBuiltInCommand(int k, char **cmd_argv, List *list)
{

    if (strcmp(cmd_argv[0], "cd") == 0) // TODO: DEAL WITH WHITE SPACE
    {
        char s[100];
        // Printing the current working directory
        printf("%s\n", getcwd(s, 100));
        if (k == 1)
        {
            cmd_argv[1] = getenv("HOME");
        }
        if (k > 2)
        {
            printf("this is an error for cd syntax\n");
            return;
        }
        if (chdir(cmd_argv[1]) != 0)
        {
            perror("chdir() to  failed");
        }
        printf("%s\n", getcwd(s, 100));
    }
    else if (strcmp(cmd_argv[0], "path") == 0)
    {
        clearList(list);
        for (int i = 1; cmd_argv[i] != NULL; i++)
        {
            push(cmd_argv[i], list);
        }
        printList(list);
    }
    else
    {
        printf("command not found\n");
    }
}
int main(int agrc, char *argv[])
{

    int rc;
    List list = {NULL, NULL};
    push(defaultPath, &list);

    // if (agrc > 1)
    // {
    //     printf("batch file\n");
    //     FILE *fp;

    //     fp = open(argv[1], "r");
    //     if (fp != NULL)
    //     {
    //         printf("file exists!\n");
    //     }
    //     else
    //     {
    //         fp = fopen(argv[1], "w");
    //         assert(fp);
    //     }
    //     char *buffer;
    //     size_t bufsize = 0;
    //     size_t characters;

    //     buffer = (char *)malloc(bufsize * sizeof(char));
    //     if (buffer == NULL)
    //     {
    //         perror("Unable to allocate buffer\n");
    //         exit(1);
    //     }
    //     fp = fopen(argv[1], "r");

    //     while ((characters = getline(&buffer, &bufsize, fp) != -1))
    //     {
    //     }
    //     if (buffer)
    //     {
    //     }
    //     return 0;
    // }

    // change to a do while

    /*
        do
        {
            read in shell
            run comands
            get status
        } while(status);
    */

    while (1)
    {
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
            char **cmd_argv;
            char *orginalCommand;
            int k = parseInput(&cmd_argv, line);
            orginalCommand = strdup(cmd_argv[0]);

            if (asprintf(&cmd_argv[0], "%s/%s", defaultPath, cmd_argv[0]) == -1)
            {
                fprintf(stderr, "Memory allocation error\n");
                return 1; // exit with an error code
            }
            for (int i = 0; cmd_argv[i] != NULL; i++)
            {
                printf("Argument %d: %s\n", i, cmd_argv[i]);
            }

            if (!access(cmd_argv[0], F_OK))
            {
                rc = fork();
            }
            else
            {
                printf("k valuee: %i", k);
                cmd_argv[0] = orginalCommand;
                runBuiltInCommand(k, cmd_argv, &list);
            }

            if (rc == 0)
            {
                printf("wer ar hjee");
                execv(cmd_argv[0], cmd_argv);
                printf("exec failed\n");
            }
            else if (rc > 0)
            {
                (void)wait(NULL);
                printf("parent running\n");
                // free(tofree);
            }
            else
            {
                printf("failed\n");
            }
        }
        else
        {
            printf("Error reading input.\n");
        }

        // Don't forget to free the allocated memory
        free(line);
    }
}

/*

      // print to a file
                // (void)close(STDOUT_FILENO);
                // int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
                // printf("%d\n", fd);

*/