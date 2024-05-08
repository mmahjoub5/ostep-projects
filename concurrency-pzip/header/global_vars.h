#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define CHUNK_SIZE 512
// Global mutex for the queue
extern pthread_mutex_t pc_lock, q_lock;
extern pthread_cond_t empty, fill;
void safe_free(void **ptr);
#endif /* GLOBAL_VARS_H */
