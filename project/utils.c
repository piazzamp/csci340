// take in 32 bits and return a char array of hex chars
// for easy printing
char* mtoh(unsigned int memory);

// take in 32 bits and return an array of characters of octal chars
char* mtoo(unsigned int memory);

// translate memory to ascii chars
char* mtoc(unsigned int memory);

// make little endian FAT16 bytes into
unsigned int lendtobend(unsigned int memory){
	// lsb is leftmost in `memory`s bytes
	unsigned int result = 0;
	// mask: 1111 0000 0000 0000 = 61440
	// and the leftmost byte is the least significant one
	// results in xxxx 0000 0000 0000
	// then shift right 12 bits to: 0000 0000 0000 xxxx
	result = (memory & 61440) >> 12;

	// mask 0000 1111 0000 0000
	// then shift right one byte
	result += (memory & 3840) >> 4;

	// mask 0000 0000 1111 0000
	// then shift left one byte
	result += (memory & 240) << 4;

	// mask 000 000 0000 1111 (hmm, 15?)
	// then shift all the way left 12 bits
	result += (memory & 15) << 12;

	return result;
}
