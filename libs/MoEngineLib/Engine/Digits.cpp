#include "Bignum.h"

int DigitsAdd(unsigned int *r, int n, unsigned int *a, unsigned int *b)
{
    unsigned int carry = 0;

    while (n--)
    {
        unsigned int w1 = *a++;
        unsigned int w2 = *b++;
        w1 += carry;
        carry = (carry > w1);
        w1 += w2;
        carry += (w2 > w1);
        *r++ = w1;
    }

    return carry;
}

int DigitsSub(unsigned int *r, int n, unsigned int *a, unsigned int *b)
{
    int carry = 0;

    while (n--)
    {
        unsigned int w1 = *a++;
        unsigned int w2 = *b++;

        if (carry != 0)
            carry = (w2 >= w1--);
        else
            carry = (w2 > w1);

        w1 -= w2;
        *r++ = w1;
    }

    return carry;
}

unsigned int DigitsMul(unsigned int *r, int n, unsigned int *a, unsigned int b)
{
    unsigned int carry = 0;

    unsigned int bl = b & 0xffff;
    unsigned int bh = b >> 16;
    while (n--)
    {
        unsigned int h, l, lt, ht, m, m1;

        h = *a;
        l = h & 0xffff;
        h = h >> 16;
        lt = l;
        ht = h;

        m = bh * lt;
        lt = bl * lt;

        m1 = bl * ht;
        ht = bh * ht;

        m += m1;

        if (m < m1)
            ht += 0x10000;

        ht += m >> 16;
        m1 = m << 16;
        lt += m1;

        if (lt < m1)
            ht++;

        l = lt;
        h = ht;
        l += carry;

        if (l < carry)
            h++;
        
        carry = h;

        *r = l;
        r++;
        a++;
    }
    return carry;
}

unsigned int DigitsMulAdd(unsigned int *r, int n, unsigned int *a, unsigned int b)
{
    unsigned int carry = 0;

    unsigned int bl = b & 0xffff;
    unsigned int bh = b >> 16;
    while (n--)
    {
        unsigned int h, l, lt, ht, m, m1;

        h = *a;
        l = h & 0xffff;
        h = h >> 16;
        lt = l;
        ht = h;

        m = bh * lt;
        lt = bl * lt;

        m1 = bl * ht;
        ht = bh * ht;

        m += m1;
        
        if (m < m1)
            ht += 0x10000;

        ht += m >> 16;
        m1 = m << 16;
        lt += m1;

        if (lt < m1)
            ht++;

        l = lt;
        h = ht;
        l += carry;

        if (l < carry)        
            h++;        

        carry = *r;
        l += carry;

        if (l < carry)        
            h++;
        
        carry = h;

        *r = l;
        r++;
        a++;
    }
    return carry;
}