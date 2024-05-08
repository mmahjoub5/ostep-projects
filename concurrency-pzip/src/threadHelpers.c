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
        // Calculate the new capacity
        int new_capacity = (returnArgs->capacity == 0) ? 1 : returnArgs->capacity * 2;
        compressedNode **temp;
        if (returnArgs->list == NULL)
        {
            temp = (compressedNode **)malloc(sizeof(compressedNode *) * new_capacity);
        }
        else
        {
            temp = realloc(returnArgs->list, new_capacity * sizeof(compressedNode *));
        }

        if (temp == NULL)
        {
            // Handle memory allocation failure
            perror("realloc");
            printf("Memory reallocation failed!\n");
            exit(EXIT_FAILURE);
        }
        // Update the list and capacity
        returnArgs->list = temp;
        returnArgs->capacity = new_capacity;
    }
    // Add the new node to the array
    returnArgs->list[returnArgs->size++] = node;
}

void freeReturnArgs(ThreadReturnArgs returnArgs)
{
    for (int i = 0; i < returnArgs.size; i++)
    {
        free(returnArgs.list[i]);
    }
    // printf("\n\nsize of list:: \n\n %zu", sizeof(returnArgs.list));
    free(returnArgs.list);
}