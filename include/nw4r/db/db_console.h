#pragma once

#include "types.h"

// Copied form MKW-SP

namespace nw4r {
namespace db {
    namespace detail {
        struct ConsoleHead
        {
            u8 _00[0x06 - 0x00];
            u16 _06;
            u16 _08;
            u16 _0A;
            u16 _0C;
            u16 _0E;
            u16 _10;
            s32 mLineCount;
            s32 mCurrentTopLine;
            s16 mPosX;
            s16 mPosY;
            u16 mCurLine;
            bool mIsVisible;
            u8 _23[0x2c - 0x23];
        }; // Size: 0x2c
    }
    typedef detail::ConsoleHead *ConsoleHandle;

    void Console_Printf(ConsoleHandle, const char *, ...);
    s32 Console_GetTotalLines(ConsoleHandle);
    extern ConsoleHandle sAssertionConsole;
}
}