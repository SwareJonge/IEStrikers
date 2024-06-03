#include "MoSdkMem.h"

static MEMAllocator *MOiAllocator;
static int MOiAllocated;
static int MOiNbAllocated;
static int MOiMaxAllocated;

void MOiSetAllocator(MEMAllocator *allocator) { MOiAllocator = allocator; }

MEMAllocator *MOiGetAllocator() { return MOiAllocator; }

void *MOiAlloc(unsigned int size)
{
    if (MOiAllocator == NULL)
    {
        MoThrowError(NULL, "Heap not initialized\n");
        return NULL;
    }

    size = ROUND_UP(size, 32) + 32;
    void *ptr = MEMAllocFromAllocator(MOiAllocator, size);
    
    if (ptr == NULL)
        return NULL;

    u32 *u32Ptr = (u32 *)ptr;
    *u32Ptr = size;
    MOiAllocated += size;

    if (MOiAllocated > MOiMaxAllocated)
        MOiMaxAllocated = MOiAllocated;
    
    MOiNbAllocated++;

    return u32Ptr + 8;
}

void MOiFree(void *p)
{
    if (MOiAllocator == NULL)
    {
        MoThrowError(NULL, "Heap not initialized\n");
        return;
    }

    if (p) 
    {
        u32 *u32Ptr = (u32 *)p; 
        u32Ptr -= 8;
        MOiAllocated -= *u32Ptr;
        MOiNbAllocated--;
        MEMFreeToAllocator(MOiAllocator, u32Ptr);
    }
}