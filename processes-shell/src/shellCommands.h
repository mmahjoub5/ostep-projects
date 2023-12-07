#ifndef IO_H
#define IO_H
#include "constants.h"
#include <stdbool.h>

void removeWhiteSpace(char *str);
bool checkIfShellCommand(char **command, char *paths[PATHNAMESIZE]);
void executeShellCommand(char **cmd_argv, int loopNum);
void printToFile(char *filename);
#endif