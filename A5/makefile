CC = gcc
FLAGS = -Wall -pedantic -std=c99


all: P11 P12 P21 P22

clean:
	rm -f P11 P12 P21 P22


P11: P1.h P11.c
	$(CC) $(FLAGS) P11.c -o P11

P12: P1.h P12.c
	$(CC) $(FLAGS) P12.c -o P12


P21: P2.h P21.c
	$(CC) $(FLAGS) P21.c -o P21

P22: P2.h P22.c
	$(CC) $(FLAGS) P22.c -o P22
