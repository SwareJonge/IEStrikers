#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    __declspec(section ".init") extern void __init_hardware(void);
    extern void __init_user(void);
    __declspec(weak)  extern void _ExitProcess(void);
    __declspec(section ".init") extern void __flush_cache(void *address, unsigned int size);

    __declspec(section ".init") __declspec(weak) extern void __exception_info_constants(void **info, char **R2);

#ifdef __cplusplus
}
#endif