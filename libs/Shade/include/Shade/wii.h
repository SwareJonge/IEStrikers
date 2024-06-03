#pragma once

#include <revolution/OS/OSMutex.h>

bool wiiCheckReqReset() ;

/*static */int shdInitGrp();
void wiiResetGP();
void wiiResetChk() ;
void wiiSetWideMode(int);
/*static */ void wiiExit(int, int);
void shdFreeMemAllocList();
void shdBeforeRender();
void shdDoneRender();
void shdSwapBuffers();
void wiiSetFrameBufferFormat(int fmt);
void wiiSetProgFilter(bool enable);
void wiiVintWait(int cycles);

bool wiiIsExecThread();
void shdInitMutex();
void shdLockMutex(int idx);
void shdUnlockMutex(int idx);
void wiiLockMutexMcb();
void wiiUnlockMutexMcb();
void wiiLockMutexGX();
void wiiUnlockMutexGX();

void wiiErrDisp(char *);

// check Suzumiya Haruhi no Heiretsu or Mahou Sensei Negima!? Neo-Pactio Fight!! for maybe more info, unfortunately no symbols

struct wii_work
{
    u8 _0[0xc]; // unknown, padding or unused
    OSMutex threadMutex;
    OSMutex consoleMutex;
    OSMutex soundMutex;
    OSMutex mcbMutex;
    OSMutex gxMutex;
    OSMutex mutexArray[8]; // 0 = another thread mutex, 5 is used for MEM, 7 is used for wifi
    volatile u32 intervals;
    volatile u32 retraceCount;
    volatile int _14C; // seems to be always at 0
    u8 _150[6];        // unknown, padding or unused
    u8 activeSpeakers; // flags related to the wiimote speakers?
    char fadeDone;     // not entirely sure if this is named properly
    char threadActive; // doesn't exist in JP Strikers
    u8 resetFlags;     // 1, = shutdown, 2 = reset, 4 = soft reset, 8 = wii menu, 0x10 = data manager
    u8 oldResetFlags;  // the old reset flags, needed for shutdown functions(EU and GO2013 only)
};                     // size: 0x15c

#if defined(JP_STRIKERS) || defined(STRIKERS2012XTREME)
#define OLD_RESET_FLAGS wiiwk.resetFlags
#else
#define OLD_RESET_FLAGS wiiwk.oldResetFlags
#endif

extern wii_work wiiwk;