// initializes device and returns a 
disk_t physical disk(char* name);

// reads data from the given logical sector and enters it into the buffer
// allocate the buffer within this function, make sure it's big enough, etc
int sector_read(Disk theDisk, unsigned int logicalSectorNumber, unsigned char* buffer);

// calls sector_read, then prints it using one of the following types:
// 'x': hex
// 'c': char
// 'o': octal 
void sector dump(Disk theDisk, int logicalSectorNumber, char type);

// Disk geometry type
typedef struct {
   unsigned int bytesPerSector;
   unsigned int sectorsPerTrack;
   unsigned int heads;
   unsigned int cylinders;
} geometry_t;

// Our floppy disk abstraction
typedef struct {
   handle_t floppyDisk;
   geometry_t geometry;
} disk_t;

// Handle is a file descriptor returned by open()
typedef int handle_t;

// Disk is a pointer to a disk_t
typedef disk_t *disk_p;
