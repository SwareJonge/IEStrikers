#include "MoSdkMem.h"
#include "Bignum.h"
#include <revolution/types.h>

Bignum::Bignum()
{
    Digits = NULL;
    Count = Capacity = 0;
    Neg = 0;
}

Bignum::~Bignum()
{
    DigitsFree(Digits);
}

static int BitsUsed(unsigned int w)
{
    static const char bits[256] = {
        0x00, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03,
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
        0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};

    if (w >= 65536)
    {
        if (w >= 16777216)
        {
            return bits[w >> 24] + 24;
        }
        return bits[w >> 16] + 16;
    }
    if (w >= 256)
    {
        return bits[w >> 8] + 8;
    }

    return bits[w];
}

int Bignum::GetBits() const
{
    if (Count == 0)
        return 0;

    unsigned int w = Digits[Count - 1];
    int n = BitsUsed(w);
    return n + (Count - 1) * 32;
}

int Bignum::GetBytes() const
{
    return (GetBits() + 7) / 8;
}

int Bignum::ToBin(u8 *dump) const
{

    int n = GetBytes();
    int i = n;
    while (i-- > 0)
    {
        unsigned int w = Digits[(i / 4)];
        *dump++ = w >> ((i & 3) * 8);
    }

    return n;
}

void Bignum::FromBin(const u8 *dump, int nBytes)
{
    int odd = (((nBytes + 3) / 4));
    Prepare(odd);

    int nDigits = (nBytes - 1) & 3;
    unsigned int w = 0;
    while (nBytes--)
    {
        w = w << 8 | *dump++;

        if (nDigits-- == 0)
        {
            Digits[--odd] = w;
            nDigits = 3;
        }
    }
    Clamp();
}

void Bignum::Prepare(int count)
{
    if (count > Capacity)
    {
        DigitsFree(Digits);
        Capacity = count + 2;
        Digits = (unsigned int *)DigitsAlloc(Capacity * 4);
    }
    Count = count;
}

void Bignum::Swap(Bignum &alt)
{
    Bignum swap(alt);
    alt = *this;
    *this = swap;
    swap.Digits = NULL; // set Digits to null to avoid clear
}

void Bignum::Copy(const Bignum &b)
{
    Prepare(b.Count);
    int i = Count;
    while (i--)
    {
        Digits[i] = b.Digits[i];
    }
}

void Bignum::Clamp()
{
    unsigned int *top = Digits + (Count - 1);
    while (Count > 0)
    {
        if (*top-- != 0) {
            break;
        }
        Count--;
    }
}

void Bignum::And(int nw)
{
    while (Count > nw)
        Digits[--Count] = 0;
}

void Bignum::ShiftRight(int nw)
{
    Count -= nw;
    unsigned int *to = Digits;
    unsigned int *from = to + nw;
    int c = Count;

    while (c--)
        *to++ = *from++;
}

void Bignum::Exp3Mod(const Bignum &m, const Bignum &mu)
{
    Bignum *x = this;
    Bignum stack[2];

    Bignum x2;
    x2.SetMul(*x, *x);
    x2.Reduce(m, mu, stack);

    Bignum x3;
    x3.SetMul(x2, *x);
    x3.Reduce(m, mu, stack);
    Swap(x3);
}

void Bignum::Reduce(const Bignum &m, const Bignum &mu, Bignum *stack)
{
    int nw = m.Count;
    
    Bignum *q1 = stack++;
    Bignum *q2 = stack++;

    q1->Copy(*this);
    q1->ShiftRight(nw - 1);

    q2->SetMul(*q1, mu);
    q2->ShiftRight(nw + 1);

    q1->SetMul(*q2, m);
    q1->And(nw + 1);

    And(nw + 1);
    Swap(*q2);
    SetSub(*q2, *q1);

    int uhoh;
    if (Neg == 0)
    {
        uhoh = 4;
        while (CmpAbsolute(m) >= 0)
        {
            if (uhoh-- == 0)
                break;

            q1->SetSubAbsolute(*this, m);
            Swap(*q1);
        }
    }
}

