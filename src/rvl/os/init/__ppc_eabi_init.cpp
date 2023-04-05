#include "PowerPC_EABI_Support/Runtime/__ppc_eabi_init.h"
#include "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h"

#include <rvl/base/PPCArch.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef void (*voidfunctionptr)(void); // pointer to function returning void
    __declspec(section ".ctors") extern voidfunctionptr _ctors[];
    __declspec(section ".dtors") extern voidfunctionptr _dtors[];

    static void __init_cpp(void);
    static void __fini_cpp(void);
    extern void abort(void);
    extern void exit(int status);

    extern void __OSCacheInit(void);
    extern void __OSFPRInit(void);
    extern void __OSPSInit(void);

#ifdef __cplusplus
}
#endif

// clang-format off
asm void __init_hardware(void) {
    nofralloc
    mfmsr r0
    ori r0,r0,0x2000
    mtmsr r0
    mflr r31
    bl __OSPSInit
    bl __OSFPRInit
    bl __OSCacheInit
    mtlr r31
    blr
}

asm void __flush_cache(void *ptr, unsigned int n) {
#pragma unused (ptr, n)
	nofralloc
	lis r5, 0xFFFF
	ori r5, r5, 0xFFF1
	and r5, r5, r3
	subf r3, r5, r3
	add r4, r4, r3
loop:
	dcbst 0, r5
	sync
	icbi 0, r5
	addic r5, r5, 8
	addic. r4, r4, -8
	bge loop
	isync 
	blr
}
    // clang-format on

    asm void __init_user(void) 
    {
        fralloc
        bl __init_cpp
        frfree
        blr
    }

    static void __init_cpp(void)
    {
        voidfunctionptr *constructor;
        /*
         *	call static initializers
         */
        for (constructor = _ctors; *constructor; constructor++)
        {
            (*constructor)();
        }
    }

    static void __fini_cpp(void)
    {
        voidfunctionptr *destructor;

        /*
         *  call destructors
         */
        for (destructor = _dtors; *destructor; destructor++)
        {
            (*destructor)();
        }
    }

    void exit(int status)
    {
#pragma unused(status)
        __fini_cpp();
        _ExitProcess();
    }

    void _ExitProcess(void) { PPCHalt(); }
