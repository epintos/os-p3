#include "../../include/process.h"
#include "../../include/iolib.h"
#include "../../include/systemCalls.h"

int procF(int argc, char ** argv) {
	printf(
			"Este test intenta acceder al inicio del stack. Deberia ser matado por no estar presentes las paginas\n");
	sleepP(3);

	unsigned int * dir = (unsigned int *)STACK_START;
	dir[0] = 1;
	return 0;
}
