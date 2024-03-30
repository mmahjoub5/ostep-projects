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
#include "../header/parser.h"
#include "../header/threadHelpers.h"

#define LISTSIZE 2048
#define NUMTHREADS 4

ThreadReturnArgs results[NUMTHREADS];
pthread_mutex_t result_mutex;
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

void *runThread(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    // printf("address %p\n", &args->size);
    // printf("\nprint sizes %zu\n", (size_t)args->size);
    // printf("\n thread id: %i \n", args->thread_id);
    // printf("\n string: %s \n", args->start);
    compress(&results[args->thread_id], args->start, &result_mutex);

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
    printf("chunksize %zu", chunkSize);
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

    for (int i = 0; i < NUMTHREADS - 1; i++)
    {
        int array_size_1 = results[i].size - 1;
        int array_size_2 = results[i + 1].size - 1;
        printf("thread id: %i, \n%c  %c\n", i, results[i].list[array_size_1]->letter, results[i + 1].list[0]->letter);

        if (results[i].list[array_size_1]->letter == results[i + 1].list[0]->letter)
        {
            printf("thread id: %i, \n%c\n", i, results[i].list[array_size_1]->letter);
            results[i + 1].list[0]->number += results[i].list[array_size_1]->number;
            results[i].list[array_size_1]->number = -1000;
        }
    }

    for (int i = 0; i < NUMTHREADS; i++)
    {

        for (int k = 0; k < results[i].size; k++)
        {
            if (results[i].list[k]->number == -1000)
            {
                continue;
            }

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