#pragma once

extern char **shd_ftyp_fname;
extern int shd_ftyp_max;

void shdFlushChache(void *buf, int n);

bool shdSysFileExist(const char *path);
int shdSysFileLoad(const char *path, int, void *, int);
void shdSysFileSave(const char *, void *, int);
int shdSysFileSize(const char *path);
int shdMcbCalTblSz(int entries);
