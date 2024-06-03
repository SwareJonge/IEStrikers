#pragma once

#include "types.h"

#ifndef __INTELLISENSE__
#include <new>
#endif
#include <revolution/mem/mem_allocator.h>

/*static*/ int shdInitMem();
void shdFreeMemAllocList();
void *shdPushMemAllocList(void *block);
void shdPopMemAllocList(void *block);

void *shdMemGet(size_t size, int alignment, s8 flag); // TODO: investigate flag
void shdMemFree(void *mem);

/*static*/ void *shdAllocatorAllocForExpHeap_(MEMAllocator *allocator, u32 size);
/*static*/ void shdAllocatorFreeForExpHeap_(MEMAllocator *allocator, void *block);
/*static*/ void *shdAllocator2AllocForExpHeap_(MEMAllocator *allocator, u32 size);
/*static*/ void shdAllocator2FreeForExpHeap_(MEMAllocator *allocator, void *block);

MEMAllocator *shdGetAllocator(int);
MEMAllocator *shdGetAllocator2(int);
