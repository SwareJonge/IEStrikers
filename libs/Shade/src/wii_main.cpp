#include <string.h>

#include <revolution/GX.h>
#include <revolution/DVD.h>
#include <revolution/OS.h>
#include <revolution/SC.h>
#include <revolution/VI.h>
#include <nw4r/lyt/lyt_init.h>
#include <nw4r/g3d/g3d_init.h>

#include <Shade/shd.h>
#include <Shade/shd_memory.h>
#include <Shade/shd_pad.h>
#include <Shade/shd_thread.h>
#include <Shade/std/shd_stdarg.h>
#include <Shade/wii.h>
#include <Shade/wii_debug.h>
#include <Shade/wii_pad.h>
#include <Shade/wii_sound.h>
#include <Shade/HBM/HBMenu.h>
#include <Shade/HBM/wii_flash.h>
#include <Shade/HBM/wii_pad_flash.h>
#include <Shade/movie/movie.h>

// NOTE: this file currently compiled with a bit of hackery, some functions here should be static local but because of extab issues i can't do that(yet)

int vcnted = 32;
const GXRenderModeObj *RMNtscInt = &GXNtsc480IntDf;
const GXRenderModeObj *RMNtscProg = &GXNtsc480Prog;
const GXRenderModeObj *RMPalInt = &GXPal528IntDf;
const GXRenderModeObj *RMPalProg = &GXEurgb60Hz480Prog;
const GXRenderModeObj *RMPal60Hz = &GXEurgb60Hz480Int;

void *Arena1;
void *Arena2;

void *FrameBuffer[2];
int FrameBufferNo;
u32 RetraceCount;

wii_work wiiwk;
OSMutex AllocListMutex;
void *AllocList[64];
GXRenderModeObj RMObject;

static void power_cb_func();
static void reset_cb_func();
static void vint_cb(u32 retraceCount);

static void calc_vsync_cnted();

void main(int argc, char *argv[])
{
    OSInit();
    DVDInit();
    VIInit();
    memset(&shdwk,  0, sizeof(shade_work));
    memset(&shdsys, 0, sizeof(shade_sys));
    memset(&wiiwk,  0, sizeof(wii_work));

    while (SCCheckStatus() == 1)
        ;

    OSSetPowerCallback(power_cb_func);
    OSSetResetCallback(reset_cb_func);

    if (!shdInitMem())
        return;

    shdInit1();

    if (!shdInitGrp())
        return;

    nw4r::lyt::LytInit();

    shdInitMutex(); // fakematch?

    OSInitMutex(&wiiwk.threadMutex);
    OSInitMutex(&wiiwk.consoleMutex);
    OSInitMutex(&wiiwk.soundMutex);
    OSInitMutex(&wiiwk.mcbMutex);
    OSInitMutex(&wiiwk.gxMutex);

    wiiwk.threadActive = 0;
    Flash_Init();
    WPFlash_Init();

    VISetPostRetraceCallback(vint_cb);
    wiiwk.intervals = 0;
    wiiwk._14C = 0;
    wiiwk.fadeDone = 1;

    wiiConInit();
    shdInit2();
    shdPadInput();

    while (true)
    {
        shdsys.mainLoopEndTime = shdsys.loop1EndTime; // bug?
        shdsys.mainLoopStartTime = OSGetTime();
        shdLoop1();
        Flash_Exec();
        WPFlash_Exec();
        wiiSndUpdate();
        wiiResetChk();
        shdLoop3();

        calc_vsync_cnted();
        if (shdwk.flags & 8 && (shdwk.flags & 0x10) == 0)
        {
            int cpuTime = OS_TICKS_TO_USEC(OS_DIFF_TICK(shdsys.loop1EndTime, shdsys.loop1StartTime));
            int ramTime = OS_TICKS_TO_USEC(OS_DIFF_TICK(shdsys.ramEndTime, shdsys.ramStartTime));
            int gpuTime = OS_TICKS_TO_USEC(OS_DIFF_TICK(shdsys.gpuEndTime, shdsys.gpuStartTime));
            int totalTime = OS_TICKS_TO_USEC(OS_DIFF_TICK(shdsys.mainLoopStartTime, shdsys.mainLoopEndTime));
            bprintf("C:%5d.%03dus R:%5d.%03dus G:%5d.%03dus <ALL=%5d.%03d>\n",
                    cpuTime / 1000, cpuTime % 1000,
                    ramTime / 1000, ramTime % 1000,
                    gpuTime / 1000, gpuTime % 1000,
                    totalTime / 1000, totalTime % 1000);
        }
    }
}

