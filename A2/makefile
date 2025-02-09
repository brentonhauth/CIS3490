FLAGS = -Wall -pedantic -std=c99 -g

all: P11 P12 P21 P22

clean:
	rm -f *.out *.exe P11 P12 P21 P22

# Problem 1
P11: P11.c P1.h data_A2_Q1.txt
	gcc $(FLAGS) P11.c -o P11

P12: P12.c P1.h data_A2_Q1.txt
	gcc $(FLAGS) P12.c -o P12


# Problem 2
P21: P21.c P2.h data_A2_Q2.txt
	gcc $(FLAGS) P21.c -o P21 -lm

P22: P22.c P2.h data_A2_Q2.txt
	gcc $(FLAGS) P22.c -o P22 -lm
