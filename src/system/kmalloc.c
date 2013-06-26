/*
 * kmalloc.c
 *
 *  Created on: Nov 18, 2011
 *      Author: mdesanti90
 */

#include "../../include/defs.h"
#include "../../include/scheduler.h"
#include "../../include/kmalloc.h"
#include "../../include/paging.h"
#include "../../include/kasm.h"
#include "../../include/iolib.h"
#include "../../include/string.h"

extern int kernel;

#define STACK_PAGES_QTY ((STACK_LIMIT-STACK_START)/PAGE_SIZE)
#define MAX_PAGES 3

typedef struct {
	unsigned int stackStart;
	unsigned int stackEnd;
	//point to the next page to be set as present - Top value is MAX_PAGES -1
	unsigned int offset;
} stackData;

static int used[STACK_PAGES_QTY] = { 0 };
static int next = 0;
static stackData data[PROCESS_QTY];

static void mark(int from, int qty);
static void unMark(int from, int to);
static int findFreePage();
static void markPages(int perms, unsigned int from, unsigned int to, int pid);
void printPages();

/*
 * Please notice that stackStart points to the last page of the stack.
 * StackStart must always be greater than stackEnd.
 *
 * --------------------------------------------
 * |             |              |             |
 * |   Page 1    |   Page 2     |   Page 3    |
 * |             |              |             |
 * --------------------------------------------
 * In this case, stackStart could be page 3 and stackEnd could be either page 1 or page 2.
 */

//used to give the first page to a process
void * kmalloc(int pid) {
	int freePage = findFreePage();
	if (freePage == -1)
		return NULL;
	data[pid].stackStart = freePage + MAX_PAGES - 1;
	data[pid].stackEnd = freePage;
	data[pid].offset = 1;
	return (void*) ((data[pid].stackStart + 1) * PAGE_SIZE + STACK_START - 1);
}

void * krealloc(int pid) {
	int start = data[pid].stackStart;
	if (data[pid].offset == MAX_PAGES - 1) {
		unMark(data[pid].stackEnd, MAX_PAGES);
		kill(0, 0, pid);
	}
	data[pid].offset++;
//	upPages(pid);
//	printPages();

	return (void*) ((start + 1) * PAGE_SIZE + STACK_START - 1);
}

static void unMark(int from, int qty) {
	for (; from < STACK_PAGES_QTY && qty >= 0; qty--, from++) {
		used[from] = 0;
	}
}

static void mark(int from, int qty) {
	for (; from < STACK_PAGES_QTY && qty >= 0; qty--, from++) {
		used[from] = 1;
	}
}

static int findFreePage() {

	if (next >= STACK_PAGES_QTY)
		return -1;
	uint ret = next;
	mark(next, MAX_PAGES);
	next = next + MAX_PAGES;
	return ret;
}

void upPages(int pid) {
	int back = kernel;
	kernel = 1;
	markPages(RWUPRESENT, data[pid].stackStart - data[pid].offset,
			data[pid].stackStart, pid);
	kernel = back;
}

void downPages(int pid) {
	int back = kernel;
	kernel = 1;
	markPages(RWUNOTPRESENT, data[pid].stackStart - data[pid].offset,
			data[pid].stackStart, pid);
	kernel = back;
}

static void markPages(int perms, unsigned int from, unsigned int to, int qty) {
	int i = 0;
	for (i = from; i <= to; i++) {
		int addr = STACK_START + PAGE_SIZE * i;
		unsigned int offset = ((unsigned int) addr) >> 22;
		unsigned int *dir = (unsigned int *) PAGE_DIR_START + offset;
		unsigned int *align = (unsigned int *) alignEntry(*dir);
		unsigned int *entry = align + ((((unsigned int) addr) >> 12) & 0x3FF);
		//remove old perms
		unsigned int modif = alignEntry(*entry);
		modif |= perms;
		*entry = modif;
	}
}

void printPages() {
	int i = 0, back = kernel;
	kernel = 1;
	for (i = 0; i < STACK_PAGES_QTY; i++) {
		if (used[i]) {
			int addr = STACK_START + PAGE_SIZE * i;
			unsigned int offset = ((unsigned int) addr) >> 22;
			unsigned int *dir = (unsigned int *) PAGE_DIR_START + offset;
			unsigned int *align = (unsigned int *) alignEntry(*dir);
			unsigned int *entry = align
					+ ((((unsigned int) addr) >> 12) & 0x3FF);
			printf("Entry %d = %x\n", i, *entry);
		}
	}
	kernel = back;
}

uint checkStackPosition(unsigned int esp, int pid) {
	unsigned int lastPage = (data[pid].stackStart - data[pid].offset) * PAGE_SIZE + STACK_START;
	unsigned int firstPage = (data[pid].stackStart) * PAGE_SIZE + STACK_START;
//	int back = kernel;
//	kernel = 1;
	if (strcmp(getProcessName(pid), "ProcessG"))
		printf("First Page %x - LastPage %x and ESP %x\n", firstPage, lastPage,
				esp);
//	kernel = back;
	if ((esp - DELTA) <= lastPage) {
		return 1;
	}
	return 0;
}

void notifyEndOfProcess(int pid) {
	unMark(data[pid].stackEnd, data[pid].stackStart);
}
