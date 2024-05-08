
#include "../header/producerConsumer.h"

/*
    Map entire file and break it into chunks

*/

void *Producer2(void *arg)
{

    ProducerArgs *args = (ProducerArgs *)arg;
    for (int fileIndex = 0; fileIndex < args->num_files; fileIndex++)
    {

        // printf("\nPRODUCER THREAD ID: %i  Q SIZE %i\n ", args->id, args->q->size);
        int fd;
        struct stat sb;
        fd = open(args->file_names[fileIndex], O_RDWR);

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

        off_t fileSize = sb.st_size;
        char *fileMap = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (fileMap == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            printf("Error: %d\n", errno);
            exit(1);
        }
        size_t num_chunks = (sb.st_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
        // printf("num of chunks: %li\n", num_chunks);
        for (int i = 0; i < num_chunks; i++)
        {

            Page *page = (Page *)malloc(sizeof(Page));
            if (page == NULL)
            {
                fprintf(stderr, "QUUE RETURNED NULL\n");
                exit(EXIT_FAILURE);
            }
            page->pageNum = i;
            page->filenum = 0;
            page->size = 0;
            page->next = NULL;

            if (i == num_chunks - 1)
            {
                page->size = fileSize - (i * CHUNK_SIZE);
            }
            else
            {
                page->size = CHUNK_SIZE;
            }
            // Copy data from fileMap into the page's data field
            memcpy(page->data, fileMap + i * CHUNK_SIZE, page->size);
            pthread_mutex_lock(&pc_lock);
            while (IS_FULL(args->q))
            {
                printf("we are heere");
                if (fileIndex + 1 == args->num_files && i + 1 == num_chunks)
                {
                    args->q->finished = 1;
                }
                pthread_cond_wait(&empty, &pc_lock);
            }

            ENQUEUE(args->q, page);

            pthread_cond_signal(&fill);
            pthread_mutex_unlock(&pc_lock);

            // printf("%s", temp->data);
        }
        if (munmap(fileMap, sb.st_size) == -1)
        {
            perror("munmap");
            exit(1);
        }
        close(fd);
    }
    pthread_mutex_lock(&pc_lock);
    printf("\nwe are done reading the filee\n");
    args->q->finished = 1;
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&pc_lock);
    return NULL;
}

void ENQUE_WRAPPER(ThreadReturnArgs *compressedData, OutputBuffer *b)
{
    pthread_mutex_lock(&q_lock);
    // QUUEUE EMPTY
    if (b->Tail == NULL)
    {
        ENQUEUE(b, compressedData);
        pthread_mutex_unlock(&q_lock);
        return;
    }

    int size = b->Tail->size - 1;
    compressedNode **list = b->Tail->list;
    if (list[size]->letter == compressedData->list[0]->letter)
    {
        if (compressedData->size > 1)
        {
            printf("there is another thing to add to queue, %c %i", compressedData->list[1]->letter, compressedData->list[1]->number);
            safe_free((void **)&compressedData->list[0]); // Free memory of the first element
            compressedData->list++;
            compressedData->size--;
            ENQUEUE(b, compressedData);
        }
        else
        {
            safe_free((void **)&compressedData->list);
            safe_free((void **)&compressedData);
        }
    }
    else
    {
        ENQUEUE(b, compressedData);
    }
    pthread_mutex_unlock(&q_lock);
    return;
}

void *Consumer(void *arg)
{
    ConsumerArgs *args = (ConsumerArgs *)arg;
    Queue *q = args->q;
    OutputBuffer *outBuffer = args->output;
    int k = 0;

    while (1)
    {
        k++;

        pthread_mutex_lock(&pc_lock);

        while (IS_EMPTY(q))
        {

            if (q->finished == 1)
            {

                //  jank solutiion but wakes up sleeping consumer threads
                pthread_cond_signal(&fill);
                pthread_mutex_unlock(&pc_lock);
                return NULL;
            }
            // printf("\nwe are sleeeping:     %i      is q finished: %i \n", args->id, q->finished);
            pthread_cond_wait(&fill, &pc_lock);
        }

        Page *node = DEQUEUE(q, Page);
        if (node == NULL)
        {
            fprintf(stderr, "QUUE RETURNED NULL\n");
            exit(EXIT_FAILURE);
        }
        // compress
        ThreadReturnArgs *compressedData = (ThreadReturnArgs *)malloc(sizeof(ThreadReturnArgs));
        if (compressedData == NULL)
        {
            // Handle memory allocation failure
            fprintf(stderr, "Failed to allocate memory\n");
            exit(EXIT_FAILURE);
        }

        compressedData->capacity = 0;
        compressedData->size = 0;
        compressedData->list = NULL;
        compressedData->next = NULL;
        compress(compressedData, node->data);
        pthread_mutex_lock(&q_lock);
        free(node);
        node = NULL;
        pthread_mutex_unlock(&q_lock);

        ENQUE_WRAPPER(compressedData, outBuffer);

        // this should directly write to a file so change the std output

        pthread_cond_signal(&empty);

        pthread_mutex_unlock(&pc_lock);

        // printf("\nCONSUMER THREAD ID: %i is unlocking \n", args->id);
    }
}

/*
    map & unmmap parts of file to memory solution, however not a working solution
    bug: tbd

*/
// void *Producer(void *arg)
// {

//     ProducerArgs *args = (ProducerArgs *)arg;
//     int fd;
//     struct stat sb;

//     printf("\nfile name %s\n", args->file_names[0]);
//     fd = open(args->file_names[0], O_RDWR);
//     printf("\n%i", fd);
//     printf("\nfile name %s\n", args->file_names[0]);
//     printf("\n%i", fd);
//     off_t offset = 0;
//     if (fd == -1)
//     {
//         perror("open");
//         exit(1);
//     }

//     // Get information about the file
//     if (fstat(fd, &sb) == -1)
//     {
//         perror("fstat");
//         exit(1);
//     }

//     off_t fileSize = sb.st_size;
//     off_t pageSize = sysconf(_SC_PAGESIZE);
//     // off_t systemPageSize =  // Get the system's page size
//     off_t length = pageSize;
//     printf("length: %li", length);

//     while (1)
//     {
//         // printf("\nfile size: %li\n ", fileSize);
//         // printf("length: %li\n ", length);
//         // printf("offset %li\n ", offset);
//         // printf("file descriptor: %i\n", fd);

//         Page *page = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, offset);

//         if (page == MAP_FAILED)
//         {
//             perror("mmap");
//             close(fd);
//             printf("Error: %d\n", errno);
//             exit(1);
//         }

//         pthread_mutex_lock(&pc_lock);
//         page->pageNum = args->q->size;

//         while (isFull(args->q))
//         {
//             pthread_cond_wait(&empty, &pc_lock);
//         }
//         ENQUEUE(args->q, page);

//         pthread_cond_signal(&fill);
//         pthread_mutex_unlock(&pc_lock);
//         // munmap(page, length);

//         offset += length;
//         offset = offset - (offset % pageSize); // Align the offset to the nearest page boundary
//         if (offset >= fileSize)
//         {
//             offset = 0; // Reset offset to the beginning of the file if reached the end
//             return NULL;
//         }
//         Page *temp = DEQUE(args->q);
//         printf("\n print this dat%s\n", temp->data);
//     }
//     free(args);
// }