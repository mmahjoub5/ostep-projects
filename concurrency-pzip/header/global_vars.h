#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <pthread.h>
#define CHUNK_SIZE 16
// Global mutex for the queue
extern pthread_mutex_t pc_lock;
extern pthread_cond_t empty, fill;

#endif /* GLOBAL_VARS_H */
