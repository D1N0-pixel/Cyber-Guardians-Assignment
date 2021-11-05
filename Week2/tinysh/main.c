#include "tinysh_cd.h"
#include "tinysh_exit.h"
#include "tinysh_help.h"
#include "tinysh_ls.h"
#include "tinysh_mv.h"
#include "builtin.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>

int (*builtin_func[]) (int, char **) = {
    &tinysh_cd,
    &tinysh_help,
    &tinysh_exit,
    &tinysh_mv,
    &tinysh_ls
};

void sig_handler(int signum) { }

int tinysh_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("tinysh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("tinysh");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int tinysh_execute(int argc, char **args) {
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < tinysh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(argc, args);
        }
    }

    return tinysh_launch(args);
}

char *tinysh_read_line(void) {
//#ifdef TINYSH_USE_STD_GETLINE
//    char *line = NULL;
//    ssize_t bufsize = 0; // have getline allocate a buffer for us
//    if (getline(&line, &bufsize, stdin) == -1) {
//        if (feof(stdin)) {
//            exit(EXIT_SUCCESS);  // We received an EOF
//        } else  {
//            perror("tinysh: getline\n");
//            exit(EXIT_FAILURE);
//        }
//    }
//    return line;
//#else
//#define TINYSH_RL_BUFSIZE 1024
//    int bufsize = TINYSH_RL_BUFSIZE;
//    int position = 0;
//    char *buffer = malloc(sizeof(char) * bufsize);
//    int c;
//
//    if (!buffer) {
//        fprintf(stderr, "tinysh: allocation error\n");
//        exit(EXIT_FAILURE);
//    }
//
//    while (1) {
//        // Read a character
//        c = getchar();
//
//        if (c == EOF) {
//            exit(EXIT_SUCCESS);
//        } else if (c == '\n') {
//            buffer[position] = '\0';
//            return buffer;
//		} else if (c == '\3') {
//			printf("\n> ");
//			position = -1;
//			buffer = malloc(sizeof(char) * bufsize);
//		} else {
//            buffer[position] = c;
//        }
//        position++;
//
//        // If we have exceeded the buffer, reallocate.
//        if (position >= bufsize) {
//            bufsize += TINYSH_RL_BUFSIZE;
//            buffer = realloc(buffer, bufsize);
//            if (!buffer) {
//                fprintf(stderr, "tinysh: allocation error\n");
//                exit(EXIT_FAILURE);
//            }
//        }
//    }
//#endif
	char *line;
	line = readline("> ");
	return line;
}

#define TINYSH_TOK_BUFSIZE 64
#define TINYSH_TOK_DELIM " \t\r\n\a"

char **tinysh_split_line(char *line)
{
    int bufsize = TINYSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;

    if (!tokens) {
        fprintf(stderr, "tinysh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TINYSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TINYSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "tinysh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TINYSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int tinysh_args_len(char **args) {
    int i;
    for (i = 0; args[i] != NULL; i++) {}
    return i;
}

void tinysh_loop(void)
{
    char *line;
    char **args;
    int argc;
    int status;

    do {
        line = tinysh_read_line();
        args = tinysh_split_line(line);
        argc = tinysh_args_len(args);
        status = tinysh_execute(argc, args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);
    tinysh_loop();

    return EXIT_SUCCESS;
}
