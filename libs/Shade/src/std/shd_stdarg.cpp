#include <stdarg.h>
#include <printf.h>
#include <Shade/shd.h>
#include <Shade/shd_debug.h>
#include <Shade/std/shd_cstring.h>
#include <Shade/std/shd_stdarg.h>

// i'm not entirely sure why this file doesn't inline, it either uses different flags or a pragma was used
#pragma optimize_for_size on 

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
        *str1++ = *str2;
    } while (*str2++ != '\0');
}

int hstrncmp(const char *str1, const char *str2, int n)
{
    char cur;
    while (n-- > 0)
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

void bprintf(char *msg, ...) {
    if ((shd_init_flag & 1) == 0 && shdwk.flags & 2) {
        va_list list;
        va_start(list, msg);
        char buf[4096];
        int n = vsprintf(buf, msg, list);
        if (textbuff_top + n <= (int)sizeof(buf) / 2 - 1) { // use macro
            buf[n] = '\0';
            hstrcpy(&textbuff[textbuff_top], buf);
            textbuff_top += n;
        }
        va_end(list);
    }
    
}

void eprintf(char *msg, ...)
{
    if ((shd_init_flag & 1) == 0 && shdwk.flags & 2)
    {
        errdisp_cnt = 9600;
        va_list list;
        va_start(list, msg);
        
        vsprintf(errdisp_txt, msg, list);
        va_end(list);
    }
    
}