#pragma once

#include <revolution/types.h>

struct CpuFunction
{
    unsigned int (*GetCaps)(void);
    void (*ClearState)(void);
};