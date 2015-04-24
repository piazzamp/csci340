#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "disk.h"
#include "utils.h"

// initializes device and returns a 
// disk_t full of metadata about the floppy
// disk_t: handle_t (floppyDisk), geometry_t (geometry)
// geometry_t: bytesPerSector, sectorsPerTrack, heads, cylinders (all uints)
Disk physical_disk(char* name){
	disk_t *disk = (disk_t*) malloc(sizeof(disk_t));
	geometry_t *geo = (geometry_t*) malloc(sizeof(geometry_t));
	handle_t fd = open(name, O_RDONLY);
	char buf[100];
	unsigned int tmp = 0;
	int num_sectors;	

	if (fd < 0){
		fprintf(stderr, "failed to open %s, are you sure it exists?\n", name);
		exit(1);
	}
/*	if (lseek(fd, 3, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek\n");
		exit(1);
	}
	if (read(fd, buf, 7) < 0){
		fprintf(stderr, "failed to read file\n");
		exit(1);
	}
	buf[7] = '\0';
	printf("[3-10] floppy manufacturer bytes: %s\n", buf);
*/
	if (lseek(fd, 11, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek (11)\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0){
		fprintf(stderr, "failed to read number of bytes per sector\n");
		exit(1);
	}
	geo->bytesPerSector = tmp;
	printf("[11-12] bytes per sector: %d\n", geo->bytesPerSector);

	if (lseek(fd, 19, SEEK_SET) <  0){
		fprintf(stderr, "failed to seek (19)\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0) {
		fprintf(stderr, "failed to read from the floppy\n");
		exit(1);
	}	
	num_sectors = tmp;
	printf("[19-20] num of sectors: %d\n",num_sectors);

	if (lseek(fd, 26, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek to 26\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0){
		fprintf(stderr, "failed to read heads\n");
		exit(1);
	}	
	geo->heads = tmp;
	printf("[26-27] heads %d\n", geo->heads);

	if (lseek(fd, 24, SEEK_SET) < 0){
		fprintf(stderr, "failed to seek to 24\n");
		exit(1);
	}
	if (read(fd, &tmp, 2) < 0){
		fprintf(stderr, "failed to read sectors per track\n");
		exit(1);
	}
	geo->sectorsPerTrack = tmp;
	printf("[24-25] sectors per track: %d\n", geo->sectorsPerTrack);

	geo->cylinders = num_sectors / geo->sectorsPerTrack;
	printf("[     ] cylinders: %d\n", geo->cylinders);
	disk->floppyDisk = fd;
	disk->geometry = *geo;
	return disk;
}

// reads data from the given logical sector and enters it into the buffer
// allocate the buffer within this function, make sure it's big enough, etc
int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsigned char* buffer);

// calls sector_read, then prints it using one of the following types:
// 'x': hex
// 'c': char
// 'o': octal 
void sector_dump(Disk theDisk, int logicalSectorNumber, char type);
