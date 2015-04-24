#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "disk.h"

#define DEBUG 0

// initializes device and returns a 
// disk_t full of metadata about the floppy
// disk_t: handle_t (floppyDisk), geometry_t (geometry)
// geometry_t: bytesPerSector, sectorsPerTrack, heads, cylinders (all uints)
Disk physical_disk(char* name){
	disk_t *disk = (disk_t*) malloc(sizeof(disk_t));
	// geometry_t *geo = (geometry_t*) malloc(sizeof(geometry_t));
	handle_t fd = open(name, O_RDONLY);	
	unsigned int tmp = 0;
	int num_sectors;	

	if (fd < 0){
		fprintf(stderr, "failed to open %s, are you sure it exists?\n", name);
		exit(1);
	}

	if (lseek(fd, 11, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek (11)\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0){
		fprintf(stderr, "failed to read number of bytes per sector\n");
		exit(1);
	}
	//geo->bytesPerSector = tmp;
	disk->geometry.bytesPerSector = tmp;
	if (DEBUG) { printf("[11-12] bytes per sector: %d\n", disk->geometry.bytesPerSector); }

	if (lseek(fd, 19, SEEK_SET) <  0){
		fprintf(stderr, "failed to seek (19)\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0) {
		fprintf(stderr, "failed to read from the floppy\n");
		exit(1);
	}	
	num_sectors = tmp;
	if (DEBUG) { printf("[19-20] num of sectors: %d\n",num_sectors); }

	if (lseek(fd, 26, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek to 26\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0){
		fprintf(stderr, "failed to read heads\n");
		exit(1);
	}	
	disk->geometry.heads = tmp;
	if (DEBUG) { printf("[26-27] heads %d\n", disk->geometry.heads); }

	if (lseek(fd, 24, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek to 24\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0){
		fprintf(stderr, "failed to read sectors per track\n");
		exit(1);
	}
	disk->geometry.sectorsPerTrack = tmp;
	if (DEBUG) { printf("[24-25] sectors per track: %d\n", disk->geometry.sectorsPerTrack); }

	disk->geometry.cylinders = num_sectors / disk->geometry.sectorsPerTrack;
	if (DEBUG) { printf("[     ] cylinders: %d\n", disk->geometry.cylinders); }

	disk->floppyDisk = fd;	
	return disk;
	// when close(fd)??????
}

// reads data from the given logical sector and enters it into the buffer
// allocate the buffer within this function, make sure it's big enough, etc
int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsigned char* buffer){
	unsigned int offset = logicalSectorNumber * theDisk->geometry.bytesPerSector;
	if (DEBUG) { printf("seeking to %d\nfloppydisk: %d\n", offset, theDisk->floppyDisk); }
	if (lseek(theDisk->floppyDisk,offset, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek to %d\n", offset);
		return 0;
	}
	if (read(theDisk->floppyDisk, buffer, theDisk->geometry.bytesPerSector) < 0){
		fprintf(stderr, "failed to read at %d\n", offset);
		return 0;
	}
	return 1;
}

// calls sector_read, then prints it using one of the following types:
// 'x': hex
// 'c': char
// 'o': octal 
void sector_dump(Disk theDisk, int logicalSectorNumber, char type){
	unsigned char *buf = (unsigned char*) malloc(theDisk->geometry.bytesPerSector);
	char *str;
	int i, j, offset;

	if(sector_read(theDisk, logicalSectorNumber, buf) == 0){
		fprintf(stderr, "failed to read sector %d\n", logicalSectorNumber);
		exit(1);
	}
	offset = logicalSectorNumber * theDisk->geometry.bytesPerSector;

	switch(type){
		case 'x':
			str = "%01x\t";
			break;
		case 'c':
			str = "%c";
			break;
		case 'o':
			str = "%o\t";
			break;
		default:
			fprintf(stderr, "please use a type in the set {c, o, x}\n");
			exit(1);
	}
	for (i = 0; i < 32; i++){
		printf("0x%04x\t", offset + (i * 16));
		for (j = 0; j < 16; j++){
			printf(str, buf[(i * 16) + j]);
		}
		printf("\n");
	}
	free(buf);
}
