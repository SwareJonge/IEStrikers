#pragma once

#include <revolution/os/OSMutex.h>
#include <revolution/gx/GXFifo.h>
#include <revolution/gx/GXFrameBuf.h>
#include <revolution/mem/mem_allocator.h>
#include <revolution/mem/mem_expHeap.h>

#include "types.h"

// shd init flag options

#define DISABLE_DEBUG 1
#define UNK_0x2 2
#define ENABLE_HBM 4
#define FORCE_4_3 0x8
#define FORCE_16_9 0x10
#define DVD_THING 0x100 // something related to dvd file loading, always resets readsize to 0 (8032eaac)
#define ENABLE_WIIMOTE 0x200
#define UNK_0x400 0x400 // wait for retrace?
#define ENABLE_GCN 0x800

extern u32 shd_init_flag;

void shdInit1();
void shdInit2();
void shdResetRenderFlag();
u32 shdGetTimeMSec();

struct shade_sys
{
    MEMiHeapHead *mem1Heap;
    MEMiHeapHead *mem2Heap;
    u32 mem1Size;
    u32 mem2Size;
    MEMAllocator mem1Allocator;
    MEMAllocator mem2Allocator;
    MEMAllocator mem1Allocator2;
    MEMAllocator mem2Allocator2;
    s8 wide_mode;        // 0 = 4:3, 1 = 16:9/widescreen
    s8 progressive_mode; // 0 is disabled, 1 = enabled
    s8 videomode;        // 0 = NTSC, 1 = PAL
    u8 aaMode;           // might also be char/s8
    void *fifobuf;
    u8 _58[0xdc - 0x58];
    GXFifoObj *fifoObj;
    u8 _E0[0xEC - 0xE0];
    GXRenderModeObj *rmode;
    u32 vi_yDiff; // difference of xfb height and efb height
    f32 vi_yScale;
    u8 _f8[0x138 - 0xf8];
    u8 *pktbf;       // 0x138
    u8 *pktbuf2;     // 0x13c
    u8 *pktbuf3;     // 0x144
    u32 *anotherbuf; // 0x144, 324
    u32 *nextbuf;    // 0x148
    u8 _14c[0x180 - 0x14c];
    f32 field_0x180;
    u32 field_0x184;
    u32 field_0x188;
    s16 field_0x18c;
    s8 field_0x18e[10];
    u8 _198[0x1b0 - 0x198];
    s64 loop1EndTime;
    s64 loop1StartTime;
    u8 _0x1c0[0x200 - 0x1c0];
    u8 field_0x200;
    u8 _0x201[0x208 - 0x201];
}; // Size: 0x208

struct shade_work
{
    u32 flags;
    u16 video_flags; // 2 = PAL50 4 = EURGB60
    u16 flags3;
    u16 flags4;
    s16 field4_0xa;
    float aspect_ratio;
    u8 field6_0x10[4];
    s16 efb_width;
    s16 efb_height;
    u16 xfb_width; // could also be s16
    u16 xfb_height;
    u16 field_0x1c;
    u16 field_0x1e;
    u16 vi_xOrigin;
    u16 vi_yOrigin;
    u16 field_0x24;
    u16 field_0x26;
    u32 exec_frame;
    u32 field_0x2c;
    int frame_cnted;
    float frame_cntedf;
    float frame_rate;
    u32 field_0x3c;
    u32 field_0x40;
    float field_0x44[3];
    float field_0x50[2];
    float field_0x58[3];
    float field_0x64[3];
    u8 _0x70[0x84 - 0x70];
    float field_0x84[3];
    u32 field_0x90; // both of these should be GXColor/ut::Color?
    u32 fog;
    float field_0x98;
    float field_0x9c;
    u32 field_0xa0;
    u32 field_0xa4;
    u32 field_0xa8;
    u32 field_0xac;
    float field_0xb0[3];
    u32 field_0xbc;
    u32 field_0xc0;
    u8 _C4[0xDC - 0xC4];
    u8 field_0xdc;
    u8 field_0xdd;
    u8 _DE[0x1c60 - 0xDE];
    short field_0x1c60;
    short field_0x1c62;
    short field_0x1c64;
    short field_0x1c66;
    short field_0x1c68;
    u8 field_0x1c6a;
    u8 _0x1c6a[0x1c7c - 0x1c6b];
}; // Size: 0x1C7C

extern shade_sys shdsys;
extern shade_work shdwk;

extern char shd_CapFilename[64];

extern void *FrameBuffer[2];
extern int FrameBufferNo;
extern u32 RetraceCount;

extern GXRenderModeObj RMObject;