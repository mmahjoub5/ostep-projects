#include "../header/global_vars.h"

// Define the global mutex for the queue
pthread_mutex_t pc_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;