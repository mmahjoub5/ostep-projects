#include "io.h"
#include <ctype.h>

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