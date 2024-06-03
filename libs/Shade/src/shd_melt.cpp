#include <revolution/types.h>
#include <printf.h>
#include <Shade/wii_debug.h>

u8 *shdUnpackSrc;
u8 *shdUnpackDst;
u32 shdUnpackSize;

u32 shdUnpackGetSize(u8 *src)
{
    if (src[0] == 0xfc && src[1] == 0xaa && src[2] == 0x55 && src[3] == 0xa7)
        return src[4] | (src[5] << 8) | (src[6] << 16) | (src[7] << 24);

    char buf[256];
    sprintf(buf, "shdUnpackGetSize() : no header");
    sys_err_prt("FAILED shd_melt.cpp:%d Lines\n%s", 50, buf); // probably a macro
    return 0;
}

void shdUnpack(u8 *src, u8 *dst)
{
    u8 *dstStart = dst;
    if (src[0] == 0xfc && src[1] == 0xaa && src[2] == 0x55 && src[3] == 0xa7)
    {
        src += 0xc;
    }
    
    while (true)
    {
        u8 code;
        int endpos;
        int offset2;
        int offset;
        u8 *unpackdst;

        code = *src++;

        if (code == 0)
            break;

        if (code & 0x80)
        {
            offset = (((code & 0x1f) << 8) | *src++);
            unpackdst = dst - offset;

            for (endpos = (((code >> 5) & 3) + 4); endpos > 0; endpos--)
                *dst++ = *unpackdst++;

#ifndef ACTUAL_CODE
            goto silly;
            while (true)
#else
            while ((*src & 0xE0) == 0x60)
#endif
            {
                for (endpos = (((*src++)) & 0x1f); endpos > 0; --endpos)
                    *dst++ = *unpackdst++;
#ifndef ACTUAL_CODE
            silly:
                if ((*src & 0xE0) != 0x60)
                    break;
#endif
            }
        }
        else if (code & 0x40)
        {
            if (code & 0x10)
            {
                endpos = (((code << 8) & 0xF00) | *src++);
                endpos += 4;
            }
            else
            {
                endpos = (code & 0xf) + 4;
            }

            code = *src++;

            for (; endpos > 0; endpos--)
                *dst++ = code;
        }
        else
        {
            if (code & 0x20)
                endpos = ((code & 0x1F) << 8) | *src++;
            else
                endpos = code & 0x1f;

            for (; endpos > 0; endpos--)
                *dst++ = *src++;
        }
    }

    shdUnpackSrc = src;
    shdUnpackDst = dst;
    shdUnpackSize = dst - dstStart;
}