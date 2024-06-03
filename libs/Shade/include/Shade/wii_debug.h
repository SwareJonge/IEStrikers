#pragma once

#include <Shade/shd_debug.h>

void cprintf(char *, ...); // console printf
void sys_err_prt(char *, ...);

const char *get_sys_err_txt();

// TODO: get rid of this(by using #line)

#ifdef MATCHING
#define ASSERT(COND, line) \
    if (!COND)             \
        sys_err_prt("FAILED " __FILE__ ":%d Lines", line);
#else
#define ASSERT(COND) \
    if (!COND)       \
        sys_err_prt("FAILED " __FILE__ + ":%d Lines", __LINE__);
#endif