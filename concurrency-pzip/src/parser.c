#include "../header/parser.h"

compressedNode *initializeCompressedNode(int number, char letter)
{
    compressedNode *node = malloc(sizeof(compressedNode *));
    node->number = number;
    node->letter = letter;
    return node;
}

void compress(ThreadReturnArgs *returnArgs, char *str, pthread_mutex_t *result_mutex)
{
    size_t length = strlen(str);
    // printf("size of string %zu", length);
    int counter = 0;
    char prev = str[0];
    for (int i = 0; i < length; i++)
    {
        if (isspace(str[i]))
        {
            continue;
        }
        if (prev == str[i])
        {
            counter++;
        }
        else
        {
            if (counter != 0)
            {
                pthread_mutex_lock(result_mutex);
                addToThreadReturnArgs(returnArgs, initializeCompressedNode(counter, prev));
                pthread_mutex_unlock(result_mutex);
            }

            counter = 1;
        }

        prev = str[i];
    }
    if (counter != 0)
    {
        pthread_mutex_lock(result_mutex);
        addToThreadReturnArgs(returnArgs, initializeCompressedNode(counter, prev));
        pthread_mutex_unlock(result_mutex);
    }
}