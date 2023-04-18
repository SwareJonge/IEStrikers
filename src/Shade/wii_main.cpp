#include <nw4r/g3d/g3d_init.h>
#include <revolution/GX.h>
#include <revolution/os/OS.h>
#include <revolution/os/OSArena.h>
#include <revolution/os/OSError.h>
#include <revolution/sc/scapi.h>
#include <revolution/vi/vi.h>
#include <Shade/shd.h>
#include <Shade/shd_memory.h>
#include <Shade/wii.h>
#include <Shade/wii_debug.h>
// WIP

int vcnted = 32;
const GXRenderModeObj *RMNtscInt  /* = GXNtsc480IntDf*/;
const GXRenderModeObj *RMNtscProg /* = GXNtsc480Prog*/;
const GXRenderModeObj *RMPalInt   /* = GXPal528IntDf*/;
const GXRenderModeObj *RMPalProg  /* = GXEurgb60Hz480Prog*/;
const GXRenderModeObj *RMPal60Hz  /* = GXEurgb60Hz480Int*/;

void *Arena1;
void *Arena2;

void *FrameBuffer[2];
int FrameBufferNo;
u32 RetraceCount;

OSMutex AllocListMutex;
void *AllocList[64];

void *shdAllocatorAllocForExpHeap_(MEMAllocator *allocator, u32 size);
void shdAllocatorFreeForExpHeap_(MEMAllocator *allocator, void *block);
void *shdAllocator2AllocForExpHeap_(MEMAllocator *allocator, u32 size);
void shdAllocator2FreeForExpHeap_(MEMAllocator *allocator, void *block);

// mostly copied from Ghidra, misses a lot of definitions currently
/*void main(int argc, char *argv[]) {
    OSInit();
    DVDInit();
    VIInit();
    memset(&shdwk, 0, sizeof(shade_work));
    memset(&shdsys, 0, sizeof(shade_sys));
    memset(&wiiwk, 0, sizeof(wii_work));

    while (SCCheckStatus() == 1);

    OSSetPowerCallback(power_cb_func);
    OSSetResetCallback(reset_cb_func);

    if (!shdInitMem())
        return;

    shdInit1();

    if (!shdInitGrp())
        return;

    for(int i = 0; i < 8; i++) {
        OSInitMutex(&wiiwk.mutexarray[i]);
    }

    //OSInitMutex(0x8090022c);
    //OSInitMutex(0x80900244);
    //OSInitMutex(0x8090025c);
    //OSInitMutex(0x80900274);
    //OSInitMutex(0x8090028c);

    wiiwk[344] = 0;
    Flash_Init();
    WPFlash_Init();
    VISetPostRetraceCallback(vint_cb);
    wiiwk._324_4_ = 0;
    wiiwk._332_4_ = 0;
    wiiwk[343] = 1;
    wiiConInit();
    shdInit2();
    shdPadInput();
}*/

void shdInitAllocatorForExpHeap(MEMAllocator *allocator, MEMiHeapHead *heapend, int alignment) {
    static const MEMAllocatorFuncs sAllocatorFunc = { shdAllocatorAllocForExpHeap_, shdAllocatorFreeForExpHeap_ };
    allocator->funcs = &sAllocatorFunc;
    allocator->heap = heapend;
    allocator->heapParam1 = alignment;
    allocator->heapParam2 = 0;
}

void shdInitAllocator2ForExpHeap(MEMAllocator *allocator, MEMiHeapHead *heapend, int alignment)
{
    static MEMAllocatorFuncs sAllocatorFunc = {shdAllocator2AllocForExpHeap_, shdAllocator2FreeForExpHeap_};
    allocator->funcs = &sAllocatorFunc;
    allocator->heap = heapend;
    allocator->heapParam1 = alignment;
    allocator->heapParam2 = 0;
}

// i guess this is matched?
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

    shdInitAllocatorForExpHeap(&shdsys.mem1Allocator, shdsys.mem2Heap, 32);
    shdInitAllocatorForExpHeap(&shdsys.mem2Allocator, shdsys.mem2Heap, 32);
    shdInitAllocator2ForExpHeap(&shdsys.mem1Allocator2, shdsys.mem2Heap, 32);
    shdInitAllocator2ForExpHeap(&shdsys.mem2Allocator2, shdsys.mem2Heap, 32);

    for(int i = 0; i < 64; i++) {
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

// matched
int shdInitGrp()
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
    {
        shdsys.progressive_mode = 1;
    }

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
            if ((shdwk.video_flags & 2) != 0)
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
    if (shdsys.videomode == 1) {
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
        GXSetPixelFmt(2, 0);
        shdsys.aaMode = 2;
    }
    else
    {
        GXSetPixelFmt(1, 0);
        shdsys.aaMode = 0;
    }
    
    FrameBufferNo = 0;
    GXCopyDisp(FrameBuffer[FrameBufferNo], 1);
    VISetNextFrameBuffer(FrameBuffer[FrameBufferNo]);
    FrameBufferNo ^= 1;
    VISetBlack(1);
    RetraceCount = VIGetRetraceCount();
    VIFlush();
    nw4r::g3d::G3dInit(true);
    nw4r::g3d::G3DState::SetRenderModeObj(*rmode);

    return 1;
}