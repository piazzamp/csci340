// Disk geometry type
typedef struct {
unsigned int bytesPerSector;
unsigned int sectorsPerTrack;
unsigned int heads;
unsigned int cylinders;
} geometry_t;
// Handle is a file descriptor returned by open()
typedef int handle_t;
// Our floppy disk abstraction
typedef struct {
handle_t floppyDisk;
geometry_t geometry;
} disk_t;
// Disk is a pointer to a disk_t
typedef disk_t *Disk;
