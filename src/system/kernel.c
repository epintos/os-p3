#include "../../include/kernel.h"
#include "../../include/iolib.h"
#include "../../include/kasm.h"
#include "../../include/defs.h"
#include "../../include/terminal.h"
#include "../../include/kc.h"
#include "../../include/scheduler.h"
#include "../../include/diskata.h"
#include "../../include/init.h"
#include "../../include/paging.h"

DESCR_INT idt[0x81]; /* IDT with 129 entries*/
IDTR idtr; /* IDTR */
int kernel = 1;
unsigned long long int tickpos = 0;
static void PIC_remap();

/**********************************************
 kmain()
 Punto de entrada de cóo C.
 *************************************************/

int kmain() {

	_Cli();
	/* Borra la pantalla y cursor */

	k_clear_screen((unsigned char *) 0xB8000);
	cursorOFF();
	PIC_remap();

	/*DIVISION BY CERO HANDLER*/
	setup_IDT_entry(&idt[0x00], 0x08, (dword) &_int_0_hand, ACS_INT, 0);

	/*BOUNDS EXCEPTION*/
	setup_IDT_entry(&idt[0x05], 0x08, (dword) &_int_05_hand, ACS_INT, 0);

	/*INVALID OPCODE*/
	setup_IDT_entry(&idt[0x06], 0x08, (dword) &_int_06_hand, ACS_INT, 0);

	/*SEGMENT NOT PRESENT*/
	setup_IDT_entry(&idt[0x0B], 0x08, (dword) &_int_B_hand, ACS_INT, 0);

	/*STACK FAULT*/
	setup_IDT_entry(&idt[0x0C], 0x08, (dword) &_int_C_hand, ACS_INT, 0);

	/*GENERAL PROTECION FAULT*/
	setup_IDT_entry(&idt[0x0D], 0x08, (dword) &_int_D_hand, ACS_INT, 0);

	/*PAGE FAULT HANDLER*/
	setup_IDT_entry(&idt[0x0E], 0x08, (dword) &_int_E_hand, ACS_INT, 0);

	/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

	setup_IDT_entry(&idt[0x20], 0x08, (dword) &_int_08_hand, ACS_INT, 0);

	/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ1    */

	setup_IDT_entry(&idt[0x21], 0x08, (dword) &_int_09_hand, ACS_INT, 0);

	/*	CARGA LA INT 80H AL IDT */
	setup_IDT_entry(&idt[0x80], 0x08, (dword) &_int_80_hand, ACS_INT, 0);

	/* Loads IDTR */

	idtr.base = 0;
	idtr.base += (dword) &idt;
	idtr.limit = sizeof(idt) - 1;

	_lidt(&idtr);

	/* Enables timer tick and keyboard interruption */
	_mascaraPIC1(0xFC);
	_mascaraPIC2(0xFF);

	setUpPaging();
	createProcess(init, 0, 0, "Init");

	_Sti();

	while (1)
		;
	return 0;
}

static void PIC_remap() {

	/* Initialising PIC communication */
	_outb(PIC1_COMMAND_PORT, PIC_INIT_CODE);
	_outb(PIC2_COMMAND_PORT, PIC_INIT_CODE);

	/* Offsets */
	_outb(PIC1_DATA_PORT, PIC1_OFFSET);
	_outb(PIC2_DATA_PORT, PIC2_OFFSET);

	/* Master/Slave */
	_outb(PIC1_DATA_PORT, MASTER_MODE);
	_outb(PIC2_DATA_PORT, SLAVE_MODE);

	/* Enviroment */
	_outb(PIC1_DATA_PORT, ENV_8086);
	_outb(PIC2_DATA_PORT, ENV_8086);
}

