#ifndef MSL_SIGNAL_H
#define MSL_SIGNAL_H

#include "types.h"

extern void exit(int);

typedef void (*sig_func)(int sig);

int raise(int sig);

#endif
