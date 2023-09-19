/* header files go up here */
/* note that C comments are enclosed within a slash and
a star, and may wrap over lines */
// two slashes work too (and may be preferred)
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// main returns an integer

char FILENAME[] = "./database.txt";

// TODO: add this
typedef struct
{
    int key;
    char *name;
} keyPair;

struct node
{
    int key;
    char *name;
    struct node *next;
};

struct list
{
    struct node *header;
    struct node *tail;
};

void removeWhiteSpace(char *str)
{
    int readIndex = 0;
    int writeIndex = 0;

    while (str[readIndex])
    {
        if (!isspace((unsigned char)str[readIndex]))
        {
            str[writeIndex++] = str[readIndex];
        }
        readIndex++;
    }

    str[writeIndex] = '\0'; // Null-terminate the resulting string
}

void clearList(struct list *datalist)
{
    struct node *iter = datalist->header;

    while (iter != NULL)
    {
        /* code */
        free(iter);
        iter = iter->next;
    }
    datalist->header = NULL;
    datalist->tail = NULL;
}

struct node *createNode(int key, char *name)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->key = key;
    newNode->name = name;
    return newNode;
}

void printList(struct list *datalist)
{
    struct node *temp = datalist->header;
    while (temp != NULL)
    {
        printf("\n\n%d\t%s\n\n", temp->key, temp->name);
        temp = temp->next;
    }
}
void insert(struct list *dataList, int key, char *name)
{
    // list is empty
    if (dataList->header == NULL && dataList->tail == NULL)
    {
        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        newNode->key = key;
        newNode->name = name;
        newNode->next = NULL;
        dataList->header = newNode;
        dataList->tail = newNode;
        return;
    }

    // change head
    if (dataList->header->key > key)
    {
        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        newNode->key = key;
        newNode->name = name;
        newNode->next = dataList->header;
        dataList->header = newNode;
        return;
    }

    // change tail
    if (dataList->tail->key < key)
    {
        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        newNode->key = key;
        newNode->name = name;
        newNode->next = NULL;
        dataList->tail->next = newNode;
        dataList->tail = newNode;
        return;
    }

    // sorted insert
    struct node *iter = dataList->header;
    while (iter->next != NULL && iter->next->key < key && iter->key != key)
    {
        iter = iter->next;
    }

    if (iter->next->key == key)
    {
        iter->next->name = name;
    }
    else if (iter->key == key)
    {
        iter->name = name;
    }
    else
    {
        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        //  check this
        newNode->key = key;
        newNode->name = name;
        newNode->next = iter->next;
        iter->next = newNode;
    }
}

void deleteNode(struct list *datalist, int key)
{
    struct node *iter = datalist->header;
    struct node *prev;
    if (iter != NULL && iter->key == key)
    {
        datalist->header = iter->next;
        free(iter);
        return;
    }
    while (iter != NULL && iter->key != key)
    {
        prev = iter;
        iter = iter->next;
    }

    if (iter == NULL)
    {
        printf("KEY NOT IN LIST \n");
        return;
    }

    prev->next = iter->next;
    free(iter);
}

struct node *getNode(struct list *dataList, int key)
{
    struct node *iter = dataList->header;
    if (iter != NULL && iter->key == key)
    {
        return iter;
    }
    while (iter != NULL && iter->key != key)
    {
        iter = iter->next;
    }

    return iter;
}

void loadFile(struct list *datalist)
{
    if (datalist->header != NULL)
    {
        clearList(datalist);
    }
    FILE *fp;
    // CHECK IF FILE exsits
    fp = fopen(FILENAME, "r");
    if (fp != NULL)
    {
        printf("file exists!\n");
    }
    else
    {
        fp = fopen(FILENAME, "w");
        assert(fp);
    }
    char *buffer;
    size_t bufsize = 0;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL)
    {
        perror("Unable to allocate buffer\n");
        exit(1);
    }
    fp = fopen(FILENAME, "r");

    // loop to read all the lines
    while ((characters = getline(&buffer, &bufsize, fp) != -1))
    {
        printf("line read%s ", buffer);
        char *token, *string, *tofree;

        tofree = string = strdup(buffer);
        assert(string != NULL);
        int k = 0;
        char *parts[3];
        while ((token = strsep(&string, ",")) != NULL)
        {
            removeWhiteSpace(token);
            parts[k] = strdup(token);

            k++;
            // printf("toke: %s", token);
        }
        printf("%s", parts[1]);
        insert(datalist, atoi(parts[0]), parts[1]);
        free(tofree);

        // why does this break it??
        // for (int i = 0; i < k; i++)
        // {
        //     free(parts[i]);
        // }
    }

    if (buffer)
    {
        free(buffer);
    }
    fclose(fp);
}

