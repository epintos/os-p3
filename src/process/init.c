#include "../../include/filesystem.h"
#include "../../include/terminal.h"
#include "../../include/kasm.h"
#include "../../include/users.h"
#include "../../include/scheduler.h"
#include "../../include/kernel.h"
#include "../../include/kc.h"

extern int kernel;

int init(int argc, char ** argv) {

	initTerms();
	initializeFilesystem();
	readDefaultUsers();
	k_clear_screen((unsigned char *) 0xB8000);
	kernel = 0;
	return 0;
}
