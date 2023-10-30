#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

const char *defaultPath = "/bin";

int main(int agrc, char *argv[])
{

    while (1)
    {
        char *line = NULL;
        size_t len = 0; // size of the buffer

        printf("Enter a line of text: ");

        // getline will allocate memory for the line
        ssize_t read = getline(&line, &len, stdin);

        if (read != -1)
        {
            line[strcspn(line, "\n")] = '\0';
            printf("You entered: %s", line);
            int rc = fork();

            if (rc == 0)
            {
                printf("child process\n");
                // Use asprintf to dynamically allocate memory for the path
                char *path;
                if (asprintf(&path, "%s/%s", defaultPath, line) == -1)
                {
                    fprintf(stderr, "Memory allocation error\n");
                    return 1; // exit with an error code
                }

                char *cmd_argv[10];

                cmd_argv[0] = strdup(path);
                cmd_argv[1] = NULL;
                printf("%s", cmd_argv[0]);
                execv(cmd_argv[0], cmd_argv);
                printf("exec failed");
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