u32 shdGetTimeMSec()
{
    return OS_MSEC_TO_TICKS(OSGetTick()); // bug?
}

void calc_vsync_cnted()
{
    static int tick_start, tick_last;

    int vsync = shdwk.field_0x1c6a; // from what i know, this is always 32
    tick_start = OSGetTick();

    s32 v = OS_TICKS_TO_USEC(OS_DIFF_TICK(tick_start, tick_last)) / 515;
    if (v > vsync * 4) // movie mode / 20fps?
    {
        v = vsync * 4;
    }
    if ((shdwk.flags & 0x800000) != 0) // 30 fps mode
    {
        v = vsync * 2;
    }

    tick_last = tick_start;

    shdwk.field_0x3c = v / 32.f;
    vcnted = v * shdwk.field4_0xa;

    shdwk.frame_cntedf = vcnted / 128.f;
    shdwk.frame_cnted = vcnted / 128;
    shdwk.frame_rate = vcnted / 4096.f;
}

static void vint_cb(u32 retraceCount)
{
    wiiwk.retraceCount = retraceCount;
    wiiwk.intervals++;

    if (wiiwk._14C == 0)
        return;

    wiiwk._14C--;
}

void *shdMemGet(size_t size, int alignment, s8 flag)
{
    void *mem = NULL;
    if (shdwk.flags & 0x10000000 && alignment > 0)
    {
        alignment = -alignment;
    }
    if (flag == 0 || flag == 2)
    {
        mem = MEMAllocFromExpHeapEx(shdsys.mem1Heap, size, alignment);
    }
    else if (flag == 1 || flag == 3)
    {
        mem = MEMAllocFromExpHeapEx(shdsys.mem2Heap, size, alignment);
    }

    if (mem == NULL)
    {
        if (flag == 2)
            mem = shdMemGet(size, alignment, 1);
        else if (flag == 3)
            mem = shdMemGet(size, alignment, 0);
    }
    return mem;
}

void shdMemFree(void *mem)
{
    if ((u32)mem < 0x90000000)
        MEMFreeToExpHeap(shdsys.mem1Heap, mem);
    else
        MEMFreeToExpHeap(shdsys.mem2Heap, mem);
}

MEMAllocator *shdGetAllocator(int region)
{
    if (region == 0)
        return &shdsys.mem1Allocator;
    else
        return &shdsys.mem2Allocator;
}

MEMAllocator *shdGetAllocator2(int region)
{
    if (region == 0)
        return &shdsys.mem1Allocator2;
    else
        return &shdsys.mem2Allocator2;
}

static void shdInitAllocatorForExpHeap(MEMAllocator *allocator, MEMiHeapHead *heapend, int alignment)
{
    static const MEMAllocatorFuncs sAllocatorFunc = {shdAllocatorAllocForExpHeap_, shdAllocatorFreeForExpHeap_};
    allocator->funcs = &sAllocatorFunc;
    allocator->heap = heapend;
    allocator->heapParam1 = alignment;
    allocator->heapParam2 = 0;
}

static void shdInitAllocator2ForExpHeap(MEMAllocator *allocator, MEMiHeapHead *heapend, int alignment)
{
    static const MEMAllocatorFuncs sAllocatorFunc = {shdAllocator2AllocForExpHeap_, shdAllocator2FreeForExpHeap_};
    allocator->funcs = &sAllocatorFunc;
    allocator->heap = heapend;
    allocator->heapParam1 = alignment;
    allocator->heapParam2 = 0;
}

int shdInitMem()
{
    shdsys.mem1Size = (u32)OSGetMEM1ArenaHi() - (u32)OSGetMEM1ArenaLo();
    shdsys.mem2Size = (u32)OSGetMEM2ArenaHi() - (u32)OSGetMEM2ArenaLo();

    Arena1 = OSAllocFromMEM1ArenaLo(shdsys.mem1Size, 32);
    Arena2 = OSAllocFromMEM2ArenaLo(shdsys.mem2Size, 32);

    shdsys.mem1Heap = MEMCreateExpHeapEx(Arena1, shdsys.mem1Size, 4);
    shdsys.mem2Heap = MEMCreateExpHeapEx(Arena2, shdsys.mem2Size, 4);

    if (shdsys.mem1Heap == nullptr || shdsys.mem2Heap == nullptr)
    {
        if (shdsys.mem1Heap == nullptr)
            cprintf("Mem1 CreateHeap Error\n");
        if (shdsys.mem2Heap == nullptr)
            cprintf("Mem2 CreateHeap Error\n");

        return 0;
    }

    shdInitAllocatorForExpHeap(&shdsys.mem1Allocator, shdsys.mem1Heap, 32);
    shdInitAllocatorForExpHeap(&shdsys.mem2Allocator, shdsys.mem2Heap, 32);
    shdInitAllocator2ForExpHeap(&shdsys.mem1Allocator2, shdsys.mem1Heap, 32);
    shdInitAllocator2ForExpHeap(&shdsys.mem2Allocator2, shdsys.mem2Heap, 32);
    OSInitMutex(&AllocListMutex);

    for (int i = 0; i < 64; i++)
    {
        AllocList[i] = nullptr;
    }

    return 1;
}

