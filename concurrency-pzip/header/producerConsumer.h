#ifndef PRODUCERCONSUMER_H
#define PRODUCERCONSUMER_H
#include <pthread.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "global_vars.h"
#include "parser.h"
#include "Queue.h"
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define MAX_FILE_NUM 10

typedef struct ProducerArgs_struct
{
    Queue *q;
    char *file_names[MAX_FILE_NUM];
    int num_files;
    int id;
} ProducerArgs;

typedef struct
{
    Queue *q;
    OutputBuffer *output;
    int id;
} ConsumerArgs;

void *
Producer(void *arg);
void *Consumer(void *arg);
void *Producer2(void *arg);
// ProducerArgs* producerArgs_init(Queue* q, int id, int num_files, char*fil)
#endif