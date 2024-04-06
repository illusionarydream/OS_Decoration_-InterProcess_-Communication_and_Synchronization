CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11

all: file1 file2

file1: BurgerBuddies.c
	$(CC) $(CFLAGS) -o BurgerBuddies BurgerBuddies.c

file2: SantaClaus.c
	$(CC) $(CFLAGS) -o SantaClaus SantaClaus.c

clean:
	rm -f BurgerBuddies SantaClaus