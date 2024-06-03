
#include <revolution/os/OSInterrupt.h>
#include <stdarg.h>
#include "nw4r/db/db_console.h"

// NOTE: this file is incomplete, it misses a lot of functions presumably because it got ifdefd out(Mario Kart Wii has a lot more functions)

namespace nw4r
{
namespace db
{
    void Console_Printf(ConsoleHandle console, const char *txt, ...)
    {
        va_list list;
        va_start(list, txt);
        // stuff got ifdefd out, probably calls Console_VPrintf and got inlined?
        va_end(list);
        // ...
    }

    s32 Console_GetTotalLines(ConsoleHandle console)
    {
        BOOL enabled = OSDisableInterrupts();
        s32 count = console->printTop - console->ringTop;
        if (count < 0)
        {
            count += console->height;
        }

        count = (u16)count;

        if (console->printXPos != 0)
        {
            count = (u16)(count + 1);
        }

        s32 lineCnt = console->ringTopLineCnt + count; // NOTE: no temp according to DWARF
        OSRestoreInterrupts(enabled);

        return lineCnt;
    }
}
}