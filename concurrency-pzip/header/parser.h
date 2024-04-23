

#ifndef PARSER_H
#define PARSER_H
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "threadHelpers.h"
typedef struct compressedNode_Struct
{
    int number;
    char letter;
} compressedNode;

compressedNode *initializeCompressedNode(int number, char letter);
void compress(ThreadReturnArgs *returnArgs, char *str);

#endif