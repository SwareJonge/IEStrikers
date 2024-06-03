#pragma once

#include <revolution/PAD.h>

void wiiConInit();
void wiiConRead(int idx);
void gcPadRead();
//void wiiMakePadDataGC(int idx, PADStatus *padStatus);
void wiiVibAllStop();
void ramble(int idx); // typo
int wiiConGetStat(int idx);

u32 wiiConInput(int idx);
int wiiPadControlDpd(int, s8);
