#pragma once

#include <revolution/types.h>

struct MoPublisherCertificate
{
    u8 FirstByte[1];
    u8 PublicKey[64];
    u8 PublicBarrett[65];
    s16 Permit;
    int Flags;
    unsigned int TimeStart;
    unsigned int TimeEnd;
    s8 Zones[7];
    s8 ReservedForKeyClone;
    unsigned int PcId;
    unsigned int Serial;
};