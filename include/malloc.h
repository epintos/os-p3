#ifndef MALLOC_H_
#define MALLOC_H_
/* Allocs memory */
void* Malloc(int size);

/* Allocs more memory */
void * Realloc(void * ptr, int size, int oldSize);

void Free(void* ptr);

#endif
