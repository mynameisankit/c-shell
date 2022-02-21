csh: shell.c token.c token.h
	gcc -o csh shell.c token.c -I .