#pragma once

void bprintf(char *, ...); // buffer printf
void eprintf(char *, ...); // error printf

// these probably belong to a different file
extern char errdisp_txt[2048];
extern char textbuff[2048];
extern int textbuff_top; 
extern short errdisp_cnt;