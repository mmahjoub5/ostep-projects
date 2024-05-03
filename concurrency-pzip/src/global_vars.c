#include "../header/global_vars.h"

// Define the global mutex for the queue
pthread_mutex_t pc_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

// Define a function to safely free memory
void safe_free(void **ptr)
{
    if (*ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL; // Optional: Set pointer to NULL after freeing to avoid dangling pointers
    }
    else
    {
        fprintf(stderr, "Attempted to free NULL pointer. Aborting program.\n");
        exit(EXIT_FAILURE);
    }
}
