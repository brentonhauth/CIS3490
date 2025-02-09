CC = gcc
FLAGS = -Wall -pedantic -std=c99

all: P11 P12 P2

clean:
	rm -f P11 P12 P2

P11: P1.h P11.c
	$(CC) $(FLAGS) P11.c -o P11

P12: P1.h P12.c
	$(CC) $(FLAGS) P12.c -o P12

P2: P2.c
	$(CC) $(FLAGS) P2.c -o P2

