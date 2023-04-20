#ifndef MSL_FILE_IO_H
#define MSL_FILE_IO_H

#include "types.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"

int fclose(FILE* file);
int fflush(FILE* file);

int __msl_strnicmp(const char *s1, const char *s2, size_t len);

#endif
