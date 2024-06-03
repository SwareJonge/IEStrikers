#include "types.h"
#include <revolution/OS.h>

inline static void InitDefaultHeap()
{
	void* arenaLo;
	void* arenaHi;

	OSReport("GCN_Mem_Alloc.c : InitDefaultHeap. No Heap Available\n");
	OSReport("Metrowerks CW runtime library initializing default heap\n");

	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();

	arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
	OSSetArenaLo(arenaLo);

	arenaLo = (void*)OSRoundUp32B(arenaLo);
	arenaHi = (void*)OSRoundDown32B(arenaHi);

	OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
	OSSetArenaLo(arenaLo = arenaHi);
}

__declspec(weak) extern void *__sys_alloc(size_t n)
{
	if (__OSCurrHeap == -1)	{
		InitDefaultHeap();
	}
	return OSAllocFromHeap(__OSCurrHeap, n);
}

__declspec(weak) extern void __sys_free(void* ptr)
{
	if (__OSCurrHeap == -1) {
		InitDefaultHeap();
	}
	OSFreeToHeap(__OSCurrHeap, ptr);
}
