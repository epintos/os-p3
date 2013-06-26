#define SIZE 1024 * 1024 * 5
#include "../../include/filesystem2.h"
#include "../../include/iolib.h"
#include "../../include/process.h"
#include "../../include/kasm.h"

//static char wr[SIZE];
//static char rd[SIZE];

uint esp;

void recursiva(int p) {
	sleep(1);
	recursiva(++p);
}

int procG(int argc, char ** argv) {
	recursiva(1);
//	printf("ProcessG - PID %d\n", getPID());
//	printf("2\n");
//	unsigned int *dir = (unsigned int *)0x98A4000;
//	dir[0] = 152;
//	printf("3\n");
//	dir = dir - 0x1000;;
//	dir[0] = 152;
//	printf("4\n");
//	dir = dir - 0x1000;;
//	dir[0] = 152;
//	printf("5\n");
//	dir = dir - 0x1000;
//	dir[0] = 152;
	return 1;
}

