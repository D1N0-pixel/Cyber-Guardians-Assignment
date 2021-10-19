#include "tinysh_help.h"

int tinysh_help(int argc, char **argv) {
    int i;
    printf("D1N0's tiny shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    
    for (i = 0; i < tinysh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}
