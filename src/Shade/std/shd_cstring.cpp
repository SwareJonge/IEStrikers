#include <Shade/std/shd_cstring.h>

void hstrcpy(char *dest, const char *src)
{
    do
    {
        *dest++ = *src;
    } while (*src++ != '\0');
}

int hstrcmp(const char *str1, const char *str2)
{
    char cur;
    do
    {
        cur = *str1;
        if (*str1++ != *str2++)
        {
            return 1;
        }
    } while (cur != '\0');
    return 0;
}

void hstrcat(char *str1, const char *str2)
{
    char cur;
    if (*str1 != '\0')
    {
        hstrcat(str1 + 1, str2);
        return;
    }
    do
    {
        cur = *str2;
        *str1++ = *str2++;
    } while (cur != '\0');
}

int hstrncmp(const char *str1, const char *str2, int n)
{
    char cur;
    for (; n > 0; n--)
    {
        cur = *str1;
        if (*str1++ != *str2++)
            return 1;
        if (cur == '\0')
            break;
    }
    return 0;
}

int hstrlen(const char *str)
{
    int n = 0;
    while (true)
    {
        if (*str++ == '\0')
            break;
        n++;
    }
    return n;
}