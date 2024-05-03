#ifndef QUEUE_H
#define QUEUE_H

#include "global_vars.h"
#include <stdio.h>
#include <stdlib.h>
#define CHAR_BUFFER_SZ 1024
#define MAX_Q 100
#define MIN_Q 0
typedef struct Page
{
    /* data */
    char data[CHUNK_SIZE];
    long size;
    long pageNum;
    long filenum;
    struct Page *next;
} Page;

typedef struct QUEUE_STRUCT
{
    /* data */
    Page *Head;
    Page *Tail;
    int finished;
    int size;

} Queue;

typedef struct
{
    ThreadReturnArgs *Head;
    ThreadReturnArgs *Tail;
    int finished;
    int size;
} OutputBuffer;

#define MAX_Q_SIZE 100
// void initQueue(Queue *q);
// void enque(Queue *queue, void *node);
// Page *dequeu(Queue *queue);
// int isFull(Queue *q);
// int isEmpty(Queue *q);

#define INIT_QUEUE(q)      \
    do                     \
    {                      \
        (q)->Head = NULL;  \
        (q)->Tail = NULL;  \
        (q)->size = 0;     \
        (q)->finished = 0; \
    } while (0)

#define ENQUEUE(queue, node)              \
    do                                    \
    {                                     \
        if ((queue)->Tail == NULL)        \
        {                                 \
            (queue)->Head = (node);       \
            (queue)->Tail = (node);       \
            (queue)->size++;              \
        }                                 \
        else                              \
        {                                 \
            (queue)->Tail->next = (node); \
            (queue)->Tail = (node);       \
            (queue)->size++;              \
        }                                 \
    } while (0)

#define DEQUEUE(queue, type)                                                  \
    ({                                                                        \
        type *nodeToReturn = (queue)->Head;                                   \
        if ((queue)->Head == NULL)                                            \
        {                                                                     \
            printf("THERE WAS AN ERROR SHOULD NOT BE REMOVING ANYTHING!!\n"); \
            nodeToReturn = NULL;                                              \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            (queue)->Head = (queue)->Head->next;                              \
            (queue)->size--;                                                  \
            if ((queue)->Head == NULL)                                        \
            {                                                                 \
                (queue)->Tail = NULL;                                         \
            }                                                                 \
        }                                                                     \
        nodeToReturn;                                                         \
    })

#define IS_FULL(q) ((q)->size == MAX_Q ? 1 : 0)

#define IS_EMPTY(q) ((q)->size == MIN_Q ? 1 : 0)

#endif
