#pragma once

void shdDebDateCnv(char *, int *);

void shdBprintOut();

// might belong to wii_debug?
extern int textbuff_top;
extern short errdisp_cnt;
extern char textbuff[2048];
extern char errdisp_txt[2048];