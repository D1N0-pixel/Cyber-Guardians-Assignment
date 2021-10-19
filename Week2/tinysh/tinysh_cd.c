#include "tinysh_cd.h"

int tinysh_cd(int argc, char **argv) {
    if (argv[1] == NULL) {
        fprintf(stderr, "tinysh: expected argument to \"cd\"\n");
    } else {
        if (chdir(argv[1]) != 0) {
            perror("tinysh");
        }
    }
    return 1;
}
