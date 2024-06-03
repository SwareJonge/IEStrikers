#include <revolution/OS.h>
#include <revolution/GX.h>
#include <Shade/wii_debug.h>
#include <Shade/shd.h>
#include <Shade/shd_calc.h>
#include <Shade/shd_debug.h>
#include <Shade/shd_memory.h>
#include <Shade/shd_pad.h>
#include <Shade/std/shd_cstring.h>
#include <Shade/std/shd_stdarg.h>
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/ut/ut_color.h"

#define LINE 112

// Dates:
// JP: Jun  1 2011
// 2012: Oct  5 2011
// EU: Jul 10 2012
// 2013: Oct 29 2012

nw4r::g3d::Camera::PostureInfo g_info;

static float g_vUp[3] = {
    0.0f, 1.0f, 0.0f};

void shdInit1(void)
{
    char buf[64];
    int date[3];

    shdwk.flags |= 0x2000e;
    if (!(shd_init_flag & DISABLE_DEBUG))
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

    shdwk.xfb_width = 640;
    shdwk.efb_width = 640;    
    shdwk.xfb_height = 480;
    shdwk.efb_height = 480;    

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

    shdwk.cameraAt[1] = 0.0f;
    shdwk.cameraAt[0] = 0.0f;
    shdwk.cameraAt[2] = 2.0f;
    shdwk.cameraPos[0] = 0.0f;
    shdwk.cameraPos[2] = 4.0f;
    shdwk.cameraPos[1] = 4.0f;
    shdwk.cameraTarget[2] = 0.0f;
    shdwk.cameraTarget[1] = 0.0f;
    shdwk.cameraTarget[0] = 0.0f;
    shdwk.cameraFov = 25;
    shdwk.cameraAngR = 0.0f;
    shdwk.cameraAngL = 1.0f;
    shdwk.cameraNear = 0.1f;
    shdwk.cameraFar = 1280.0f;
    shdwk.cameraUnk = 32.0f;
    shdCamCalAng();
    //
    g_info.tp = 0;
    g_info.cameraUp = g_vUp;
    g_info.cameraTarget = nw4r::math::VEC3(0, 0, 0);
    //
    shdwk.field_0xc0 = 0x404040; // probably another GXColor struct?
    shdwk.field_0xbc = 0xffffff;
    shdwk.field_0xb0[0] = 1.0f;
    shdwk.field_0xb0[1] = 0.5f;
    shdwk.field_0xb0[2] = -2.0f;
    shdCalNormal(shdwk.field_0xb0);
    shdwk.field_0x90 = 0x606060;
    shdwk.fog = 0x606060;
    shdwk.fogStart = 8.0f;
    shdwk.fogEnd = 64.0f;
    shdwk.field_0xa0 = 1;
    shdwk.field_0xa8 = 0xff000000;
    shdwk.field_0xac = 0xff000000;
    shdsys.shadow_scale = 0.2f;
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

void shdLoop1()
{
    shdwk.exec_frame++;
    shdwk.field_0x2c += shdwk.frame_cnted;
    if (shdwk.flags & 2)
    {
        // Note: i believe this is inaccurate, the wii outputs 59.94fps, not 60
        int framerate = (60000 / ((shdwk.frame_cnted * 100) / shdwk.field_0x1c6a));
        bprintf("v:%2d(%d.%dfps:%d), %d, %d\n", shdwk.frame_cnted, // vertical sync(32 most of the time)
                framerate / 10, framerate % 10,
                (int)(shdwk.frame_rate * 1000),                    // i guess this is the amount of intervals?(1(000) for 60 fps, 2(000) for 30fps, 3(000) for 20fps, whatever)
                shdwk.exec_frame,                                  // execution frame
                shdwk.field_0x2c / (shdwk.field_0x1c6a * 60));     // seconds passed
    }

    shdsys.loop1StartTime = OSGetTime();
    shdPadInput();
    if (shdwk.flags4 == 0 && shdwk.flags3 == 0)
    {
        appLoop1();
        appLoop2();
    }
    shdsys.loop1EndTime = OSGetTime();
}

void shdLoop3()
{

}

void shdMchgCleanup()
{
    shdSetSprtMend();
    shdMcbOvrCntClr();
    
    /*shdwk._318_1_ = 0;
    shdwk._1198_1_ = 0;
    shdwk._2078_1_ = 0;
    shdwk._2958_1_ = 0;*/

    int i = 0;
    do
    {
        shdVceStop(i);
        i = i + 1;
    } while (i < 4);
    
    shdEffPause(false);
    shdEffDisp(true);
    shdEffDisp2(true);
}

void wiiResetGP()
{
    GXSetLineWidth(6, 0);
    GXSetPointSize(6, 0);

    GXEnableTexOffsets(GX_TEXCOORD0, FALSE, FALSE);
    GXEnableTexOffsets(GX_TEXCOORD1, FALSE, FALSE);
    GXEnableTexOffsets(GX_TEXCOORD2, FALSE, FALSE);
    GXEnableTexOffsets(GX_TEXCOORD3, FALSE, FALSE);
    GXEnableTexOffsets(GX_TEXCOORD4, FALSE, FALSE);
    GXEnableTexOffsets(GX_TEXCOORD5, FALSE, FALSE);
    GXEnableTexOffsets(GX_TEXCOORD6, FALSE, FALSE);
    GXEnableTexOffsets(GX_TEXCOORD7, FALSE, FALSE);

    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetCoPlanar(FALSE);

    nw4r::ut::Color fog; 
    GXSetFog(GX_FOG_NONE, fog, 0, 0, 0, 0);
    GXSetFogRangeAdj(FALSE, 0, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetColorUpdate(TRUE);
    GXSetAlphaUpdate(FALSE);
    GXSetZMode(TRUE, GX_LEQUAL, TRUE);
    GXSetZCompLoc(TRUE);
    GXSetDither(TRUE);
    GXSetDstAlpha(FALSE, 255);
    GXSetNumTevStages(1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE4, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE5, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE6, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE7, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE8, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE9, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE10, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE11, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE12, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE13, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE14, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE15, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);

    GXSetZTexture(0, 17, 0);
    GXSetNumIndStages(0);

    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    GXSetTevDirect(GX_TEVSTAGE4);
    GXSetTevDirect(GX_TEVSTAGE5);
    GXSetTevDirect(GX_TEVSTAGE6);
    GXSetTevDirect(GX_TEVSTAGE7);
    GXSetTevDirect(GX_TEVSTAGE8);
    GXSetTevDirect(GX_TEVSTAGE9);
    GXSetTevDirect(GX_TEVSTAGE10);
    GXSetTevDirect(GX_TEVSTAGE11);
    GXSetTevDirect(GX_TEVSTAGE12);
    GXSetTevDirect(GX_TEVSTAGE13);
    GXSetTevDirect(GX_TEVSTAGE14);
    GXSetTevDirect(GX_TEVSTAGE15);

    GXSetNumChans(0);
    GXSetNumTexGens(0);
}

void wiiSetWideMode(int mode) {
    if(mode != 0) {
        shdwk.isWideMode = 1;
        shdwk.aspect_ratio = 16.f / 9.f;
        shdwk.xfb_width = 852; // shouldn't this be 854? 480 * (16/9) = 853.33 which is closer to 854...
    }
    else {
        shdwk.isWideMode = 0;
        shdwk.aspect_ratio = (f32)shdwk.efb_width / (f32)shdwk.efb_height;
    }
}

void shdCalCamMat()
{

}

void shdCnvScreen(float *, const float *)
{

}

void render_ot(int, int)
{

}

void draw_ratebar()
{

}

u32 *entry_ottag(int, u32, int)
{

}

int shdZtoOT(float)
{

}

void wiiErrDisp()
{

}

// fabricated
inline void wiiSetRenderFlag(s8 cmp)
{
    shdsys.field_0x18e[1] = cmp;
    if (shdsys.field_0x18e[1] > 0)
    {
        if (shdsys.field_0x18e[2] > 0)
        {
            GXSetZMode(TRUE, GX_LEQUAL, TRUE);
        }
        else
        {
            GXSetZMode(TRUE, GX_LEQUAL, FALSE);
        }
    }
    else
    {
        GXSetZMode(FALSE, GX_NEVER, FALSE);
    }
}

void wiiSetStateZcmp(s8 cmp)
{
    if (shdsys.field_0x18e[1] != cmp)
    {
        wiiSetRenderFlag(cmp);
    }
}

void wiiSetStatePjMtx(s8)
{
}

void wiiSetStateTex(s16)
{

}

void wiiSetStateTev(s8)
{
}

void wiiSetStateSemi(s8)
{
}

void wiiSetStateVtxFmt(s16)
{
}

void wiiSetStateLighting(s8)
{
}

void wiiSetStateFog(s8)
{
}

void wiiSetStateCull(s8)
{
}

void wiiSetStateClip(s8)
{
}

void shdResetRenderFlag()
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
    wiiSetRenderFlag(1);
    shdsys.field_0x200 = 0;
}
