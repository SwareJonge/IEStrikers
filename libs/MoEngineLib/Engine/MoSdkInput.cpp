#include "MoSdkInput.h"
#include "MoSdkMem.h"

void *MoSdkInput::operator new(size_t size) { return MOiAlloc(size); }
void MoSdkInput::operator delete(void *mem_p) { MOiFree(mem_p); }

MoSdkInput::MoSdkInput(void *inst, u32 size, MO_MEDIUM_CONTROL* ctl, void *cook)
{
    access = ctl;
    bufferSize = size;
    buffer = NULL;
    indexWrite = 0;
    indexRead = 0;
    bufferBase = 0;
    cookie = cook;
    instance = inst;
}

MoSdkInput::~MoSdkInput()
{
    if (buffer)
        MOiFree(buffer);

    buffer = NULL;
}

bool MoSdkInput::allocInputBuffer()
{
    buffer = (u8*)MOiAlloc(bufferSize);

    if(buffer == NULL)
        return false;

    return true;
}

u32 MoSdkInput::read32()
{
#line 70
    // clang-format off
    MO_ASSERT(this, indexWrite-indexRead>=4);
    // clang-format on
    u32 val;

    u8 *b = buffer + indexRead;
    indexRead += 4;
    val = b[0] + (b[1] << 8) + (b[2] << 16) + (b[3] << 24);
    return val;
}

u32 MoSdkInput::read16()
{
#line 82
    // clang-format off
    MO_ASSERT(this, indexWrite-indexRead>=2);
    // clang-format on
    u32 val;

    u8 *b = buffer + indexRead;
    indexRead += 2;
    val = b[0] + (b[1] << 8);
    return val;
}

u32 MoSdkInput::read8()
{
#line 94
    // clang-format off
    MO_ASSERT(this, indexWrite-indexRead>=1);
    // clang-format on
    u32 val;

    u8 *b = buffer + indexRead;
    indexRead += 1;
    val = b[0];
    return val;
}

void MoSdkInput::skipBytes(u32 len)
{
#line 106
    // clang-format off
    MO_ASSERT(this, indexWrite-indexRead>=len);
    // clang-format on
    indexRead += len;
}

u32 MoSdkInput::read(u8 *bffer, u32 len)
{
#line 114
    // clang-format off
    MO_ASSERT(this, indexWrite-indexRead>=len);
    // clang-format on
    u8 *b = buffer + indexRead;
    indexRead += len;
    memcpy(bffer, b, len);
    return len;
}

u32 MoSdkInput::availableBytes() { return (indexWrite-indexRead); }

bool MoSdkInput::rescaleBuffer(u32 len)
{
    if (len < bufferSize)
        return true;

    u8 *b = (u8 *)MOiAlloc(len);
    memcpy(b, buffer, bufferSize);
    MOiFree(buffer);

    buffer = b;
    bufferSize = len;

    return true;
}

bool MoSdkInput::needBytes(u32 size)
{
    if (availableBytes() >= size)
        return true;

    if (indexRead > (bufferSize >> 1))
    {
        memmove(buffer, buffer + indexRead, indexWrite - indexRead);
        indexWrite -= indexRead;
        indexRead = 0;
        bufferBase += indexRead;
    }

    u32 avail = bufferSize - indexWrite;
    u32 toread = size;
    if (avail < toread)
        toread = avail;

    int inc = access->Read(instance, cookie, buffer + indexWrite, toread);

    if (inc > 0)
        indexWrite += inc;

    if (availableBytes() >= size)
        return true;

    return false;
}

bool MoSdkInput::jumpTo(u32 position)
{
    indexWrite = 0;
    indexRead = 0;
    bufferBase = position;
    return true;
}