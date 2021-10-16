all: tinysh

tinysh: main.o tinysh_cd.o tinysh_exit.o tinysh_help.o tinysh_ls.o tinysh_mv.o builtin.o
		gcc -o tinysh main.o tinysh_cd.o tinysh_exit.o tinysh_help.o tinysh_ls.o tinysh_mv.o builtin.o

tinysh_cd.o: tinysh_cd.c tinysh_cd.h

tinysh_exit.o: tinysh_exit.c tinysh_exit.h

tinysh_help.o: tinysh_help.c tinysh_help.h builtin.o
		gcc -c -o tinysh_help.o tinysh_help.c

tinysh_ls.o: tinysh_ls.c tinysh_ls.h

tinysh_mv.o: tinysh_mv.c tinysh_mv.h

builtin.o: builtin.c builtin.h

main.o: main.c tinysh_cd.h tinysh_exit.h tinysh_help.h tinysh_ls.h

clean: 
		rm *.o tinysh
