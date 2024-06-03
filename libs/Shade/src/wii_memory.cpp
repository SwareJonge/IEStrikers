#include <Shade/shd_memory.h>

// NOTE: doesn't match because Cpp_exception doesn't work yet!

void *__sys_alloc(size_t n) { return shdMemGet(n, 4, 1); }
void __sys_free(void *p) { shdMemFree(p); }

void *operator new(size_t n) throw()
{
    return shdMemGet(n, 4, 0);
}

void *operator new[](size_t n) throw()
{
    return shdMemGet(n, 4, 0);
}

void operator delete(void *p) throw()
{
    shdMemFree(p);
}

void operator delete[](void *p) throw()
{
    shdMemFree(p);
}