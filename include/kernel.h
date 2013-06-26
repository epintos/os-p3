#include "../include/defs.h"

/********************************** 
 *
 *  Kernel
 *
 ***********************************/

#ifndef _kernel_
#define _kernel_

#define OS_PID	0

int (*player)(void);

typedef int size_t;
typedef short int ssize_t;
typedef enum eINT_80 {
	WRITE1 = 0, READ1
} tINT_80;
typedef enum eUSER {
	U_KERNEL = 0, U_NORMAL
} tUSERS;


typedef struct {
	dword address;
} DESCR_PAGE;
void setup_DESCR_PAGE(DESCR_PAGE * item, dword address);

/* __write
 *
 * Recibe como parametros:
 * - File Descriptor
 * - Buffer del source
 * - Cantidad
 *
 **/
size_t __write(int fd, const void* buffer, size_t count);

/* __read
 *
 * Recibe como parametros:
 * - File Descriptor
 * - Buffer a donde escribir
 * - Cantidad
 *
 **/
size_t __read(int fd, void* buffer, size_t count);

#endif
