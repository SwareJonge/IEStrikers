#include <Shade/shd_calc.h>

int jrnd;
u32 rndx[521]; // i assume this should've been 512? 521 makes no sense

/* 
shdRndInit (unsigned long) 
generates the table with random numbers
*/
void shdRndInit(u32 seed)
{
    u32 nextSeed = 0;
    for (int i = 0; i <= 16; i++)
    { // initialize first table
        for (int j = 0; j < 32; j++)
        {
            seed = seed * 0x5d588b65 + 1;
            nextSeed = (nextSeed >> 1 | seed & 0x80000000);
        }
        rndx[i] = nextSeed;
    }

    rndx[16] = (rndx[16] << 0x17 ^ rndx[0] >> 9) ^ rndx[15];

    for (int i = 17; i < 521; i++) { // initialize second table
        rndx[i] = (rndx[i - 17] << 0x17 ^ rndx[i - 16] >> 9) ^ rndx[i - 1];
    }

    // now reiterate over both tables twice
    for (int i = 0; i < 32; i++) {
        rndx[i] ^= rndx[i + 489];
    }

    for (int i = 32; i < 521; i++) {
        rndx[i] ^= rndx[i - 32];
    }

    for (int i = 0; i < 32; i++) {
        rndx[i] ^= rndx[i + 489];
    }

    for (int i = 32; i < 521; i++) {
        rndx[i] ^= rndx[i - 32];
    }

    jrnd = 520;
}

/* 
shdRnd()
returns a random integer
*/
int shdRnd()
{
    jrnd++;
    if (jrnd >= 521)
    {
        jrnd = 0;
    }
    int irnd = jrnd - 32;
    if (irnd < 0)
    {
        irnd += 521;
    }
    (rndx)[jrnd] ^= (rndx)[irnd];
    return (rndx)[jrnd];
}

/* 
shdRndi (int, int)
returns a random integer betwen 2 numbers
*/
int shdRndi(int min, int max)
{
    if (min != max)
    {
        shdRnd();
        return min + (((rndx)[jrnd]) % ((max - min) + 1));
    }
    return min;
}

/*
shdRndf (float, float)
returns a random float betwen 2 numbers
*/
f32 shdRndf(f32 min, f32 max)
{
    shdRnd();
    f32 rndf = rndx[jrnd];
    f32 range = (max - min);
    f32 u32max = 4.294967295f; // yes, this needs to be declared

    return min + (rndf * range) / u32max;
}

void shdVecCpy(f32 *vec, const f32 *other)
{
    vec[0] = other[0];
    vec[1] = other[1];
    vec[2] = other[2];
}

void shdVecAdd(f32 *vec, const f32 *a, const f32 *b)
{
    vec[0] = a[0] + b[0];
    vec[1] = a[1] + b[1];
    vec[2] = a[2] + b[2];
}

void shdVecSub(f32 *vec, const f32 *a, const f32 *b)
{
    vec[0] = a[0] - b[0];
    vec[1] = a[1] - b[1];
    vec[2] = a[2] - b[2];
}

void shdVecScMul(f32 *vec, const f32 *a, f32 scalar)
{
    vec[0] = a[0] * scalar;
    vec[1] = a[1] * scalar;
    vec[2] = a[2] * scalar;
}

f32 shdVecDot(const f32 *a, const f32 *b) { return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]); }

f32 shdVecLen(const f32 *a)
{
    f32 x = a[0];
    f32 y = a[1];
    f32 z = a[2];
    return sqrt((x * x) + (y * y) + (z * z));
}