#ifndef MSL_FILE_IO_H
#define MSL_FILE_IO_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct FILE {
    u8 dummy;
} FILE;

FILE *fopen(const char *filename, const char *mode);
int fclose(FILE *);
int fflush(FILE*);
int __msl_strnicmp(const char *s1, const char *s2, size_t len);

#ifdef __cplusplus
}
#endif
#endif
