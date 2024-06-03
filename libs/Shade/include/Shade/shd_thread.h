#pragma once

#include <revolution/OS/OSThread.h>

struct SHD_THREAD
{
    u32 unlocked;
    OSThread thread;
    OSThreadFunc func;
    void *arg;
    u8 *stack;
    u32 stackSize;
    s32 prio;
};

void *th_func(void *arg);
void shdCreateThread(SHD_THREAD *t);
BOOL shdIsFinishedThread(SHD_THREAD *t);
void shdResumeThread(SHD_THREAD *t);
