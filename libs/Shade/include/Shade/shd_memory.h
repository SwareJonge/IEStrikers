#pragma once

#include "types.h"

#include <revolution/mem/mem_allocator.h>

int shdInitMem();

void *shdMemGet(u32 size, int alignment, s8 flag); // TODO: investigate flag
void shdMemFree(void *mem);

MEMAllocator *shdGetAllocator(int);
MEMAllocator *shdGetAllocator2(int);
