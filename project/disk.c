#include <fcntl.h>
#include "disk.h"
#include "utils.h"

// initializes device and returns a 
// disk_t full of metadata about the floppy
// disk_t: handle_t (floppyDisk), geometry_t (geometry)
// geometry_t: bytesPerSecor, sectorsPerTrack, heads, cylinders (all uints)
disk_t physical disk(char* name){
	disk_t *disk = (disk_t*) malloc(sizeof(disk_t));
	geometry_t *geo = (geometry_t*) malloc(sizeof(geometry_t));
	handle_t fd = open(name, O_RDONLY);
	if (fd < 0){
		fprintf(stderr, "failed to open %s, are you sure it exists?\n", name);
		exit(1);
	}
}

// reads data from the given logical sector and enters it into the buffer
// allocate the buffer within this function, make sure it's big enough, etc
int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsigned char* buffer);

// calls sector_read, then prints it using one of the following types:
// 'x': hex
// 'c': char
// 'o': octal 
void sector dump(Disk theDisk, int logicalSectorNumber, char type);
