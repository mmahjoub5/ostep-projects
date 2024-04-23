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
    producerArgs->file_names[0] = "hello.txt";
    // producerArgs->file_names[1] = "anothertest.txt";
    producerArgs->num_files = 1;
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

    while (outputBuffer->size != 0)
    {
        ThreadReturnArgs *compressedData = DEQUEUE(outputBuffer, ThreadReturnArgs);
        compressedNode **list = compressedData->list;
        for (int k = 0; k < compressedData->size; k++)
        {
            if (isspace(list[k]->letter))
            {
                printf("\nwe have white space in this\n");
            }
            printf(" \nhere:  %c  %i \n ", list[k]->letter, list[k]->number);
        }
        freeReturnArgs(*compressedData);
        free(compressedData);
    }

    free(q);
    free(producerArgs);
    free(consumerArgs2);
    free(consumerArgs);
    free(outputBuffer);

    return 0;
}
