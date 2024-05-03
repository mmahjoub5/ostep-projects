#include "../header/threadHelpers.h"

ThreadReturnArgs initializeThreadReturnArgs()
{
    ThreadReturnArgs returnArgs;
    returnArgs.list = NULL;
    returnArgs.capacity = 0;
    returnArgs.size = 0;
    return returnArgs;
}

void addToThreadReturnArgs(ThreadReturnArgs *returnArgs, compressedNode *node)
{
    // check if the size of the array is at capacity
    if (returnArgs->size == returnArgs->capacity)
    {
        if (returnArgs->size == 0)
        {
            returnArgs->capacity = 1;
        }
        else
        {
            returnArgs->capacity = 2 * returnArgs->size;
        }
        returnArgs->list = realloc(returnArgs->list, returnArgs->capacity * sizeof(compressedNode *));
        if (returnArgs->list == NULL)
        {
            // perror("realloc");
            exit(EXIT_FAILURE);
        }
        // Add the new node to the list
        returnArgs->list[returnArgs->size] = node;
        returnArgs->size++;
    }

    else
    {
        returnArgs->list[returnArgs->size] = node;
        returnArgs->size++;
    }
}

void freeReturnArgs(ThreadReturnArgs returnArgs)
{
    for (int i = 0; i < returnArgs.size; i++)
    {
        safe_free((void **)&returnArgs.list[i]);
    }

    safe_free((void **)&returnArgs.list);
}