void *shdAllocatorAllocForExpHeap_(MEMAllocator *allocator, u32 size)
{
    return MEMAllocFromExpHeapEx(allocator->heap, size, allocator->heapParam1);
}

void shdAllocatorFreeForExpHeap_(MEMAllocator *allocator, void *block)
{
    MEMFreeToExpHeap(allocator->heap, block);
}

void *shdAllocator2AllocForExpHeap_(MEMAllocator *allocator, u32 size)
{
    return shdPushMemAllocList(MEMAllocFromExpHeapEx(allocator->heap, size, allocator->heapParam1));
}

void shdAllocator2FreeForExpHeap_(MEMAllocator *allocator, void *block)
{
    MEMFreeToExpHeap(allocator->heap, block);
    shdPopMemAllocList(block);
}

u32 shdGetMemFreeSize(int region)
{
    if (region == 0)
        return MEMGetTotalFreeSizeForExpHeap(shdsys.mem1Heap);
    else
        return MEMGetTotalFreeSizeForExpHeap(shdsys.mem2Heap);
}

void shdFreeMemAllocList()
{
    OSLockMutex(&AllocListMutex);
    for (int i = 0; i < 64; i++)
    {
        if (AllocList[i] != NULL)
        {
            if ((u32)AllocList[i] < 0x90000000)
            {
                MEMFreeToExpHeap(shdsys.mem1Heap, AllocList[i]);
            }
            else
            {
                MEMFreeToExpHeap(shdsys.mem2Heap, AllocList[i]);
            }
            AllocList[i] = NULL;
        }
    }
    OSUnlockMutex(&AllocListMutex);
}

void *shdPushMemAllocList(void *block)
{
    OSLockMutex(&AllocListMutex);
    for (int i = 0; i < 64; i++)
    {
        if (AllocList[i] == NULL)
        {
            AllocList[i] = block;
            OSUnlockMutex(&AllocListMutex);
            return block;
        }
    }
    cprintf("*** shdPushMemAllocList list over ***\n");
    OSUnlockMutex(&AllocListMutex);
    return block;
}

void shdPopMemAllocList(void *block)
{
    OSLockMutex(&AllocListMutex);
    for (int i = 0; i < 64; i++)
    {
        if (AllocList[i] == block)
        {
            AllocList[i] = NULL;
            OSUnlockMutex(&AllocListMutex);
            return;
        }
    }
    cprintf("*** shdPopMemAllocList unknown addr ***\n");
    OSUnlockMutex(&AllocListMutex);
}

