#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node
{
    char *filename;
    struct Node *next;
} Node;

typedef struct List
{
    struct Node *header;
    struct Node *tail;
} List;

struct Node *createNode(char *fileName);
void push(char *pathName, List *list);

void printList(List *datalist);

void clearList(List *datalist);
#endif
