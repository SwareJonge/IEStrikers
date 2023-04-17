#pragma once

#include "types.h"

extern u32 shd_init_flag;

void shdInit1();
void shdInit2();
void shdResetRenderFlag();
u32 shdGetTimeMSec();

struct shade_sys
{
    u32 flags;
    u8 _4[0xf0 - 0x4];
    u32 field_0xf0;
    f32 field_0xf4;
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
    u8 _198[0x200 - 0x198];
    u8 field_0x200;
};

struct shade_work
{
    u32 flags;
    u16 flags2;
    u16 flags3;
    u16 flags4;
    short field4_0xa;
    float aspect_ratio;
    u8 field6_0x10[4];
    u16 xfb_height;
    u16 xfb_width;
    u16 efb_height;
    u16 efb_width;
    u16 field_0x1c;
    u16 field_0x1e;
    u16 field_0x20;
    u16 field_0x22;
    u16 field_0x24;
    u16 field_0x26;
    u32 exec_frame;
    u32 field_0x2c;
    int field_0x30;
    float field_0x34;
    float field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    float field_0x44[3];
    float field_0x50[2];
    float field_0x58[3];
    float field_0x64[3];
    u8 _0x70[0x84 - 0x70];
    float field_0x84[3];
    u32 field_0x90; // both of these should be GXColor?
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