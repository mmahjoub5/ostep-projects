// #include "../header/Queue.h"

// void initQueue(Queue *q)
// {
//     q->Head = NULL;
//     q->Tail = NULL;
//     q->size = 0;
//     q->finished = 0;
// }

// void enque(Queue *queue, Page *node)
// {
//     if (queue->Tail == NULL)
//     {
//         queue->Head = node;
//         queue->Tail = node;
//         queue->size++;
//         return;
//     }
//     else
//     {
//         queue->Tail->next = node;
//         queue->Tail = node;
//         queue->size++;
//     }
//     printf("adding to quue%i\n", queue->size);
// }

// Page *dequeu(Queue *queue)
// {
//     if (queue->Head == NULL)
//     {
//         printf("THERE WAS AN ERROR SHOULD NOT BE REMOVINF ANYTHING!!");
//         return NULL;
//     }
//     Page *nodeToReturn = queue->Head;
//     queue->Head = queue->Head->next;
//     queue->size--;
//     if (queue->Head == NULL)
//     {
//         queue->Tail = NULL;
//     }

//     return nodeToReturn;
// }

// int isFull(Queue *q)
// {
//     if (q->size == 10)
//     {
//         return 1;
//     }
//     return 0;
// }
// int isEmpty(Queue *q)
// {
//     if (q->size == 0)
//     {
//         return 1;
//     }
//     return 0;
// }