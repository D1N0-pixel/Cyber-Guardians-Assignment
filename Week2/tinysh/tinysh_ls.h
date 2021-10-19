#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int tinysh_ls(int argc, char **argv);
int listfile(char *path, char L, char A);
int ls_help();
int get_digit(int num);
int ls_l(char *path);
