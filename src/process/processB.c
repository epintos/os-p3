#include "../../include/iolib.h"
#include "../../include/string.h"
#include "../../include/process.h"
#include "../../include/systemCalls.h"
#include "../../include/filesystem2.h"
#include "../../include/diskata.h"
#include "../../include/malloc.h"
#include "../../include/defs.h"
#include "../../include/AtaDiskAdapter.h"
#include "../../include/users.h"

int procB(int argc, char ** argv) {

	printf("Este proceso intenta acceder al stack de otro proceso. Como las paginas de dicho proceso no estan presentes, se lanza una exception por pagina no presente y el proceso es matado\n");
	sleep(3);
	printf("ESP %x\n", _getESP());
	unsigned int * dir = (unsigned int *)0x9813000;
	dir[0] = 152;

	return 0;
}
