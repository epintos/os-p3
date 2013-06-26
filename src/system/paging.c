#include "../../include/paging.h"
#include "../../include/kasm.h"
#include "../../include/defs.h"


static void setUpPages(void);
static void createPageTable(void* addr, int perms);
static unsigned int getEntry(unsigned int address, unsigned int perms);


/*
 * Paging will work the following way:
 * 		CR3 register will point to the page directory.
 * 		Page directory contains 1024 references to page tables.
 * 		Page tables contain 1024 references to pages.
 * **********************************************************************
 * The 32 bits of the incoming address will be used in the following way:
 * 		Lowest 12 bits (from 0 to 11) will be the offset in the page.
 * 		Mid 10 bits (12 to 21) will be the offset in the page table.
 * 		High 10 bits (22 to 31) will be the offset in the page directory.
 * **********************************************************************
 * The 32 bits (int) entry in the page directory or page table will be
 * organized in the following way:
 * 		Bits 11 to 31 will be the Page Frame Address
 * 		Bit 0 -> Present
 * 		Bit 1 -> Read/Write
 * 		Bit 2 -> User/Supervisor
 * 		Bit 6 -> Dirty
*/

void setUpPaging(void) {

//	int i = 0;
	setUpPages();

//    register_interrupt_handler ( 14, page_fault_handler_wrapper );
//	for (i = 0; i < PAGES_ON_MEM; i++) {
//		pages[i] = -1;
//	}
	_setPageDir(PAGE_DIR_START);
	_activatePaging();
}

static void setUpPages(void) {
	int i = 0;
	int j = 0;
	for (i = 0; i < PAGE_TABLES; i++) {
		void *addr = (void *) (PAGE_SIZE * ENTRIES * i);
		createPageTable(addr, RWUPRESENT);
		int perms = RWUPRESENT;
		if((unsigned int)addr >= STACK_START)
			perms = RWUNOTPRESENT;

		for (j = 0; j < ENTRIES; j++) {
			createPage((void*) ((unsigned int) addr + j * PAGE_SIZE), perms);
		}
	}
}

static void createPageTable(void* addr, int perms) {

	unsigned int offset = ((unsigned int) addr) >> 22;
	unsigned int *dir = (unsigned int *) PAGE_DIR_START + offset;
	*dir = getEntry(PAGE_TABLE_START + offset * PAGE_SIZE, perms);
}

void createPage(void* addr, int perms) {

	unsigned int offset = ((unsigned int) addr) >> 22;
	unsigned int *dir = (unsigned int *) PAGE_DIR_START + offset;
	unsigned int *align = (unsigned int *) alignEntry(*dir);
	unsigned int *entry = align + ((((unsigned int) addr) >> 12) & 0x3FF);
	*entry = getEntry((unsigned int) addr, perms);
}

static unsigned int getEntry(unsigned int address, unsigned int perms) {

	unsigned int entry = alignEntry(address);
	entry |= perms;
	return entry;
}

unsigned int alignEntry(unsigned int entry) {

	return entry & 0xFFFFF000;
}
