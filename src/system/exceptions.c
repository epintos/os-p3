#include "../../include/kc.h"
#include "../../include/exceptions.h"
#include "../../include/iolib.h"
#include "../../include/kasm.h"
#include "../../include/scheduler.h"

extern int kernel;

void setss() {
	/*	int a;
	 int valss;
	 char *video = (char*) 0xB8000;
	 int *stacksegment = (int*) 0x10000;
	 a=lenspace(buffer);
	 valss=atoi2(&(buffer[a+1]));
	 *stacksegment=valss*8;
	 ponerss();*/
}

void setds() {
	/*int a;
	 int valds;
	 char *video = (char*) 0xB8000;
	 int *datasegment = (int*) 0x10000;
	 a=lenspace(buffer);
	 valds=atoi2(&(buffer[a+1]));
	 *datasegment=valds*8;
	 ponerds();*/
}

void bounds() {
//	printf("Bounds Exceeded!\n");
}

void invop() {
//	printf("Invalid OPCODE\n");
}

void div0() {
//	printf("Division by 0\n");
}

void gpf() {
//	printf("General Protection Fault\n");
	*(unsigned char *)0xB8002 = 'W';
	kill(0, 0, getPID());

}

void ssf() {
//	printf("Stack Segment Fault\n");
}

void snp() {
	int back = kernel;
	kernel = 1;
	printf("Segment Not Present\n");
	kill(0, 0, getPID());
	kernel = back;
}
