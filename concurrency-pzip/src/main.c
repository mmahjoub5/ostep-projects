#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysctl.h>

#define LISTSIZE 2048
#define NUMTHREADS 1

typedef struct
{
    int number;
    char letter;
} compressedNode;

typedef struct
{
    char *start;
    size_t size;
    int thread_id;
} ThreadArgs;

typedef struct
{
    compressedNode **list;
    size_t capacity;
    size_t size;
} ThreadReturnArgs;

ThreadReturnArgs results[NUMTHREADS];
pthread_mutex_t result_mutex;
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
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        // Add the new node to the list
        returnArgs->list[returnArgs->size] = node;
        returnArgs->size++;
    }
}

void freeReturnArgs(ThreadReturnArgs returnArgs)
{
    for (int i = 0; i < returnArgs.size; i++)
    {
        free(returnArgs.list[i]);
    }
    free(returnArgs.list);
}

void showCpuCores()
{
    int numCPU;
    size_t len = sizeof(numCPU);

    // Get the number of CPU cores
    if (sysctlbyname("hw.physicalcpu", &numCPU, &len, NULL, 0) == -1)
    {
        perror("sysctl");
        exit(1);
    }

    printf("Number of CPU cores: %d\n", numCPU);
}

compressedNode *initializeCompressedNode(int number, char letter)
{
    compressedNode *node = malloc(sizeof(compressedNode *));
    node->number = number;
    node->letter = letter;
    return node;
}

void compress(ThreadReturnArgs *returnArgs, char *str)
{
    size_t length = strlen(str);
    // printf("size of string %zu", length);
    int counter = 0;
    char prev = str[0];

    for (int i = 0; i < length; i++)
    {
        printf("%c", str[i]);
        if (prev == str[i])
        {
            counter++;
            if (i == length - 1)
            {

                pthread_mutex_lock(&result_mutex);
                printf("adding %c\n", prev);
                addToThreadReturnArgs(returnArgs, initializeCompressedNode(counter, prev));
                pthread_mutex_unlock(&result_mutex);
            }
        }
        else if (prev != str[i] && i == length - 1)
        {
            pthread_mutex_lock(&result_mutex);
            addToThreadReturnArgs(returnArgs, initializeCompressedNode(counter, prev));
            addToThreadReturnArgs(returnArgs, initializeCompressedNode(1, str[i]));
            pthread_mutex_unlock(&result_mutex);
        }
        else
        {
            pthread_mutex_lock(&result_mutex);
            printf("adding %c\n", prev);
            addToThreadReturnArgs(returnArgs, initializeCompressedNode(counter, str[i]));
            pthread_mutex_unlock(&result_mutex);
            counter = 1;
        }

        prev = str[i];
    }
}

void *runThread(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    // printf("address %p\n", &args->size);
    // printf("\nprint sizes %zu\n", (size_t)args->size);
    // printf("\n thread id: %i \n", args->thread_id);
    // printf("\n string: %s \n", args->start);
    compress(&results[args->thread_id], args->start);

    return NULL;
}

int main(int argc, char *argv[])
{
    showCpuCores();
    // read file using mmap
    int fd;
    struct stat sb;
    char *mapped;

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

    // Get information about the file
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        exit(1);
    }

    // Map the file into memory
    mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(fd);

    size_t chunkSize = sb.st_size / NUMTHREADS;
    // printf("chunksize %zu", chunkSize);
    pthread_t threads[NUMTHREADS];
    ThreadArgs *threadArgs[NUMTHREADS];
    for (int i = 0; i < NUMTHREADS; i++)
    {
        threadArgs[i] = malloc(sizeof(ThreadArgs)); // Allocating memory for each threadArgs
        threadArgs[i]->thread_id = i;
        // add error check
        if (threadArgs[i] == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        // Calculate size based on whether it's the last thread or not
        if (i == NUMTHREADS - 1)
        {
            threadArgs[i]->size = sb.st_size - (i * chunkSize);
        }
        else
        {
            threadArgs[i]->size = chunkSize;
        }
        // MAKE SURE TO FREE THIS
        threadArgs[i]->start = malloc(threadArgs[i]->size);
        int k = 0;

        for (k = 0; k < threadArgs[i]->size; k++)
        {
            threadArgs[i]->start[k] = mapped[k + i * chunkSize];
        }
        threadArgs[i]->start[k] = '\0';
        k = 0;
        // Print size information (optional)
        // printf("Thread %d: Start = %p, Size = %zu\n", i, threadArgs[i]->start, threadArgs[i]->size);

        // Create thread
        if (pthread_create(&threads[i], NULL, runThread, (void *)threadArgs[i]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUMTHREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUMTHREADS; i++)
    {
        for (int k = 0; k < results[i].size; k++)
        {

            printf(" \n thread id:%i hell %c  %i \n ", i, results[i].list[k]->letter, results[i].list[k]->number);
        }
    }
    // Unmap the file from memory
    if (munmap(mapped, sb.st_size) == -1)
    {
        perror("munmap");
        exit(1);
    }
}