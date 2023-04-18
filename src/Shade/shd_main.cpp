// WIP
// this uses -ipa file most likely
#include <revolution/gx/GXPixel.h>
#include <Shade/wii_debug.h>
#include <Shade/shd.h>
#include <Shade/shd_calc.h>
#include <Shade/shd_debug.h>
#include <Shade/shd_memory.h>
#include <Shade/shd_pad.h>
#include <Shade/std/shd_cstring.h>

#include "Game/appli.h"
#include "nw4r/g3d/g3d_camera.h"

#define LINE 112

// Dates:
// JP: Jun  1 2011
// 2012: Oct  5 2011
// EU: Jul 10 2012
// 2013: Oct 29 2012

long shd_pktbf_sz = 0x180000; // is 0x200000 in EU and 2012, not sure where this belongs
nw4r::g3d::Camera::PostureInfo g_info;

static float g_vUp[3] = {
    0.0f, 1.0f, 0.0f};

//static shade_sys shdsys;

inline void wiiInitRenderFlag(char cmp)
{
    shdsys.field_0x18e[1] = cmp;
    if (shdsys.field_0x18e[1] > 0)
    {
        if (shdsys.field_0x18e[2] > 0)
        {
            GXSetZMode(1, GX_LEQUAL, 1);
        }
        else
        {
            GXSetZMode(1, GX_LEQUAL, 0);
        }
    }
    else
    {
        GXSetZMode(0, GX_NEVER, 0);
    }
}

inline void wiiSetStateZcmp(char cmp)
{
    if (cmp != shdsys.field_0x18e[1])
    {
        wiiInitRenderFlag(cmp);
    }
}

void shdInit1(void)
{
    char buf[64];
    int date[3];
    static long shd_init_flag;

    shdwk.flags |= 0x2000e;
    if (!(shd_init_flag & 1))
    {
        hstrcpy(buf, "16:06:26");
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = '\0'; // hacky way to get rid of :
        shdDebDateCnv("Oct 29 2012", date);

        cprintf("\n**** Shade game library (D9) ver%d.%d.%d.%s ****\n", date[0] - 2000, date[1], date[2], buf); // -2000 to only display last 2 digits
        // TODO: make defines
        cprintf("**** RVL SDKVER      : 11Dec2009 ****\n");
        cprintf("**** RVL_SDKPATCH    : Patch02 ****\n");
        cprintf("**** RVL_SDKVERPATCH : 11Dec2009Patch02 ****\n");
    }

    shdsys.pktbf = (u8 *)shdMemGet(shd_pktbf_sz, 64, 0);
    ASSERT(shdsys.pktbf, LINE);

    shdsys.nextbuf = (u32 *)shdMemGet(0x8100, 64, 0);

    shdwk.field_0x1c6a = 32;
    shdwk.frame_cnted = shdwk.field_0x1c6a;

    shdwk.frame_cntedf = (f32)shdwk.frame_cnted;
    shdwk.frame_rate = shdwk.frame_cntedf / 32.0f;

    shdwk.efb_width = 640;
    shdwk.xfb_width = 640;
    shdwk.efb_height = 480;
    shdwk.xfb_height = 480;
    shdwk.field_0x24 = 16;
    shdwk.field_0x26 = 24;
    //
    shdsys.field_0x184 = 0x1005;
    shdsys.field_0x188 = 0x1009;
    shdsys.vi_yDiff = 0;
    shdsys.vi_yScale = 1.0f;
    //
    shdwk.field_0xdc = 4;
    shdwk.field_0xdd = 0;
    // TODO: names + figure out if this is a matrix?
    shdwk.field_0x64[1] = 0.0f;
    shdwk.field_0x64[0] = 0.0f;
    shdwk.field_0x64[2] = 2.0f;
    shdwk.field_0x58[0] = 0.0f;
    shdwk.field_0x58[2] = 4.0f;
    shdwk.field_0x58[1] = 4.0f;
    shdwk.field_0x84[2] = 0.0f;
    shdwk.field_0x84[1] = 0.0f;
    shdwk.field_0x84[0] = 0.0f;
    shdwk.field_0x40 = 25;
    shdwk.field_0x50[1] = 0.0f;
    shdwk.field_0x50[0] = 1.0f;
    shdwk.field_0x44[0] = 0.1f;
    shdwk.field_0x44[1] = 1280.0f;
    shdwk.field_0x44[2] = 32.0f;
    shdCamCalAng();
    //
    g_info.INT_0x0 = 0;
    g_info.VEC3_0x04 = g_vUp;
    g_info.VEC3_0x10 = nw4r::math::VEC3(0, 0, 0);
    //
    shdwk.field_0xc0 = 0x404040; // probably another GXColor struct?
    shdwk.field_0xbc = 0xffffff;
    shdwk.field_0xb0[0] = 1.0f;
    shdwk.field_0xb0[1] = 0.5f;
    shdwk.field_0xb0[2] = -2.0f;
    shdCalNormal(shdwk.field_0xb0);
    shdwk.field_0x90 = 0x606060;
    shdwk.fog = 0x606060;
    shdwk.field_0x98 = 8.0f;
    shdwk.field_0x9c = 64.0f;
    shdwk.field_0xa0 = 1;
    shdwk.field_0xa8 = 0xff000000;
    shdwk.field_0xac = 0xff000000;
    shdsys.field_0x180 = 0.2f;
    shdwk.flags3 = 0;
    shdwk.flags4 = 0;
    shdwk.field4_0xa = 0x80;
    shdwk.exec_frame = 0;
    shdwk.field_0x2c = 0;
    shdwk.field_0x1c68 = 1000;
    shdwk.field_0x1c66 = 1000;
    shdwk.field_0x1c64 = 1000;
    shdwk.field_0x1c62 = 1000;
    shdwk.field_0x1c60 = 1000;
    shd_CapFilename[0] = '\0';

    shdPadInit();
    shdRndInit(shdGetTimeMSec());
    appInit1();
}

void shdInit2()
{
    appInit2();

    // NOTE: this could be all inline, wiiErrDisp and shdLoop3 have the exact same code
    long extraSize = shd_pktbf_sz / 2;
    shdsys.anotherbuf = shdsys.nextbuf;

    if ((shdwk.exec_frame & 1) != 0)
    {
        shdsys.pktbuf2 = shdsys.pktbf;
    }
    else
    {
        shdsys.pktbuf2 = shdsys.pktbf + shd_pktbf_sz / 2;
    }
    shdsys.pktbuf3 = shdsys.pktbuf2 + extraSize;

    for (int i = 0; i < 8256; i += 2)
    {
        shdsys.anotherbuf[i + 1] = 0;
        shdsys.anotherbuf[i] = 0;
    }
    shdResetRenderFlag();
}

inline void shdResetRenderFlag()
{
    shdsys.field_0x18e[3] = -1;
    shdsys.field_0x18e[10] = -1;
    shdsys.field_0x18e[9] = -1;
    shdsys.field_0x18e[8] = -1;
    shdsys.field_0x18e[7] = -1;

    shdsys.field_0x18e[6] = -1;
    shdsys.field_0x18e[5] = -1;
    shdsys.field_0x18e[4] = -1;
    shdsys.field_0x18e[0] = -1;
    shdsys.field_0x18c = -99;

    shdsys.field_0x18e[2] = 1;
    wiiInitRenderFlag(1);
    shdsys.field_0x200 = 0;
}