#include <stdarg.h>
#include <printf.h>
#include <revolution/OS.h>
#include <Shade/shd.h>
#include <Shade/wii.h>
#include <Shade/wii_debug.h>
#include <Shade/std/shd_cstring.h>

char errtxt_buff[2048];

void sys_err_prt(char *msg, ...)
{
    va_list list;
    va_start(list, msg);
    vsprintf(errtxt_buff, msg, list);
    shdwk.flags |= (4 | 2);

    OSReport("\n###### sys error ######\n");
    OSReport(errtxt_buff);
    OSReport("\n###### sys error ######\n\n");
    va_end(list);

    if ((shd_init_flag & 1) == 0 && wiiIsExecThread() == true)
    {
        while (true)
            ;
    }
    
    char buf[2048];
    sprintf(buf, "\n\n  ###### sys error ######\n\n   %s\n\n  ###### sys error ######", errtxt_buff);
    hstrcpy(errtxt_buff, buf);

    if ((shd_init_flag & 1) != 0)
    {
#line 56
        OSError(errtxt_buff);
        while (true)
            ;
    }

    wiiErrDisp(errtxt_buff);
    while (true)
        ;
}

const char *get_sys_err_txt() {
    return errtxt_buff;
}

void cprintf(char *msg, ...) {
    if ((shd_init_flag & DISABLE_DEBUG) == 0 && shdwk.flags & 4)
    {
        va_list list;
        va_start(list, msg);
        char buf[4096];
        vsprintf(buf, msg, list);
        OSLockMutex(&wiiwk.consoleMutex);
        OSReport(buf);
        OSUnlockMutex(&wiiwk.consoleMutex);

        va_end(list);
    }
}

CW_FORCE_STRINGS(shd_wii_debug, "Spad over!\n", "unknown error!\n");