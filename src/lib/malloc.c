#include "../../include/defs.h"
#include "../../include/iolib.h"
#include "../../include/malloc.h"

#define HEAP_PAGES ((HEAP_LIMIT-HEAP_START)/PAGE_SIZE)
#define NOT_ENOUGH_PAGES -1

#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 16
#define BIT5 32
#define BIT6 64
#define BIT7 128

#define FOURK_BLOCKS_QTY ((FOURK_BLOCK_END - FOURK_BLOCK_START)/0x1000)
#define EIGHTK_BLOCKS_QTY ((EIGHTK_BLOCK_END - EIGHTK_BLOCK_START)/0x2000)
#define SIXTEENK_BLOCKS_QTY ((SIXTEENK_BLOCK_END - SIXTEENK_BLOCK_START)/0x4000)

static char fourk[FOURK_BLOCKS_QTY] = { 0 };
static char eightk[EIGHTK_BLOCKS_QTY] = { 0 };
static char sixteenk[SIXTEENK_BLOCKS_QTY] = { 0 };

static void * Malloc2(int size);
static void * getFourKBlock();
static void freeFourK(uint dir);
static void * getEightKBlock();
static void freeEightK(uint dir);
static void * getSixteenKBlock();
static void freeSixteenK(uint dir);
static void mark(int from, int qty);
static void * bigSize(uint size);

//static int offset = 0;
//extern int kernel;

void * Malloc(int size) {
	return Malloc2(size);

//	int old = kernel;
//	kernel = 1;
//	void* ret = 0;
//	ret = (void*) (offset + size + HEAP_START);
//	kernel = old;
//	offset += size;
//	return ret;
}

void * Calloc(int size, int cols) {
	char* ret = (char*) Malloc(size * cols);
	int i = 0;
	for (; i < size; i++)
		ret[i] = 0;
	return ret;
}

void * Realloc(void * ptr, int size, int oldSize) {

	int i = 0;
	char* ret = (char*) Malloc(size);

	for (i = 0; i < oldSize; i++) {
		ret[i] = ((char *) ptr)[i];
	}
	return ret;
}

void Free(void* ptr) {
	uint dir = (uint) ptr;

	if (dir < FOURK_BLOCK_END) {
		freeFourK(dir);
	} else if (dir < EIGHTK_BLOCK_END) {
		freeEightK(dir);
	} else {
		freeSixteenK(dir);
	}
}

static void * Malloc2(int size) {
	void * ret = NULL;
	if (size <= 0x1000) {
		ret = getFourKBlock();
	} else if (size <= 0x2000) {
		ret = getEightKBlock();
	} else if (size <= 0x4000) {
		ret = getSixteenKBlock();
	} else {
		ret = bigSize(size);
	}
	return ret;
}

static void * getFourKBlock() {
	int i = 0;

	for (i = 0; i < FOURK_BLOCKS_QTY; i++) {
		int index = i / 8;
		int bit = 7 - (i % 8);
		if (((fourk[index] >> bit) & 1) == 0) {
			fourk[index] |= 1 << bit;
			return (void *) (FOURK_BLOCK_START + i * 0x1000);
		}
	}
	return NULL;
}

static void * getEightKBlock() {
	int i = 0;

	for (i = 0; i < EIGHTK_BLOCKS_QTY; i++) {
		int index = i / 8;
		int bit = 7 - (i % 8);
		if (((eightk[index] >> bit) & 1) == 0) {
			eightk[index] |= 1 << bit;
			return (void *) (EIGHTK_BLOCK_START + i * 0x2000);
		}
	}
	return NULL;
}

static void * getSixteenKBlock() {
	int i = 0;

	for (i = 0; i < SIXTEENK_BLOCKS_QTY; i++) {
		int index = i / 8;
		int bit = 7 - (i % 8);
		if (((sixteenk[index] >> bit) & 1) == 0) {
			sixteenk[index] |= 1 << bit;
			return (void *) (SIXTEENK_BLOCK_START + i * 0x4000);
		}
	}
	return NULL;
}

static void * bigSize(uint size) {
	int i = 0;
	int qty = size / 0x4000 + 1;

	for (i = 0; i < SIXTEENK_BLOCKS_QTY; i++) {
		int index = i / 8;
		int bit = 7 - (i % 8);
		if (((sixteenk[index] >> bit) & 1) == 0) {
			int aux = qty;
			int start = i;
			for (; i < SIXTEENK_BLOCKS_QTY && aux > 0; i++, aux--) {
				int newIdx = i / 8;
				int bit2 = 7 - (i % 8);
				if (((sixteenk[newIdx] >> bit2) & 1) == 1) {
					aux = -1;
				}
			}
			if (aux == 0) {
				mark(start, qty);
				return (void *) (SIXTEENK_BLOCK_START + start * 0x4000);
			}
		}
	}

	return NULL;
}

static void mark(int from, int qty) {

	for (; qty > 0; qty--) {
		int index = from / 8;
		int bit = 7 - (from % 8);
		sixteenk[index] |= 1 << bit;
		from++;
	}
}

static void freeFourK(uint dir) {
	uint offset = (dir - FOURK_BLOCK_START) / 0x1000;
	int index = offset / 8;
	int bit = 7 - (offset % 8);

	fourk[index] &= 254 << bit;

	return;
}

static void freeEightK(uint dir) {
	uint offset = (dir - EIGHTK_BLOCK_START) / 0x2000;
	int index = offset / 8;
	int bit = 7 - (offset % 8);

	eightk[index] &= 254 << bit;

	return;
}

static void freeSixteenK(uint dir) {
	uint offset = (dir - SIXTEENK_BLOCK_START) / 0x4000;
	int index = offset / 8;
	int bit = 7 - (offset % 8);

	sixteenk[index] &= 254 << bit;

	return;
}


