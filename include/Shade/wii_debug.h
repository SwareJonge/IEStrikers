#pragma once

void cprintf(char *, ...); // console printf
void sys_err_prt(char *, ...);

char *get_sys_err_txt();

//#ifdef MATCHING
#define ASSERT(COND, line) sys_err_prt("FAILED " __FILE__  ":%d Lines", line);
/*#else
#define ASSERT(COND) \
    if (!COND)       \
        sys_err_prt("FAILED " __FILE__ + ":%d Lines", __LINE__);
#endif*/