/***************************************************
 Defs.h

 ****************************************************/

#ifndef _defs_
#define _defs_

typedef struct {
	int operation;
	int * param1;
	int param2;
	char ** param3;
	char * param4;
	void * ret;
} params;

/*************Defines para system calls*******************/
#define BLOCK_PROC 1
#define CREATE_PROCESS 2
#define GET_PID 5
#define WAKE_PID 6
#define WAIT_PID 7
#define YIELD 8
#define KILL 9
#define GET_PROCESS_NAME 10
#define PASS_MODE 11
#define CANNONICAL_MODE 12
#define LAST100 13
#define SLEEP 14
#define CLEAR_SCREEN 15
#define FORMAT_PROC 16
#define UP_SEM 17
#define DOWN_SEM 18
#define GET_SEM 19
#define MALLOC 20
#define	REALLOC 21
#define FREE 22

/*********Defines for system calls permissions***********/
#define P_READ 4
#define P_WRITE 8
#define P_CREATE_PROCESS 3
#define P_BLOCK 3
#define P_GET_PID 4
#define P_WAKE_PID 3
#define P_WAIT_PID 3
#define P_YIELD 2
#define P_KILL 3
#define P_GET_PROCESS_NAME 4
#define P_PASS_MODE 4
#define P_CANNONICAL_MODE 4
#define P_LAST100 2
#define P_SLEEP 3
#define P_CLEAR_SCREEN 2
#define P_FORMAT 3

#define byte unsigned char
#define word short int
#define dword int
#define NULL 0

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80           /* segmento presente en memoria*/
#define ACS_CSEG        0x18           /* segmento de codigo */
#define ACS_DSEG        0x10           /* segmento de datos */
#define ACS_READ        0x02           /* segmento de lectura */
#define ACS_WRITE       0x02           /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )

#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define FOURKB			0x1000;

#define TICKS_PER_SEC 18
#define WRITE 4
#define READ 3
#define STDOUT 1
#define STDIN 0
#define PROCESS_QTY 1000
/*
 * Defines for paging and malloc
 */
#define KERNEL_MEMORY 0x10000000
#define PAGE_TABLES (KERNEL_MEMORY >> 22)
#define PAGE_DIR_START 0x00200000
#define PAGE_SIZE 0x1000
#define PAGE_TABLE_START (PAGE_DIR_START + PAGE_SIZE)
#define ENTRIES 1024
#define PAGES_ON_MEM PAGE_TABLES * 1024
#define KERNEL_STACK_END 0x600000
#define KERNEL_STACK_START (KERNEL_STACK_END + 3*PAGE_SIZE)
#define HEAP_START KERNEL_STACK_START //6 megas
#define HEAP_LIMIT 0x9600000 //150 megas
#define RWUPRESENT 7
#define RWUNOTPRESENT 6
#define STACK_START 0x9800000 //mega 152 - must be aligned to 4 megas
#define STACK_LIMIT 0x10000000

#define EACH_BLOCK_SIZE ((HEAP_LIMIT-HEAP_START)/3)

#define FOURK_BLOCK_START HEAP_START
#define FOURK_BLOCK_END (HEAP_START + EACH_BLOCK_SIZE)

#define EIGHTK_BLOCK_START FOURK_BLOCK_END
#define EIGHTK_BLOCK_END (EIGHTK_BLOCK_START + EACH_BLOCK_SIZE)

#define SIXTEENK_BLOCK_START EIGHTK_BLOCK_END
#define SIXTEENK_BLOCK_END (EIGHTK_BLOCK_END + EACH_BLOCK_SIZE)


#define STACK_SIZE 32*1024
#define DELTA 0x1000
#define PROCESS_QTY 1000
#define EXEC_TIME 1
#define MAX_FILES 20
#define MAX_PRIORITY 8

/*
 * ***********************************************************
 * Defines for pic remap
 */
/* Initialization commands */
#define PIC_INIT_CODE  0x11
#define MASTER_MODE    0x04
#define SLAVE_MODE     0x02
#define ENV_8086       0x01

/* PIC data ports */
#define PIC1_DATA_PORT 0x21
#define PIC2_DATA_PORT 0xA1
/* Pic IDT positions */
#define PIC1_OFFSET  0x20
#define PIC2_OFFSET  0x28

#define IRQ_BEGIN    PIC1_OFFSET
#define IRQ_END      (PIC2_OFFSET + 8)

/* Pic ports */
#define PIC1_COMMAND_PORT  0x20
#define PIC2_COMMAND_PORT  0xA0

/* EOI code */
#define PIC_EOI_CODE 0x20

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
typedef struct {
	word limit, base_l;
	byte base_m, access, attribs, base_h;
} DESCR_SEG;

/* Descriptor de interrupcion */
typedef struct {
	word offset_l, selector;
	byte cero, access;
	word offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
	word limit;
	dword base;
} IDTR;

typedef struct {
	unsigned int entries[1024];
} pageDirectory;

typedef unsigned int uint;

#endif
