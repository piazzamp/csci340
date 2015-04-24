// take in 32 bits and return a char array of hex chars
// for easy printing
char* mtoh(int memory);

// take in 32 bits and return an array of characters of octal chars
char* mtoo(int memory);

// translate memory to ascii chars
char* mtoc(int memory);

// make little endian FAT16 bytes into
int lendtobend(int memory);

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

