#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

const char *defaultPath = "/bin";
/*
    TODO:

    - add built-in commands, loop and parallel
    - check for edge cases
    - add ability for read in a bash file that has mulitple commands, and execute those commands
    - reroute outputs to a file
    - add your own make file to practice compiling multiple files and linking them together
*/
int main(int agrc, char *argv[])
{

    while (1)
    {
        char *line = NULL;
        size_t len = 0; // size of the buffer

        printf("wish>\t");

        // getline will allocate memory for the line
        ssize_t read = getline(&line, &len, stdin);

        if (read != -1)
        {
            // build in commands

            if (strcmp("exit\n", line) == 0)
            {
                exit(0);
            }

            int rc = fork();

            if (rc == 0)
            {
                // parse input
                printf("%s\n", line);
                char *token, *string, *tofree;

                tofree = string = strdup(line);
                assert(string != NULL);
                int k = 0;
                char *cmd_argv[10];
                while ((token = strsep(&string, " ")) != NULL)
                {
                    // printf("%s\n", token);
                    cmd_argv[k] = token;
                    cmd_argv[k][strcspn(cmd_argv[k], "\n")] = '\0';
                    k++;
                }
                cmd_argv[k] = NULL;

                if (asprintf(&cmd_argv[0], "%s/%s", defaultPath, cmd_argv[0]) == -1)
                {
                    fprintf(stderr, "Memory allocation error\n");
                    return 1; // exit with an error code
                }
                for (int i = 0; cmd_argv[i] != NULL; i++)
                {
                    printf("Argument %d: %s\n", i, cmd_argv[i]);
                }

                cmd_argv[k] = NULL;
                execv(cmd_argv[0], cmd_argv);
                free(tofree);

                printf("exec failed\n");
                exit(0);
            }
            else if (rc > 0)
            {
                (void)wait(NULL);
                printf("parent running\n");
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