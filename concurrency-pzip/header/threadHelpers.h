// thread container struct

#ifndef THREADHELPERS_H
#define THREADHELPERS_H
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include <errno.h>
#include "global_vars.h"
// Forward declaration of the compressedNode struct
typedef struct compressedNode_Struct compressedNode;

typedef struct ThreadArgs_struct
{
    char *start;
    size_t size;
    int thread_id;
} ThreadArgs;

typedef struct ThreadReturnArgs
{
    compressedNode **list;
    size_t capacity;
    size_t size;
    struct ThreadReturnArgs *next;
} ThreadReturnArgs;

ThreadReturnArgs initializeThreadReturnArgs();
void addToThreadReturnArgs(ThreadReturnArgs *returnArgs, compressedNode *node);
void freeReturnArgs(ThreadReturnArgs returnArgs);

#endif