int Bignum::CmpAbsolute(Bignum const &b) const
{
    int i = Count - b.Count;
    if (i != 0)
    {
        return i;
    }
    else
    {
        i = Count;
        while (--i >= 0)
        {
            unsigned int w1 = Digits[i];
            unsigned int w2 = b.Digits[i];
            if (w1 != w2)
                return w1 > w2 ? 1 : -1;
        }
    }
    return 0;
}

void Bignum::SetSub(const Bignum &ra, const Bignum &rb)
{
    const Bignum *a = &ra;
    const Bignum *b = &rb;

    if (a->Neg)
    {
        if (b->Neg)
        {
            const Bignum *swap = a;
            a = b;
            b = swap;
        }
        else
        {
            SetAddAbsolute(*a, *b);
            Neg = 1;
            return;
        }
    }
    else
    {
        if (b->Neg)
        {
            SetAddAbsolute(*a, *b);
            Neg = 0;
            return;
        }
    }

    int order = a->CmpAbsolute(*b);
    if (order < 0)
    {
        SetSubAbsolute(*b, *a);
        Neg = 1;
    }
    else
    {
        SetSubAbsolute(*a, *b);
        Neg = 0;
    }
}

void Bignum::SetAddAbsolute(const Bignum &ra, const Bignum &rb)
{
    const Bignum *a = &ra;
    const Bignum *b = &rb;
    if (a->Count < b->Count)
    {
        const Bignum *swap = a;
        a = b;
        b = swap;
    }

    int hiCount = a->Count;
    int loCount = b->Count;

    Prepare(hiCount + 1);
    Count = hiCount;

    unsigned int *wr = Digits;
    unsigned int *wa = a->Digits;
    unsigned int *wb = b->Digits;

    int carry = DigitsAdd(wr, loCount, wa, wb);

    wr += loCount;
    wa += loCount;

    if (carry)
    {
        while (true)
        {
            if (loCount == hiCount)
            {
                *wr++ = 1;
                Count++;
                break;
            }
            loCount++;
            unsigned int w = *wa++;
            w++;
            *wr++ = w;
            if (w != 0)
                break;
        }
    }

    while (loCount++ < hiCount)
        *wr++ = *wa++;
}

void Bignum::SetSubAbsolute(const Bignum &a, const Bignum &b)
{
    int hiCount = a.Count;
    int loCount = b.Count;
    Prepare(hiCount);

    Count = hiCount;

    unsigned int *wr = Digits;
    unsigned int *wa = a.Digits;
    unsigned int *wb = b.Digits;

    int carry = DigitsSub(wr, loCount, wa, wb);

    wr += loCount;
    wa += loCount;

    if (carry)
    {
        while (true)
        {
            unsigned int w;
            unsigned int stop;
            loCount++;
            w = *wa++;
            stop = w, w--; // dumb match
            *wr++ = w;

            if (stop)
                break;
        }
    }

    while (loCount++ < hiCount)
        *wr++ = *wa++;

    Clamp();
}

void Bignum::SetMul(const Bignum &a, const Bignum &b)
{
    Neg = a.Neg ^ b.Neg;
    int na = a.Count;
    int nb = b.Count;
    Prepare(na + nb);

    unsigned int *wa = a.Digits;
    unsigned int *wb = b.Digits;

    if (na < nb)
    {
        int nswap;
        unsigned int *wswap;

        nswap = na;
        na = nb;
        nb = nswap;

        wswap = wa;
        wa = wb;
        wb = wswap;
    }
    unsigned int *wr = Digits;
    unsigned int *wrn = wr + na;

    *wrn = DigitsMul(wr, na, wa, *wb);

    while (--nb > 0)
    {
        wrn++;
        wr++;
        wb++;
        *wrn = DigitsMulAdd(wr, na, wa, *wb);
    }
    Clamp();
}

void RsaPublicDecrypt(void *buf, int size, const void *binM, const void *binMu)
{
    Bignum x, m, mu;

    x.FromBin((u8 *)buf, size);
    m.FromBin((u8 *)binM, size);
    mu.FromBin((u8 *)binMu, size + 1);

    x.Exp3Mod(m, mu);
    x.ToBin((u8 *)buf);
}

void *DigitsAlloc(int size)
{
    return MOiAlloc(size);
}

void DigitsFree(void *p)
{
    if (p)
        MOiFree(p);
}
