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
