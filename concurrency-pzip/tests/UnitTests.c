#include <stdio.h>
#include <stdlib.h>
#include "../header/threadHelpers.h"
#include "../header/parser.h"
#include "../header/Queue.h"
#include "../header/producerConsumer.h"
#include <assert.h>
#include <pthread.h>
// Define the structures used by the function

// Test function
void test_addToThreadReturnArgs()
{
    ThreadReturnArgs returnArgs;
    returnArgs.size = 0;
    returnArgs.capacity = 0;
    returnArgs.list = NULL;

    compressedNode node1, node2;
    // You can initialize node1 and node2 as needed for testing

    // Add node1 to the list
    addToThreadReturnArgs(&returnArgs, &node1);
    if (returnArgs.size != 1)
    {
        printf("Test failed: Incorrect size after adding node1\n");
    }

    // Add node2 to the list
    addToThreadReturnArgs(&returnArgs, &node2);
    if (returnArgs.size != 2)
    {
        printf("Test failed: Incorrect size after adding node2\n");
    }

    // Test if the correct nodes are added to the list
    if (returnArgs.list[0] != &node1 || returnArgs.list[1] != &node2)
    {
        printf("Test failed: Incorrect nodes added to the list\n");
    }

    // Free memory allocated for list
    free(returnArgs.list);
}

void add_multiple_loop(int num)
{
    ThreadReturnArgs returnArgs;
    returnArgs.size = 0;
    returnArgs.capacity = 0;
    returnArgs.list = NULL;

    compressedNode *nodes = malloc(num * sizeof(compressedNode));

    for (int i = 0; i < num; i++)
    {
        addToThreadReturnArgs(&returnArgs, &nodes[i]);
    }
    if (returnArgs.size != num)
    {
        printf("\nTest FAILDED test_addToThreadReturnArgs_variable failed: Incorrect size %zu after adding nodes EXPECTED: %i\n", returnArgs.size, num);
    }

    free(nodes);
}

void enque_test()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    INIT_QUEUE(q);
    Page *p = malloc(sizeof(Page));
    ENQUEUE(q, p);

    free(q);
    free(p);
}

void fill_queue(int num)
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    INIT_QUEUE(q);
    Page *p = malloc(sizeof(Page) * num);

    for (int i = 0; i < num; i++)
    {
        ENQUEUE(q, &p[i]);
    }
    if (q->size != num)
    {
        printf("QUEUE  SIZE WRONG %i\n", q->size);
    }

    free(q);
    free(p);
}

void fill_empty_queue(int num)
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    INIT_QUEUE(q);
    Page *p = malloc(sizeof(Page) * num);

    for (int i = 0; i < num; i++)
    {
        ENQUEUE(q, &p[i]);
    }
    if (q->size != num)
    {
        printf("QUEUE  SIZE WRONG %i\n", q->size);
    }
    for (int i = 0; i < num; i++)
    {
        void *temp = DEQUEUE(q, void);
    }
    if (q->size != 0)
    {
        printf("QUEUE  SIZE WRONG %i\n", q->size);
    }

    free(q);
    free(p);
}

void deque_test()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    INIT_QUEUE(q);
    Page *p = malloc(sizeof(Page));
    p->pageNum = 10;
    ENQUEUE(q, p);
    assert(q->size == 1);
    p = DEQUEUE(q, Page);
    assert(q->size == 0);
    assert(p->pageNum == 10);

    free(q);
    free(p);
}

int main()
{
    // test_addToThreadReturnArgs();
    // add_multiple_loop(15);
    // fill_queue(5);
    // deque_test();

    // fill_empty_queue(10);

    printf("Tests complete.\n");

    return 0;
}
