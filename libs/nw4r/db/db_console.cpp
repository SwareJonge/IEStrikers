
#include <revolution/os/OSInterrupt.h>
#include <stdarg.h>
#include "nw4r/db/db_console.h"

// NOTE: this file is incomplete, it misses a lot of functions presumably because it got ifdefd out(Mario Kart Wii has a lot more functions)
// i'm not aware of any games that have DWARF info with this file

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

    // i have trouble believing this is correct
    s32 Console_GetTotalLines(ConsoleHandle console)
    {
        BOOL ret = OSDisableInterrupts();
        int v1 = console->_0C - console->_10;
        if (v1 < 0)
        {
            v1 += console->_06;
        }

        v1 = (u16)v1;

        if (console->_0E != 0)
        {
            v1 = (u16)(v1 + 1);
        }

        s32 lineCnt = console->mLineCount + v1;
        OSRestoreInterrupts(ret);

        return lineCnt;
    }
}
}