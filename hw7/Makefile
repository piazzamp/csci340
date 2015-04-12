CC=gcc
CFLAGS=-c -O2 -Wall -g

all: hw6

hw6: mem.o main.o
	$(CC) mem.o main.o -o hw7

mem.o: mem.c
	$(CC) $(CFLAGS) mem.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	/bin/rm -f hw7 *.o *.gz

run:
	./hw7 1200 100 20 667

tarball:
	# put your tar command here
	tar -cvzf piazza.tar.gz *

