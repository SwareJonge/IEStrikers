#pragma once

#include <revolution/OS/OSCache.h>

static void Tile(u8 *dest, u8 *src, int w, int h, int stride)
{
    u32 *out0 = (u32 *)dest;
    u32 *out;
    u32 *a, *b, *c, *d;
    u32 inc = w / 4;
    a = (u32 *)(src);
    for (int y = h >> 2; y > 0; y--)
    {
        b = a + (inc);
        c = b + (inc);
        d = c + (inc);
        out = out0;
        out0 += stride;
        for (int x = w >> 4; x > 0; x--) // is this loop optimized by cw?
        {
            out[0] = a[0];
            out[1] = a[1];
            out[2] = b[0];
            out[3] = b[1];
            out[4] = c[0];
            out[5] = c[1];
            out[6] = d[0];
            out[7] = d[1];
            out += 8;
            out[0] = a[2];
            out[1] = a[3];
            out[2] = b[2];
            out[3] = b[3];
            out[4] = c[2];
            out[5] = c[3];
            out[6] = d[2];
            out[7] = d[3];

            a += 4;
            b += 4;
            c += 4;
            d += 4;
            out += 8;
        }
        a = d;
        out = out0;
    }
    DCFlushRange(dest, h * stride);
}

static void Tile(u8 *dest, const u8 *src, int w, int h, int stride)
{
    u32 *out0 = (u32 *)dest;
    u32 *out;
    u32 *a, *b, *c, *d;
    u32 inc = w / 4;
    a = (u32 *)(src);
    for (int y = h >> 2; y > 0; y--)
    {
        b = a + (inc);
        c = b + (inc);
        d = c + (inc);
        out = out0;
        out0 += stride;
        for (int x = w >> 4; x > 0; x--) // is this loop optimized by cw?
        {
            out[0] = a[0];
            out[1] = a[1];
            out[2] = b[0];
            out[3] = b[1];
            out[4] = c[0];
            out[5] = c[1];
            out[6] = d[0];
            out[7] = d[1];
            out += 8;
            out[0] = a[2];
            out[1] = a[3];
            out[2] = b[2];
            out[3] = b[3];
            out[4] = c[2];
            out[5] = c[3];
            out[6] = d[2];
            out[7] = d[3];

            a += 4;
            b += 4;
            c += 4;
            d += 4;
            out += 8;
        }
        a = d;
        out = out0;
    }
    DCFlushRange(dest, h * stride);
}