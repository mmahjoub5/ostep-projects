
#include "cliHelpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int parseInput(char ***cmd_argv, char *line)
{
    // parse input
    printf("%s\n", line);
    char *token, *string, *tofree;

    tofree = string = strdup(line);

    if (string == NULL)
    {
        // mem allocation failure
        return -1;
    }
    int size = 10;
    assert(string != NULL);
    int k = 0;

    char **cmd_argv_local = malloc(size * sizeof(char *));

    if (cmd_argv_local == NULL)
    {
        // Handle memory allocation failure
        free(tofree);
        return -1;
    }

    while ((token = strsep(&string, " ")) != NULL)
    {
        cmd_argv_local[k] = strdup(token);
        if (cmd_argv_local[k] == NULL)
        {
            // Handle memory allocation failure
            free(tofree);
            // Free already allocated cmd_argv_local strings
            for (int i = 0; i < k; ++i)
            {
                free(cmd_argv_local[i]);
            }
            free(cmd_argv_local);
            return -1;
        }

        cmd_argv_local[k][strcspn(cmd_argv_local[k], "\n")] = '\0';
        k++;

        // Assign the local array to the output parameter
        if (cmd_argv != NULL)
        {
            *cmd_argv = cmd_argv_local;
        }
        else
        {
            // If the caller didn't request the array, free it
            for (int i = 0; i < k; ++i)
            {
                free(cmd_argv_local[i]);
            }
            free(cmd_argv_local);
        }
    }
    cmd_argv[k] = NULL;
    return k;
}