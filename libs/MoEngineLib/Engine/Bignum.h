#pragma once

#ifdef __cplusplus
class Bignum
{
public:
    Bignum();
    ~Bignum();

    int GetBits() const;
    int GetBytes() const;

    int ToBin(unsigned char *dump) const;
    void FromBin(const unsigned char *dump, int nBytes);
    void Prepare(int count);

    void Swap(Bignum &alt);
    void Copy(const Bignum &b);

    void Clamp();
    void And(int nw);
    void ShiftRight(int nw);

    void Exp3Mod(const Bignum &m, const Bignum &mu);
    void Reduce(const Bignum &m, const Bignum &mu, Bignum *stack);
    int CmpAbsolute(const Bignum &b) const;

    void SetSub(const Bignum &ra, const Bignum &rb);
    void SetAddAbsolute(const Bignum &ra, const Bignum &rb);
    void SetSubAbsolute(const Bignum &a, const Bignum &b);
    void SetMul(const Bignum &a, const Bignum &b);

private:
    unsigned int *Digits;
    int Count;
    int Capacity;
    int Neg; // perhaps C-Style bool
};

extern "C"
{
#endif
int DigitsAdd(unsigned int *r, int n, unsigned int *a, unsigned int *b);
int DigitsSub(unsigned int *r, int n, unsigned int *a, unsigned int *b);
unsigned int DigitsMul(unsigned int *r, int n, unsigned int *a, unsigned int b);
unsigned int DigitsMulAdd(unsigned int *r, int n, unsigned int *a, unsigned int b);
#ifdef __cplusplus
};
#endif

void *DigitsAlloc(int size);
void DigitsFree(void *p);

void RsaPublicDecrypt(void *buf, int size, const void *binM, const void *binMu);
