/* 7zAlloc.h -- Allocation functions
2008-03-28
Igor Pavlov
Public domain */

#ifndef _7zAllocH
#define _7zAllocH

#include <stddef.h>

void *SzAlloc(void *p, size_t size);
void SzFree(void *p, void *address);

void *SzAllocTemp(void *p, size_t size);
void SzFreeTemp(void *p, void *address);

#endif