char *intToString(int number)
{
    int buffer_size = snprintf(NULL, 0, "%d", number);
    if (buffer_size < 0)
    {
        return NULL; // Error handling
    }

    char *buffer = (char *)malloc(buffer_size + 1); // +1 for null terminator
    if (buffer == NULL)
    {
        return NULL; // Error handling
    }

    snprintf(buffer, buffer_size + 1, "%d", number); // Convert int to string

    return buffer;
}

char *concatenateWithComma(const char *str1, const char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t totalLen = len1 + 1 + len2 + 1; // +1 for the comma and +1 for the null terminator

    char *result = (char *)malloc(totalLen);
    if (result == NULL)
    {
        return NULL; // Memory allocation failed
    }

    snprintf(result, totalLen, "%s,%s", str1, str2);

    return result;
}
// TODO: final function then will be functioning
void saveToFile(struct list *datalist)
{
    FILE *fp;
    // CHECK IF FILE exsits
    fp = fopen(FILENAME, "o");
    if (fp != NULL)
    {
        printf("file exists!\n");
    }
    else
    {
        fp = fopen(FILENAME, "w");
        assert(fp);
    }

    // write a file
    struct node *iter = datalist->header;
    while (iter != NULL)
    {
        char *c = intToString(iter->key);
        char *finalString = concatenateWithComma(c, iter->name);
        printf("string to print to file: %s\n", finalString);
        fprintf(fp, "%s\n ", finalString);
        free(c);
        free(finalString);
        iter = iter->next;
    }
    fclose(fp);
}

/*
    - read in file to data structute (fille linked list)
    - parse input
    - get, put into sorted linked list
    - output to file at the end of program
*/

int main(int argc, char *argv[])
{
    struct list dataList = {NULL, NULL};
    loadFile(&dataList);
    // printList(&dataList);
    // return 0;
    // read in CLI
    for (int i; i < argc; i++)
    {
        // printf("%s\n", argv[i]);
        char *token, *string, *tofree;

        tofree = string = strdup(argv[i]);
        assert(string != NULL);
        int k = 0;
        char *parts[3];
        while ((token = strsep(&string, ",")) != NULL)
        {
            parts[k] = token;
            k++;
        }

        // printf("%s\n", parts[0]);
        if (strcmp(parts[0], "g") == 0)
        {
            // get value from file with key
            printf("get value from database\t :");
            struct node *temp = getNode(&dataList, atoi(parts[1]));
            if (temp != NULL)
            {
                printf("%d\t%s\n", temp->key, temp->name);
            }
            else
            {
                printf("not in database\n");
            }
        }
        else if (strcmp(parts[0], "p") == 0)
        {
            // get value from file with key
            printf("put value into database\t: %s\t%s\n", parts[1], parts[2]);
            char *c = strdup(parts[2]);
            insert(&dataList, atoi(parts[1]), c);
        }
        else if (strcmp(parts[0], "d") == 0)
        {
            printf("delete node with key:%s ", parts[1]);
            deleteNode(&dataList, atoi(parts[1]));
        }
        else if (strcmp(parts[0], "a") == 0)
        {
            printList(&dataList);
        }
        else if (strcmp(parts[0], "c") == 0)
        {
            clearList(&dataList);
        }
        else
        {
            printf("format error NEED G OR P\n");
        }

        free(tofree);
    }
    saveToFile(&dataList);
    return (0);
}