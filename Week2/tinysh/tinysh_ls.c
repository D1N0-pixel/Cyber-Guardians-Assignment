#include "tinysh_ls.h"

int tinysh_ls(int argc, char **argv) {
    char L = 0;
    char A = 0;
    char HELP = 0;
    char DR = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            HELP = 1;
            break;
        }
        if (argv[i][0] == '-') {
            for (int j = 1; j < strlen(argv[i]); ++j) {
                if (argv[i][j] == 'l') {
                    L = 1;
                }
                if (argv[i][j] == 'a') {
                    A = 1;
                }
            }
        } else {
            DR = DR <= 1 ? DR + 1 : 2;
        }
    }
    if (HELP) {
        ls_help();
        return 1;
    } 
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            struct stat sb;
            stat(argv[i], &sb);
            if ((sb.st_mode & S_IFMT) == S_IFDIR) {
                if (DR >= 2) {   
                    printf("%s:\n", argv[i]);
                }
                listfile(argv[i], L, A);
            } else {
                if (L) {
                    ls_l(argv[i]);
                } else {
                    printf("%s\n", argv[i]);
                }
            }
        }
    }
    if (!DR) {
        listfile(".\0", L, A);
    }
    return 1;
}

int ls_help() {
    printf("Usage: ls [OPTION]... [FILE]...\n");
    printf("List information about the FILEs (the current directory by default).\n");
    printf("options:\n");
    printf("  -a       do not ignore entries starting with .\n");
    printf("  -l       use a long listing format\n");
    printf("  --help   display this help and exit\n");
    return 0;
}

int listfile(char *path, char L, char A) {
    DIR * d = opendir(path);
    if (d == NULL)
        return -1;

    struct dirent * dir;
    while ((dir = readdir(d)) != NULL) {
        if (!A && dir->d_name[0] == '.') {
            continue;
        }
        if (L) {
            ls_l(dir->d_name);
        } else {
            printf("%s  ", dir->d_name);
        }
    }
    if (!L) {
        printf("\n");
    }
    closedir(d);
    return 0;
}

int get_digit(int num) {
    int digit = 1;
    while (num /= 10) digit++;
    return digit;
}

int ls_l(char *path) {
    struct stat sb;
    if (stat(path, &sb) == -1) {
        return -1;
    }

    // file type
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  printf("b");break;
        case S_IFCHR:  printf("c");break;
        case S_IFDIR:  printf("d");break;
        case S_IFIFO:  printf("p");break;
        case S_IFREG:  printf("-");break;
        case S_IFSOCK: printf("s");break;
        default:       printf("?");
    }

    // file permission
    if (sb.st_mode & S_IRWXU & S_IRUSR) printf("r"); else printf("-");
    if (sb.st_mode & S_IRWXU & S_IWUSR) printf("w"); else printf("-");
    if (sb.st_mode & S_IRWXU & S_IXUSR) printf("x"); else printf("-");
    if (sb.st_mode & S_IRWXG & S_IRGRP) printf("r"); else printf("-");
    if (sb.st_mode & S_IRWXG & S_IWGRP) printf("w"); else printf("-");
    if (sb.st_mode & S_IRWXG & S_IXGRP) printf("x"); else printf("-");
    if (sb.st_mode & S_IRWXO & S_IROTH) printf("r"); else printf("-");
    if (sb.st_mode & S_IRWXO & S_IWOTH) printf("w"); else printf("-");
    if (sb.st_mode & S_IRWXO & S_IXOTH) printf("x"); else printf("-");
    
    // hard link
    printf(" %ld", (long)sb.st_nlink);

    // owner
    printf(" %s", getpwuid(sb.st_uid)->pw_name);
    printf(" %s", getgrgid(sb.st_gid)->gr_name);

    // file size
    printf(" %lld", (long long)sb.st_size);

    // last modified
    printf(" %s", strtok(ctime(&sb.st_mtime), "\n"));

    // file name
    printf(" %s\n", path);
    return 0;
}
