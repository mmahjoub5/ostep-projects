
#include "cliHelpers.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int parseInput(char **cmd_argv, char *line)
{
    // parse input
    printf("%s\n", line);
    char *token, *buffer, *tofree;

    tofree = buffer = strdup(line);

    if (buffer == NULL)
    {
        // mem allocation failure
        return -1;
    }
    assert(buffer != NULL);
    int k = 0;

    while ((token = strsep(&buffer, " ")) != NULL)
    {
        cmd_argv[k] = strdup(token);
        cmd_argv[k][strcspn(cmd_argv[k], "\n")] = '\0';
        k++;
    }
    cmd_argv[k] = NULL;
    free(tofree);
    // *cmd_argv = cmd_argv_local;

    printf("k valuue :%i", k);

    return k;
}
