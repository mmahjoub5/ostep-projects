#include <stdio.h>
#include <stdlib.h>
#include "../header/threadHelpers.h"
#include "../header/parser.h"
#include "../header/Queue.h"
#include "../header/producerConsumer.h"
#include <assert.h>
#include <pthread.h>

int main()
{
    printf("\nchunk size: %i\n", CHUNK_SIZE);
    ProducerArgs *producerArgs = (ProducerArgs *)malloc(sizeof(ProducerArgs));
    ConsumerArgs *consumerArgs = (ConsumerArgs *)malloc(sizeof(ConsumerArgs));
    ConsumerArgs *consumerArgs2 = (ConsumerArgs *)malloc(sizeof(ConsumerArgs));

    Queue *q = (Queue *)malloc(sizeof(Queue));
    OutputBuffer *outputBuffer = (OutputBuffer *)malloc(sizeof(OutputBuffer));

    producerArgs->q = q;
    producerArgs->id = -100;
    consumerArgs->q = q;
    consumerArgs->output = outputBuffer;
    consumerArgs->id = 0;
    consumerArgs2->q = q;
    consumerArgs2->output = outputBuffer;
    consumerArgs->id = 1;

    INIT_QUEUE(q);
    INIT_QUEUE(outputBuffer);
    // producerArgs->file_names[0] = "hello.txt";
    producerArgs->file_names[2] = "hello.txt";
    producerArgs->file_names[0] = "main.txt";
    producerArgs->file_names[1] = "test2.txt";

    producerArgs->num_files = 3;
    /*
        one producer
        one consumer
    */
    int numThreads = 4;
    pthread_t threads[numThreads];

    if (pthread_create(&threads[0], NULL, Producer2, (void *)producerArgs) != 0)
    {
        printf("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&threads[1], NULL, Consumer, (void *)consumerArgs) != 0)
    {
        printf("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&threads[2], NULL, Consumer, (void *)consumerArgs2) != 0)
    {
        printf("pthread_create");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
    printf("\nthreads stopped runnign\n");
    int w = 0;
    while (outputBuffer->size != 0)
    {
        ThreadReturnArgs *compressedData = DEQUEUE(outputBuffer, ThreadReturnArgs);
        compressedNode **list = compressedData->list;
        for (int k = 0; k < compressedData->size; k++)
        {

            printf(" \nhere:  %c  %i \n ", list[k]->letter, list[k]->number);
        }
        // printf("\nmemory addres of list: %p i\n", (void *)list);
        // printf("\n %zu node number %i\n", sizeof(list), w);
        if (compressedData != NULL)
        {
            freeReturnArgs(*compressedData);
            safe_free((void **)&compressedData);
        }
        w++;
    }

    safe_free((void **)&q);
    safe_free((void **)&producerArgs);
    safe_free((void **)&consumerArgs2);
    safe_free((void **)&consumerArgs);
    safe_free((void **)&outputBuffer);

    return 0;
}

// void *printToFile(void *arg)
// {

//     while (outputBuffer->size != 0)
//     {
//         ThreadReturnArgs *compressedData = DEQUEUE(outputBuffer, ThreadReturnArgs);
//         compressedNode **list = compressedData->list;
//         for (int k = 0; k < compressedData->size; k++)
//         {
//             if (isspace(list[k]->letter))
//             {
//                 printf("\nwe have white space in this\n");
//             }
//             printf(" \nhere:  %c  %i \n ", list[k]->letter, list[k]->number);
//         }
//         freeReturnArgs(*compressedData);
//         free(compressedData);
//     }
// }
