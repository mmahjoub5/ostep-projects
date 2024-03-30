#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 1

typedef struct
{
    size_t start;
    size_t size;
} ThreadArgs;

void *runThread(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    printf("Thread: Start = %zu, Size = %zu\n", args->start, args->size);
    free(args); // Free memory allocated for ThreadArgs
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    size_t mapped = 0;                         // Example start position for demonstration
    size_t fileSize = 512;                     // Example total file size for demonstration
    size_t chunkSize = fileSize / NUM_THREADS; // Example chunk size for demonstration
    for (int i = 0; i < NUM_THREADS; i++)
    {
        ThreadArgs *threadArgs = malloc(sizeof(ThreadArgs)); // Allocating memory for each threadArgs

        // add error check
        if (threadArgs == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // Calculate start position for each thread
        threadArgs->start = i * chunkSize + mapped;

        // Calculate size based on whether it's the last thread or not
        if (i == NUM_THREADS - 1)
        {
            threadArgs->size = fileSize - (i * chunkSize);
        }
        else
        {
            threadArgs->size = chunkSize;
        }

        // Print size information (optional)
        printf("Thread %d: Start = %zu, Size = %zu\n", i, threadArgs->start, threadArgs->size);

        // Create thread
        if (pthread_create(&threads[i], NULL, runThread, (void *)threadArgs) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
