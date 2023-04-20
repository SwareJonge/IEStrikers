#ifndef MSL_STRTOUL_H
#define MSL_STRTOUL_H

unsigned long __strtoul(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, int* chars_scanned, int* negative, int* overflow);
unsigned long long __strtoull(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, int* chars_scanned, int* negative, int* overflow);
long strtol(const char *str, char **end, int base);
int atoi(const char *str);

#endif