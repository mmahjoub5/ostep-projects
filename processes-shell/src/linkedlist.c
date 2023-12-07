#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
Node *createNode(char *fileName)
{
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return NULL;
    }
    // Allocate memory for the filename and copy the content
    newNode->filename = malloc(strlen(fileName) + 1);

    // Check if memory allocation was successful
    if (newNode->filename == NULL)
    {
        // Handle memory allocation failure (e.g., free the previously allocated node and return an error code)
        free(newNode);
        return NULL;
    }
    // Copy the content of fileName to newNode->filename
    strcpy(newNode->filename, fileName);

    // Set the next pointer to NULL (assuming it's the last node in the list)
    newNode->next = NULL;

    // Return the newly created node
    return newNode;
}

void push(char *pathName, List *list)
{
    Node *new_node;
    new_node = createNode(pathName);
    if (new_node == NULL)
    {
        return;
    }
    if (list->header == NULL)
    {
        list->header = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }
}
void printList(List *datalist)
{
    Node *temp = datalist->header;
    // printf("Print List: \n");
    while (temp != NULL)
    {
        // printf("--> %s \n", temp->filename);
        temp = temp->next;
    }
}

void clearList(List *datalist)
{
    Node *iter = datalist->header;

    while (iter != NULL)
    {
        /* code */
        free(iter);
        iter = iter->next;
    }
    datalist->header = NULL;
    datalist->tail = NULL;
}