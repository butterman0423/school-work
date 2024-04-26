# Name: Nathaniel Escaro
# Pledge: I pledge my honor that I have abided by the Stevens Honor System.

CC=gcc
CFLAGS=-g -Wall -I.

all: utils
	$(CC) $(CFLAGS) utils.o server.c -o server
	$(CC) $(CFLAGS) utils.o client.c -o client

utils: utils.c
	$(CC) $(CFLAGS) -c $^ -o utils.o

clean:
	rm -f *.o server client
