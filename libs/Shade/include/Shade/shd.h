#pragma once

#include <revolution/os/OSMutex.h>
#include <revolution/gx/GXFifo.h>
#include <revolution/gx/GXFrameBuf.h>
#include <revolution/mem/mem_allocator.h>
#include <revolution/mem/mem_expHeap.h>

#include <nw4r/g3d/g3d_scnroot.h>
#include <Shade/wii_debug.h>
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

extern const u32 shd_init_flag;
extern const long shd_pktbf_sz;

// might be in appli.h? however that doesn't make much sense to me as that's part of the game code
void appInit1();
void appInit2();
void appLoop1();
void appLoop2();
void appExit();

void shdInit1();
void shdInit2();
void shdLoop1();
void shdLoop3(); // what happened to loop 2?

void shdResetRenderFlag();
u32 shdGetTimeMSec();

typedef void (*shade_resetCb)(void);

struct mcb0Entry
{
    u32 id;
    u32 offset;
    u32 size;
};

struct SHD_TEXCUT;

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
    s8 fbFormat;         // might also be char/s8
    void *fifobuf;
    u8 _58[0xdc - 0x58];
    GXFifoObj *fifoObj;
    u8 _E0[0xEC - 0xE0];
    GXRenderModeObj *rmode;
    u32 vi_yDiff; // difference of xfb height and efb height
    f32 vi_yScale;
    nw4r::g3d::ScnRoot *scnRoot;
    u8 _fc[0x138 - 0xfc];
    u8 *pktbf;       // 0x138
    u8 *pktbuf2;     // 0x13c
    u8 *pktbuf3;     // 0x140
    u32 *anotherbuf; // 0x144, 324
    u32 *nextbuf;    // 0x148
    char mcbFlags; // -2 = create?, -1 = error opening, 0 = closed, 1 = open
    u8 *mcbBuf;
    int mcbBufSize;
    mcb0Entry *mcbTblBuf;
    int mcbTblSize;
    int mcbTblBufSize;
    u32 openMcbId; // 0x164
    u32 _168;      // unknown, always 0
    char *mcb0Path;
    char *mcb1Path;
    u32 *ftyp_ftime;
    s16 field_0x178;
    s16 field_0x17a;
    SHD_TEXCUT *texcut;
    float shadow_scale;
    u32 field_0x184;
    u32 field_0x188;
    s16 field_0x18c;
    s8 field_0x18e[10];
    s8 state_clip;
    s64 mainLoopStartTime; // this might need to be renamed, it's probably used to measure how long the previous loop took
    s64 mainLoopEndTime;
    s64 loop1EndTime;
    s64 loop1StartTime;
    s64 ramEndTime;
    s64 ramStartTime;
    s64 gpuEndTime;
    s64 gpuStartTime;
    u8 _0x1e0[0x200 - 0x1e0];
    u8 field_0x200;
    u8 _0x201[0x208 - 0x201];
};
/* struct size:
 JP: 0x1a0
 2012: 0x210
 EU: 0x218
 GO2013: 0x208
*/

struct shade_work
{
    u32 flags;
    u16 video_flags; // 2 = PAL50 4 = EURGB60
    u16 flags3;
    u16 flags4;
    s16 field4_0xa;
    float aspect_ratio;
    u8 field6_0x10[4]; // padding?
    s16 efb_width;
    s16 efb_height;
    s16 xfb_width;
    s16 xfb_height;
    u16 field_0x1c;
    u16 field_0x1e;
    u16 vi_xOrigin;
    u16 vi_yOrigin;
    s16 field_0x24; // overscan values?
    s16 field_0x26;
    u32 exec_frame;
    u32 field_0x2c;
    int frame_cnted;
    float frame_cntedf;
    float frame_rate;
    f32 field_0x3c;
    u32 cameraFov;
    float cameraNear;
    float cameraFar;
    float cameraAngR;
    float cameraAngL; // not sure
    float cameraUnk;
    float cameraPos[3];
    float cameraAt[3];
    float _field_0x70;
    float _field_0x74;
    float _field_0x78;
    float _field_0x7c;
    float _field_0x80;
    float cameraTarget[3]; // not sure
    u32 field_0x90; // both of these should be GXColor/ut::Color?
    u32 fog;
    float fogStart;
    float fogEnd;
    u32 field_0xa0;
    u32 field_0xa4;
    u32 field_0xa8;
    u32 field_0xac;
    float field_0xb0[3];
    u32 field_0xbc;
    u32 field_0xc0;
    u8 _C4[0xD4 - 0xC4];
    shade_resetCb resetCb;
    s16 field_0xd8;
    s16 field_0xda;
    u8 field_0xdc;
    u8 field_0xdd;
    u8 _DE[0x1c60 - 0xDE];
    short field_0x1c60;
    short field_0x1c62;
    short field_0x1c64;
    short field_0x1c66;
    short field_0x1c68;
    s8 field_0x1c6a;
    char isWideMode;
    char field_0x1c6c;
    char field_0x1c6d;
    u8 _0x1c6e[0x1c7c - 0x1c6e];
};

/*
 struct size:
 JP: 0x1ca0
 2012: 0x1c9c
 EU: 0x1ca4
 GO2013: 0x1c7c
*/

extern shade_sys shdsys;
extern shade_work shdwk;

extern char shd_CapFilename[64];

extern void *FrameBuffer[2];
extern int FrameBufferNo;
extern u32 RetraceCount;

extern GXRenderModeObj RMObject;