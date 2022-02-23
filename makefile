CC=gcc
CFLAGS=-Wall -I .

csh: shell.o history.o token.o built-in.o
	$(CC) $(CFLAGS) -o csh shell.o history.o token.o built-in.o

shell.o: shell.c history.h constants.h built-in.h token.h
	${CC} ${CFLAGS} -c shell.c

history.o: history.c history.h constants.h
	${CC} ${CFLAGS} -c history.c

token.o: token.c token.h constants.h
	${CC} ${CFLAGS} -c token.c

built-in.o: built-in.c token.h built-in.h history.h
	${CC} ${CFLAGS} -c built-in.c

clean:
	rm -f *.o 