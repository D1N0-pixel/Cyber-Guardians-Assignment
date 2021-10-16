#include "tinysh_mv.h"

int tinysh_mv(int argc, char **argv) {
    if (argc < 3) {
        printf("mv: missing destination file operand after '%s'\n", argv[1]);
    } else if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            printf("mv: '%s' and '%s' are the same file\n", argv[1], argv[2]);
        }
        struct stat sb;
        if (stat(argv[2], &sb) == -1) {
            if (rename(argv[1], argv[2]) != 0) {
                printf("mv: cannot move '%s' to '%s'\n", argv[1], argv[2]);
            }
        } else if ((sb.st_mode & S_IFMT) == S_IFDIR) {
            move_to_dir(argv[1], argv[2]);
        } else {
            if (rename(argv[1], argv[2]) != 0) {
                printf("mv: cannot move '%s' to '%s'\n", argv[1], argv[2]);
            }
        }
    } else {
        char *last_argv = argv[argc - 1];
        if (last_argv[strlen(last_argv)-1] == '/') {
            last_argv[strlen(last_argv)-1] == '\0';
        }
        struct stat sb;
        if (stat(last_argv, &sb) == -1) {
            printf("mv: target '%s' is not a directory\n", last_argv);
        } else if ((sb.st_mode & S_IFMT) == S_IFDIR) {
            for (int i = 1; i < argc-1; ++i) {
                move_to_dir(argv[i], argv[argc-1]);
            }
        } else {
            printf("mv: target '%s' is not a directory\n",last_argv);
        }
    }
    return 1;
}

int move_to_dir(char *file, char *path) {
    if (strncmp(file, path, strlen(file)) == 0) {
        printf("mv: '%s' and '%s' are the same file\n", file, path);
        return -1;
    }
    char *new_name = malloc(sizeof(char) * (strlen(file) + strlen(path) + 2));
    sprintf(new_name, "%s/%s", path, file);
    if (rename(file, new_name) != 0) {
        printf("mv: cannot move '%s' to '%s\n", file, path);
    }
    return 0;
}
