CC=gcc
CFLAGS=-c -O2 -Wall -g

all: hw5

hw5: mcsim.o hw5.o
	$(CC) mcsim.o hw5.o -o hw5 -lpthread -lrt

mcsim.o: mcsim.c
	$(CC) $(CFLAGS) mcsim.c

hw5.o: hw5.c
	$(CC) $(CFLAGS) hw5.c

clean:
	/bin/rm -f hw5 *.o *.gz

run:
	./hw5 300 100

tarball:
	# put your tar command here
	# tar -cvzf <lastname>.tar.gz *

