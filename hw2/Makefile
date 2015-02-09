CC=gcc
CFLAGS=-c

all: hw2

hw2: shell.o hw2.o
	$(CC) shell.o hw2.o -o hw2

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c

hw2.o:	hw2.c
	$(CC) $(CFLAGS) hw2.c

clean:
	/bin/rm -f hw2 *.o *.gz

run:
	./hw2

tarball:
	# put your tar command here
	# tar -cvzf <lastname>.tar.gz *
