#include "builtin.h"

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "mv",
    "ls"
};

int tinysh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}
