#pragma once

#include <string.h>
#include <revolution/mem/mem_allocator.h>

void MOiSetAllocator(MEMAllocator *allocator);
MEMAllocator *MOiGetAllocator();

void *MOiAlloc(unsigned int size);
void MOiFree(void *p);

#ifdef __cplusplus
extern "C" 
{
#endif

// does this really belong here?
void MoThrowError(void *instance, const char *fmt, ...);
void MoLog(void *instance, const char *fmt, ...);

#define MO_ASSERT(inst, COND) \
if ((COND) == false)                 \
    MoThrowError(inst, "%s line %d : %s failed!\n", __FILE__, __LINE__, #COND);

#ifdef __cplusplus
};
#endif
