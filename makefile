CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11

all: file1 file2

file1: BurgerBuddies.c
	$(CC) $(CFLAGS) -o BBC BurgerBuddies.c

file2: SantaClaus.c
	$(CC) $(CFLAGS) -o SantaClaus SantaClaus.c

clean:
	rm -f BBC SantaClaus