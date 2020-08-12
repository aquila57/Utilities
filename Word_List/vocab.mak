OBJ=vocab.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=

vocab:				$(OBJ)
		$(CC) -Wall -O2 $(OBJ) -o vocab $(LDFLAGS)

vocab.o:			vocab.c
		$(CC) $(CFLAGS) vocab.c

clean:
		rm -f $(OBJ) vocab
