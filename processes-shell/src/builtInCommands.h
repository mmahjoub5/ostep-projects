#ifndef BUILTINCOMMANDS_H
#define BUILTINCOMMANDS_H
#include "constants.h"
void builtin_cd(char **cmd_argv, int numCLIArgs);
void builtin_path(char **cmd_argv, char *paths[PATHNAMESIZE]);
void builtin_loop(char **cmd_argv, char *paths[PATHNAMESIZE]);
void runBuiltInCommand(int numCLIArgs, char **cmd_argv, char *paths[PATHNAMESIZE]);

#endif