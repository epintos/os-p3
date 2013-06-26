#include "../../include/diskata.h"
#include "../../include/AtaDiskAdapter.h"
#include "../../include/iolib.h"
#include "../../include/malloc.h"
#include "../../include/kasm.h"

#define CACHED_BLOCKS 800
#define BLOCK_AMOUNT 20

typedef struct {
	int sector;
	char data[512];
	int access;
} cacheData;

static cacheData cache[CACHED_BLOCKS];
static unsigned long long int flash = 0;
static int enabled = 0;
extern int kernel;

static int isInCache(int sector);
static int findFreeEntry();
static void writeData(char * from, char * to);
static void writeInCache(int sector, char * data);

void enableCache() {
	enabled = 1;
}

void disableCache() {
	enabled = 0;
}

void initCache() {
	int i = 0;
//	cache = (cacheData *) Malloc(sizeof(cacheData) * CACHED_BLOCKS);
	for (i = 0; i < CACHED_BLOCKS; i++) {
		cache[i].access = 0;
		cache[i].sector = -1;
	}
}

static int isInCache(int sector) {
	int i = 0;

	for (i = 0; i < CACHED_BLOCKS; i++) {
		if (cache[i].sector == sector)
			return i;
	}

	return -1;
}

static int findFreeEntry() {
	int i = 0;

	for (i = 0; i < CACHED_BLOCKS; i++) {
		if (cache[i].sector == -1)
			return i;
	}
	return -1;
}

static void writeData(char * from, char * to) {
	int j = 0;
	for (j = 0; j < 512; j++) {
		to[j] = from[j];
	}
}

static int freeLeastUsed() {
	int i = 0;
	int min = 0;
	int back = kernel;
	kernel = 1;

	for (; i < CACHED_BLOCKS; i++) {
		if (cache[i].access < cache[min].access) {
			min = i;
		}
	}
	_disk_write2(0, cache[min].data, 1, cache[min].sector);
	kernel = back;
	return min;
}

static void writeInCache(int sector, char * data) {
	if (!enabled)
		return;
	int i = isInCache(sector);
	int back = kernel;
	kernel = 1;
	if (i != -1) {
		//is in cache
		writeData(data, cache[i].data);
	} else {
		//not in cache
		int j = findFreeEntry();
		if (j == -1) {
			//there are no free entries
			j = freeLeastUsed();
		}
		writeData(data, cache[j].data);
		cache[j].sector = sector;
		cache[j].access = 0;
	}
	flash++;
	kernel = back;
}

static void cacheRead(int sector, char * into) {
	int back = kernel;
	kernel = 1;
//	printf("Quiero leer sector %d\n", sector);
	int i = isInCache(sector);
	if ((i == -1) || (!enabled)) {
		//not in cache
		_disk_read2(0, into, 1, sector);
		writeInCache(sector, into);
		return;
	}
//	printf("Leo de Cache\n");
	cache[i].access++;
	int m = 0;
	for (m = 0; m < 512; m++) {
		into[m] = cache[i].data[m];
	}
	kernel = back;
}

/*
 * This is an implementation of the interface proposed by
 * AtamDiskAdapter.h. Its aim is to give the user
 * the possiblity of writing into disk with an offset and a size
 * smaller thank 512 bytes.
 */

void diskWrite(char * msg, int bytes, int sector, int offset) {
	int back = kernel;
	kernel = 1;
//	printf("Escribo en disco sector %d\n", sector);
	kernel = back;
	if (bytes == 0)
		return;
	if (offset == 0 && bytes == 512) {
		writeInCache(sector, msg);
		_disk_write2(0, msg, 1, (unsigned int) sector);
		return;
	}

	//qty of sectors to use
	int sectQty = (bytes + offset) / 512 + 1;
	int j = 0;
	int i = offset;
	char read[512];

	cacheRead(sector, read);

	for (; i < SECTOR_SIZE && j < bytes; i++, j++) {
		read[i] = msg[j];
	}
	writeInCache(sector, read);
	_disk_write2(0, read, 1, sector);
	if (j == bytes)
		return;
	sector++;
	sectQty--;
	_disk_write2(0, &msg[j], sectQty - 1, sector);
	int w = 0;
	for (w = 0; w < sectQty - 1; w++) {
		writeInCache(sector + w, &msg[j + 512 * w]);
	}

	j += SECTOR_SIZE * (sectQty - 1);
	sector += sectQty;
	if (j == bytes)
		return;
	cacheRead(sector, read);
	i = 0;
	for (; i < SECTOR_SIZE && j < bytes; i++, j++) {
		read[i] = msg[j];
	}
	_disk_write2(0, read, 1, sector);
	writeInCache(sector, read);
}

void diskRead(char * ans, int bytes, int sector, int offset) {
	int back = kernel;
	kernel = 1;
//	printf("Leo de disco sector %d\n", sector);
	kernel = back;

	if (bytes == 0)
		return;

	//qty of sectors to use
	int sectQty = (bytes + offset) / 512 + 1;
	int j = 0;
	int i = offset;
	char read[512];

//	_disk_read2(0, read, 1, sector);
	cacheRead(sector, read);

	for (; i < SECTOR_SIZE && j < bytes; i++, j++) {
		ans[j] = read[i];
	}
	if (j == bytes)
		return;
	sector++;
	sectQty--;
	int w = 0;
	for (w = 0; w < sectQty - 1; w++) {
		cacheRead(sector + w, &ans[j + w * 512]);
	}
//	_disk_read2(0, &ans[j], sectQty - 1, sector);

	j += SECTOR_SIZE * (sectQty - 1);
	sector += sectQty;
	if (j == bytes)
		return;

	cacheRead(sector, read);
//	_disk_read2(0, read, 1, sector);
	i = 0;
	for (; i < SECTOR_SIZE && j < bytes; i++, j++) {
		ans[j] = read[i];
	}
}
