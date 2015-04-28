/*
 *  Final Project option 2: floppy disk reading
 * composed and compiled 
 * by matt piazza and john anderson
 * on 28 APR 2015 for Dr Tony Lerclerc's CSCI340
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	disk = physical_disk(argv[1]);
		
	sector_dump(disk, sector, type);

	close(disk->floppyDisk);
	free(disk);
	return 0;
}
