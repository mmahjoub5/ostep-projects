#ifndef QUEUE_H
#define QUEUE_H

typedef struct  Page_struct
{
    /* data */
    char* data;
    long pageSz;
    long pageNum;
    long filenum;
    struct Page *next;
} Page;


typedef struct  QUEUE_STRUCT
{
    /* data */
    Page* Tail;
    int size;
}Queue;

void enque(Queue* queue, Page* node);
Page* dequeu(Queue* queue);


#endif