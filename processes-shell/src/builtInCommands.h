#ifndef BUILTINCOMMANDS_H
#define BUILTINCOMMANDS_H
#include "constants.h"
static void builtin_cd(char **cmd_argv, int numCLIArgs);
static void builtin_path(char **cmd_argv, char *paths[PATHNAMESIZE]);
static void builtin_loop(char **cmd_argv, char *paths[PATHNAMESIZE]);
static void builtin_exit(int numCLIArgs);
void runBuiltInCommand(int numCLIArgs, char **cmd_argv, char *paths[PATHNAMESIZE]);

#endif