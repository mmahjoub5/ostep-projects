#include <stdio.h>
#include <stdlib.h>
#include "../header/threadHelpers.h"
#include "../header/parser.h"
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

int main()
{
    test_addToThreadReturnArgs();
    add_multiple_loop(15);
    printf("Tests complete.\n");
    return 0;
}
