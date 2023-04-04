#include <stl/stdarg.h>
#include <stl/internal/printf.h>
#include <Shade/shd.h>
#include <Shade/std/shd_cstring.h>
#include <Shade/std/shd_stdarg.h>

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