// matched
/*static*/ int shdInitGrp()
{
    if (SCGetAspectRatio() == SC_ASPECT_STD)
        shdsys.wide_mode = 0;
    else
        shdsys.wide_mode = 1;

    if ((shd_init_flag & FORCE_4_3))
        shdsys.wide_mode = 0;
    else if ((shd_init_flag & FORCE_16_9))
        shdsys.wide_mode = 1;

    shdsys.progressive_mode = 0;
    if (SCGetProgressiveMode() == SC_PROGRESSIVE && VIGetDTVStatus() != 0)
        shdsys.progressive_mode = 1;

    switch (VIGetTvFormat())
    {
    case VI_TV_FMT_NTSC:
        shdsys.videomode = 0;
        break;
    case VI_TV_FMT_PAL:
        shdsys.videomode = 1;
        shdwk.video_flags |= 2;
        shdwk.field_0x1c6a = 39;
        shdwk.frame_cnted = shdwk.field_0x1c6a;
        shdwk.frame_cntedf = (f32)shdwk.frame_cnted;
        shdwk.frame_rate = shdwk.frame_cntedf / 32.0f;
        break;
    case VI_TV_FMT_EURGB60:
        shdsys.videomode = 1;
        shdwk.video_flags |= 4;
        break;
    }

    shdsys.fifobuf = shdMemGet(0x100000, 0x20, 0);
    if (shdsys.fifobuf == 0)
    {
        OSReport("*** shdInitGrp FifoBuff alloc error ***\n");
        return 0;
    }

    shdsys.fifoObj = GXInit(shdsys.fifobuf, 0x100000);
    GXSetMisc(1, 8);
    if (shdsys.progressive_mode == 0)
    {
        switch (shdsys.videomode)
        {
        case 0:
            RMObject = *RMNtscInt;
            break;
        case 1:
            if ((shdwk.video_flags & 2) != 0)
                RMObject = *RMPalInt;
            else
                RMObject = *RMPal60Hz;
            break;
        }
    }
    else
    {
        switch (shdsys.videomode)
        {
        case 0:
            RMObject = *RMNtscProg;
            break;
        case 1:
            RMObject = *RMPalProg;
            break;
        }
    }

    shdsys.rmode = &RMObject;
    GXRenderModeObj *rmode = shdsys.rmode;

    if (shdsys.progressive_mode == 0)
    {
        if (shdsys.videomode == 0)
        { // inconsistency is key
            rmode->fbWidth = 640;
            rmode->efbHeight = 456;
            rmode->xfbHeight = 456;

            rmode->viWidth = 688;
            rmode->viHeight = 456;
            rmode->viXOrigin += -22;
            rmode->viYOrigin = rmode->viYOrigin + 16;
        }
        else
        {
            if ((shdwk.video_flags & 2))
            {
                rmode->fbWidth = 640;
                rmode->efbHeight = 456;
                rmode->xfbHeight = 528;
                rmode->viWidth = 640;
                rmode->viHeight = 528;
                rmode->viXOrigin = 40;
                rmode->viYOrigin = 22;
            }
            else
            {
                rmode->fbWidth = 640;
                rmode->efbHeight = 456;
                rmode->xfbHeight = 456;
                rmode->viWidth = 688;
                rmode->viHeight = 456;
                rmode->viXOrigin += -18;
                rmode->viYOrigin += 16;
            }
        }
    }
    else
    {
        if (shdsys.videomode == 0)
        {
            rmode->fbWidth = 640;
            rmode->efbHeight = 456;
            rmode->xfbHeight = 456;
            rmode->viWidth = 688;
            rmode->viHeight = 456;
            rmode->viXOrigin += -22;
            rmode->viYOrigin += 16;
        }
        else
        {
            rmode->fbWidth = 640;
            rmode->efbHeight = 456;
            rmode->xfbHeight = 456;
            rmode->viWidth = 688;
            rmode->viHeight = 456;
            rmode->viXOrigin += -18;
            rmode->viYOrigin += 16;
        }
    }

    if (shdsys.wide_mode == 0)
        wiiSetWideMode(0);
    else
        wiiSetWideMode(1);

    shdwk.efb_width = rmode->fbWidth;
    shdwk.efb_height = rmode->efbHeight;
    shdwk.vi_xOrigin = rmode->viXOrigin;
    shdwk.vi_yOrigin = rmode->viYOrigin;
    shdwk.field_0x1c = 0;
    shdwk.field_0x1e = 0;

    VIConfigure(rmode);
    if (shdsys.videomode == 1)
    {
        GXSetDispCopyYScale(GXGetYScaleFactor(rmode->efbHeight, rmode->xfbHeight));
        shdsys.vi_yDiff = rmode->xfbHeight - rmode->efbHeight;
        shdsys.vi_yScale = (f32)rmode->efbHeight / (f32)rmode->xfbHeight;
    }

    GXSetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, rmode->xfbHeight);
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, TRUE, rmode->vfilter);
    GXSetDispCopyGamma(0);

    u32 fbSize = (rmode->fbWidth + 0xf & 0xfff0) * rmode->xfbHeight * 2;
    FrameBuffer[0] = shdMemGet(fbSize, 0x20, 0);
    FrameBuffer[1] = shdMemGet(fbSize, 0x20, 0);
    if ((FrameBuffer[0] == nullptr) || (FrameBuffer[1] == nullptr))
    {
        OSReport("*** shdInitGrp FrameBuff alloc error ***\n");
        return 0;
    }

    if (rmode->aa != 0)
    {
        GXSetPixelFmt(GX_PF_RGBA565_Z16, GX_ZC_LINEAR);
        shdsys.fbFormat = 2;
    }
    else
    {
        GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
        shdsys.fbFormat = 0;
    }

    FrameBufferNo = 0;
    GXCopyDisp(FrameBuffer[FrameBufferNo], 1);
    VISetNextFrameBuffer(FrameBuffer[FrameBufferNo]);
    FrameBufferNo ^= 1;
    VISetBlack(TRUE);
    RetraceCount = VIGetRetraceCount();
    VIFlush();
    nw4r::g3d::G3dInit(true);
    nw4r::g3d::G3DState::SetRenderModeObj(*rmode);

    return 1;
}

