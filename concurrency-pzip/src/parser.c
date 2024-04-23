#include "../header/parser.h"

compressedNode *initializeCompressedNode(int number, char letter)
{
    if (isspace(letter))
    {
        printf("\nwe aree here\n");
        return NULL;
    }
    compressedNode *node = malloc(sizeof(compressedNode *));
    node->number = number;
    node->letter = letter;
    return node;
}

void compress(ThreadReturnArgs *returnArgs, char *str)
{
    size_t length = strlen(str);
    printf("\n%li\n", length);
    // printf("size of string %zu", length);
    int counter = 0;
    char prev = str[0];
    for (int i = 0; i < length; i++)
    {
        if (isspace(str[i]))
        {
            // printf("\n%c we aree here\n", prev);
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
                if (!isspace(prev))
                {
                    addToThreadReturnArgs(returnArgs, initializeCompressedNode(counter, prev));
                }
            }

            counter = 1;
        }
        prev = str[i];
    }
    if (counter != 0)
    {
        if (!isspace(prev))
        {
            addToThreadReturnArgs(returnArgs, initializeCompressedNode(counter, prev));
        }
    }
}
