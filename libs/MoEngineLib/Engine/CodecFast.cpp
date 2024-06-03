#include "FA_Codec.h"

static s16 Apcm[] {

};

typedef short FA_BUF[3][21]; // idk what to call this

static void DecodeStream(const s8 *in, s16 *lar, s16 *p3, s16 *p4, FA_BUF out);

void Codec::UnpackFast(const s8 *in, short *out)
{

}

static void DecodeStream(const s8 *in, s16 *lar, s16 *p3, s16 *p4, FA_BUF out)
{
    
}