#pragma once

#include <revolution/types.h>

#include "Common/VX2_Cpu.h"
#include "Common/VX2_MotionVector.h"
#include "Common/VX2_Pred.h"

#include "VX2_BitStream.h"
#include "VX2_InvMc.h"
#include "VX2_DecoderMeFct.h"

void *VX2_Malloc(unsigned int size);
void VX2_Free(void *mem_p);

struct VX2DecoderBlock; // forward declared and used in one of the VX2 source files

struct VX2DecoderData {
    int Width;
    int Height;
    u8 *Buffer[6];
    u8 *Y[6];
    u8 *U[6];
    u8 *V[6];
    u8 *ImgY[6];
    u8 *ImgU[6];
    u8 *ImgV[6];
    VX2DecoderBlock *Block;
    int SliceNbBloc[256];
    int SliceSize[256];
    int PosY;
    int PosUV;
    int Index;
    int InvQ8x8Zigzag[64];
    int InvQ4x4Zigzag[16];
    int LatestQuantizer;
    VX2BitStream BitStream;
    BOOL IsIntraFrame;
    int Yv12Mode;
    VX2MotionVector *MotionVectorPredictors;
    VX2MotionVector OutputMotionVector;
    VX2MotionVector BlockMotionVector;
    int IntraPred[25];
    int AvailableFlag;
    u16 *InvVlc[2];
    u8 *MaxLevel[2];
    u8 *MaxRun[2];
    MeFct Me;
    PredFunction Pred;
    InvMcFunction InvMc;
    CpuFunction Cpu;
    unsigned int CpuId;
};

bool VX2DecoderInit(VX2DecoderData &data, int w, int h);
u8 *VX2DecoderUnpack(VX2DecoderData &data, const void *src); // investigate return type
u32 VX2DecoderGetQuantizer(VX2DecoderData &data);
void VX2DecoderRelease(VX2DecoderData &data);

