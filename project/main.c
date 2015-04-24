#include <stdio.h>
#include <stdlib.h>

#include "disk.h"


// usage: main floppy.img sectornum {c,x,o}
int main(int argc, char **argv){
	Disk disk;
	int sector;
	char type;

	if (argc < 4){
		fprintf(stderr, "usage: %s <fat16 image file> <sector num> {c,x,o}\nwhere c, x, or o indicate character, hex, or octal print formatting\n\n", argv[0]);
		exit(1);
	}
	type = argv[3][0];
	sector = atoi(argv[2]);
	printf("got sector num: %d\n", sector);
	disk = physical_disk(argv[1]);
		
	sector_dump(disk, sector, type);

	return 0;
}
