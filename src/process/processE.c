#include "../../include/iolib.h"
#include "../../include/systemCalls.h"
#include "../../include/process.h"
#include "../../include/filesystem2.h"

#define SIZE 300*1024


int procE(int argc, char ** argv) {
	int i = 0;
	int aux, fd;
	char * write = malloc(SIZE);
	char * read = malloc(SIZE);
	if(write == NULL || read == NULL) {
		printf("Mal el malloc\n");
		return 0;
	}

	for (; i < SIZE; i++) {
		write[i] = 'A';
	}
	write[SIZE - 1] = 'B';

	if ((fd = open("dinux/prueba.txt", O_CREATE)) == -1) {
			printf("Error en open\n");
		while (1) {
		}
	}

	printf("Starting to write\n");
	aux = Write(fd, write, SIZE);
	printf("Finished writing\n");
	close(fd);

	if ((fd = open("dinux/prueba.txt", O_RDWR)) == -1) {
			printf("Error en open2\n");
		while (1) {
		}
	}

	printf("Reading...\n");
	aux = Read(fd, read, SIZE);
	printf("Finished reading...\n");
	close(fd);

	for(i = 0; i < SIZE; i++) {
		if(write[i] != read[i]) {
			printf("Son distintos\n");
			return 0;
		}
	}
	printf("Son iguales! =)\n");
	return 0;
}
