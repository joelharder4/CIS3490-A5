CC = gcc
CFLAGS = -Wall -std=c11 -pedantic

all: P11 P12 P21 P22

clean:
	rm P11 P12 P21

P11: P11.c
	$(CC) $(CFLAGS) -lm P11.c -o P11

P12: P12.c
	$(CC) $(CFLAGS) -lm P12.c -o P12

P21: P21.c
	$(CC) $(CFLAGS) -lm P21.c -o P21

P22: P22.c
	$(CC) $(CFLAGS) -lm P22.c -o P22