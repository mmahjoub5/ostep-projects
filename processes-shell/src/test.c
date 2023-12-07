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

int main(int agrc, char *argv[])
{
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
    char *command = "ls";
    printf("%d\n", checkIfShellCommand(&command, paths));
}
