// take in 32 bits and return a char array of hex chars
// for easy printing
char* mtoh(int memory);

// take in 32 bits and return an array of characters of octal chars
char* mtoo(int memory);

// translate memory to ascii chars
char* mtoc(int memory);

// make little endian FAT16 bytes into
int lendtobend(int memory);
