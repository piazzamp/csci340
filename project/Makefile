CC=gcc
CFLAGS=-c -O2 -Wall -g

all: dump

dump: disk.o main.o
	$(CC) disk.o main.o -o dump

disk.o: disk.c
	$(CC) $(CFLAGS) disk.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	/bin/rm -f dump *.o *.gz

run:
	./dump floppy.img 35 x

tarball:
	# put your tar command here
	tar -cvzf piazza.tar.gz *

