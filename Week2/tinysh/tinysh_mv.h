#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tinysh_mv(int argc, char **args);
int move_to_dir(char *file, char *path);
