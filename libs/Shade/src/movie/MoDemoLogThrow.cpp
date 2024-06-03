#include <revolution/OS.h>
#include <Shade/movie/MoDemoLogThrow.h>

void MoError(void *instance, const char *msg)
{
    OSReport("[FATAL ERROR] Instance %08x : %s\n", instance, msg);
    OSDisableScheduler();
    while (TRUE)
    {
    } // hang
}

void MoDebug(void *instance, const char *msg)
{
    OSReport("[Debug] Instance %08x : %s\n", instance, msg);
}