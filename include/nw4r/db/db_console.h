#ifndef NW4R_DB_CONSOLE_H
#define NW4R_DB_CONSOLE_H

#include "types.h"

#include "nw4r/ut/TextWriterBase.h"

namespace nw4r {
namespace db {
    namespace detail {
        struct ConsoleHead
        {
            u8 *textBuf;
            u16 width;
            u16 height;
            u16 priority;
            u16 attr;
            u16 printTop;
            u16 printXPos;
            u16 ringTop;
            s32 ringTopLineCnt;
            s32 viewTopLine;
            s16 viewPosX;
            s16 viewPosY;
            u16 viewLines;
            u8 isVisible;
            u8 padding_; // should be an array but not needed
            ut::TextWriterBase *writer;
            ConsoleHead *next;
        }; // Size: 0x2c
    }
    typedef detail::ConsoleHead *ConsoleHandle;

    void Console_Printf(ConsoleHandle, const char *, ...);
    s32 Console_GetTotalLines(ConsoleHandle);
    extern ConsoleHandle sAssertionConsole;
}
}

#endif