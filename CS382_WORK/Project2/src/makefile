CC = gcc
CFLAGS = -I ./include
DEPS = pardef.h parser.h bcode.h

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

cbld: bcode.o parser.o main.o
	$(CC) -g -o ../cbld bcode.o parser.o main.o

clean:
	rm *.o ../cbld
