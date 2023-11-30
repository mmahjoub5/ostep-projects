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

// struct node
// {
//     char *fileName;
//     struct node *next;
// };

// struct pathList
// {
//     struct node *header;
//     struct node *tail;
// };

// struct node *createNode(char *fileName)
// {
//     struct node *newNode = (struct node *)malloc(sizeof(struct node));
//     newNode->fileName = fileName;
//     return newNode;
// }

// void push(char *pathName, struct pathList *list)
// {
//     struct node *new_node;
//     new_node = createNode(pathName);
//     list->tail->next = new_node;
//     list->tail = new_node;
// }
// void printList(struct pathList *datalist)
// {
//     struct node *temp = datalist->header;
//     while (temp != NULL)
//     {
//         printf("%s", temp->fileName);
//         temp = temp->next;
//     }
// }