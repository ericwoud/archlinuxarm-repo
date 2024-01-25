/*
 * pokefile.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
  
int main(int argc, char **argv) {
	FILE *file = NULL;
	int memfd, filefd;
	int map_size = getpagesize();
	void *map_base; 
	off_t target;
	size_t bytesRead, totalbytesRead = 0;
	
	if(argc < 3) {
		fprintf(stderr, "\nUsage:\t%s { address } { filename }\n"
			"\taddress : starting memory address to write\n"
			"\tfile	: filename of file to write\n",
			argv[0]);
		exit(1);
	}
	target = strtoul(argv[1], 0, 0);
	target &= ~(map_size-1);

	filefd = open(argv[2], O_RDONLY);
	if (filefd == -1) exit(1);

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if (memfd == -1) exit(1);

	do {
		map_base = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, target + totalbytesRead);
		if (map_base == (void *) -1) break; 
		bytesRead = read(filefd, map_base, map_size);
		totalbytesRead += bytesRead;
		msync((void *)(target + totalbytesRead), map_size, MS_SYNC);
		if (munmap(map_base, map_size) == -1) break;
	}
	while (bytesRead == map_size);

	printf("Written %d bytes from %s to 0x%llx.\n", totalbytesRead, argv[2], target);
	
	close(memfd);
	return 0;
}