void shdBeforeRender()
{
    GXRenderModeObj *rmode = shdsys.rmode;
    if (rmode->field_rendering != 0)
    {
        GXSetViewportJitter(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1, VIGetNextField());
    }
    else
    {
        GXSetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1);
    }

    nw4r::g3d::G3DState::Invalidate(0x7ff);

    GXSetAlphaUpdate(0);
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
    if (shdsys.scnRoot != NULL)
    {
        nw4r::g3d::Camera cam = shdsys.scnRoot->GetCurrentCamera();
        cam.SetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
        cam.SetScissorBoxOffset(0, 0);
    }
}

void shdDoneRender()
{
    GXRenderModeObj *rmode = shdsys.rmode;
    GXSetZMode(TRUE, GX_LEQUAL, TRUE);
    GXSetColorUpdate(TRUE);
    GXSetAlphaUpdate(TRUE);
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, 1, rmode->vfilter);
    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXCopyDisp(FrameBuffer[FrameBufferNo], 1);
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, 0, rmode->vfilter);
    GXDrawDone();
    GXSetAlphaUpdate(FALSE);
}

void shdSwapBuffers()
{
    VISetNextFrameBuffer(FrameBuffer[FrameBufferNo]);
    if (wiiwk.fadeDone == 0)
    {
        wiiwk.fadeDone = 1;
        VISetBlack(FALSE);
    }
    VIFlush();
    do
    {
        VIWaitForRetrace();
        if ((shdwk.flags & 0x400) == 0 && (shdwk.flags3 & 4) == 0)
            break;

    } while (VIGetRetraceCount() < RetraceCount + 2);
    
    RetraceCount = VIGetRetraceCount();
    FrameBufferNo ^= 1;
}

void wiiSetFrameBufferFormat(int fmt)
{
    shdsys.fbFormat = fmt;
    switch (fmt)
    {
    default:
        GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
        break;
    case 1:
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
        break;
    case 2:
        GXSetPixelFmt(GX_PF_RGBA565_Z16, GX_ZC_LINEAR);
        break;
    }
}

void wiiSetProgFilter(bool enable)
{
    GXRenderModeObj *rmode = shdsys.rmode;
    if (rmode->tvMode != VI_TV_FMT_MPAL)
    {
        return;
    }
    if (rmode->aa != 0)
    {
        return;
    }
    if (enable)
    {
        rmode->vfilter[0] = 4;
        rmode->vfilter[1] = 6;
        rmode->vfilter[2] = 12;
        rmode->vfilter[3] = 20;
        rmode->vfilter[4] = 12;
        rmode->vfilter[5] = 6;
        rmode->vfilter[6] = 4;
        return;
    }
    // no filter, this gives the sharpest output
    rmode->vfilter[0] = 0;
    rmode->vfilter[1] = 0;
    rmode->vfilter[2] = 21;
    rmode->vfilter[3] = 22;
    rmode->vfilter[4] = 21;
    rmode->vfilter[5] = 0;
    rmode->vfilter[6] = 0;
}

void wiiVintWait(int cycles)
{
    u32 intervals = wiiwk.intervals;

    if (cycles <= 0)
        cycles = 1;

    while (cycles--)
    {
        while (intervals == wiiwk.intervals)
        {
        }
        intervals = wiiwk.intervals;
    }
}

void *th_func(void *arg)
{
    SHD_THREAD *shd_thread = (SHD_THREAD *)arg;
    if (shd_thread != NULL)
    {
        shd_thread->func(shd_thread->arg);
    }
    OSLockMutex(&wiiwk.threadMutex);
    shd_thread->unlocked = 0;

    if (0 < wiiwk.threadActive)
    {
        wiiwk.threadActive--;
    }
    OSCancelThread(&shd_thread->thread);
    OSUnlockMutex(&wiiwk.threadMutex);

    return NULL;
}

