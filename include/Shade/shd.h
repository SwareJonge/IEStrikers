#pragma once

extern int shd_init_flag;

struct shade_work {
    int flags;
    unsigned char _4[0x1C7C - 4];
}; // Size: 0x1C7C

extern shade_work shdwk;