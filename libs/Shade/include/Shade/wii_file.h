#pragma once

#include <Shade/shd_file.h>
#include <stdio.h>
#include <revolution/types.h>
#include <Shade/shd_calc.h>
#include <revolution/DVD.h>

#define READ_LE32(buf, offset) \
    (__lwbrx(buf + offset, 0))

void wiiFileInit();
int wiiFileLoadSync(int async);
void wiiFileLoadBegin(int ftyp, int offset, int size, u8 *address);

extern DVDFileInfo *last_dvd_info;
extern char wiiCNTerrEmu;
extern char wiiCNTerrEmu;
extern s16 wiiErrEmuRate;

void chk_nand_err(int error) DONT_INLINE;