void shdCreateThread(SHD_THREAD *t)
{
    OSCreateThread(&t->thread, th_func, t, t->stack + t->stackSize, t->stackSize, t->prio, 1);
}

BOOL shdIsFinishedThread(SHD_THREAD *t)
{
    return OSIsThreadTerminated(&t->thread);
}

void shdResumeThread(SHD_THREAD *t)
{
    OSLockMutex(&wiiwk.threadMutex);
    wiiwk.threadActive++;
    OSUnlockMutex(&wiiwk.threadMutex);
    OSResumeThread(&t->thread);
}

bool wiiIsExecThread()
{
    OSLockMutex(&wiiwk.threadMutex);
    bool exec = wiiwk.threadActive != 0;
    OSUnlockMutex(&wiiwk.threadMutex);
    return exec;
}

void shdInitMutex()
{
    for (int i = 0; i < 8; i++)
    {
        OSInitMutex(&wiiwk.mutexArray[i]);
    }
}

void shdLockMutex(int idx)
{
    OSLockMutex(&wiiwk.mutexArray[idx]);
}

void shdUnlockMutex(int idx)
{
    OSUnlockMutex(&wiiwk.mutexArray[idx]);
}

void wiiLockMutexMcb()
{
    OSLockMutex(&wiiwk.mcbMutex);
}

void wiiUnlockMutexMcb()
{
    OSUnlockMutex(&wiiwk.mcbMutex);
}

void wiiLockMutexGX()
{
    OSLockMutex(&wiiwk.gxMutex);
}

void wiiUnlockMutexGX()
{
    OSUnlockMutex(&wiiwk.gxMutex);
}

void wiiExit(int exitType, int)
{
    if (Movie_IsPlay() != 0)
    {
        Movie_Stop();
    }

    HBMenu_CleanupData();
    wiiVibAllStop();
    wiiSndFinish1();
    VISetBlack(1);
    VIFlush();
    VIWaitForRetrace();
    VIWaitForRetrace();

    if (exitType == 2)
    {
        PADRecalibrate(0xf0000000);
    }

    wiiSndFinish2();
    appExit();

    if (exitType == 1)
    {
        OSShutdownSystem();
    }
    else if (exitType == 2)
    {
        OSRestart(0);
    }
    else if (exitType == 3)
    {
        OSReturnToMenu();
    }
    else if (exitType == 4)
    {
        OSReturnToDataManager();
    }
    while (true)
    {
    }
}

bool wiiCheckReqReset() // could be int return and != 0
{
    return (wiiwk.resetFlags & 31);
}

void wiiResetChk()
{
    if (((wiiwk.resetFlags & 0x80) == 0 || shdwk.field_0x1c6d != 0))
    {
        if (!Flash_IsAccess() && WPFlash_IsWriteAccess() == 0)
        {
            OLD_RESET_FLAGS = wiiwk.resetFlags;

            if (wiiwk.resetFlags & 1)
            {
                cprintf("***** POWEROFF *****\n");
                wiiwk.resetFlags &= 0xe0;
                wiiExit(1, shdwk.flags3 >> 2 & 1);
            }
            if (wiiwk.resetFlags & 8)
            {
                cprintf("***** WII MENU *****\n");
                wiiwk.resetFlags &= 0xe0;
                wiiExit(3, shdwk.flags3 >> 2 & 1);
            }
            if (wiiwk.resetFlags & 2)
            {
                cprintf("***** RESET *****\n");
                wiiwk.resetFlags &= 0xe0;
                wiiExit(2, shdwk.flags3 >> 2 & 1);
            }
            if (wiiwk.resetFlags & 4)
            {
                cprintf("***** SOFT RESET *****\n");
                wiiwk.resetFlags &= 0xe0;
                if (shdwk.resetCb != NULL)
                    shdwk.resetCb();
            }
            if ((wiiwk.resetFlags & 0x10) != 0)
            {
                wiiExit(4, 0);
            }
        }
    }
}

void wiiGotoDataManager()
{
    wiiwk.resetFlags |= 0x10;
}

void wiiResetEnable(bool enable)
{
    if (enable)
    {
        wiiwk.resetFlags &= 0x7f;
        return;
    }
    wiiwk.resetFlags |= 0x80;
}

static void power_cb_func()
{
    wiiwk.resetFlags |= 1;
}

static void reset_cb_func()
{
    wiiwk.resetFlags |= 2;
}

void wiiTVoutput(bool output)
{
    if (output == true)
    {
        wiiwk.fadeDone = 0;
        return;
    }
    VISetBlack(TRUE);
}