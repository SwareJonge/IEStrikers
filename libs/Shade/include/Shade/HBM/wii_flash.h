#pragma once

#include "types.h"

void Flash_Init();

u32 Flash_Exec();
// these are probably local
void Flash_FileChkJob();
void Flash_FileSizeJob();
void Flash_FileReadJob();
void Flash_FileWriteJob();
void Flash_CheckJob();
BOOL Flash_FileWriteOpenTask();
BOOL Flash_FileReadTask();
BOOL Flash_FileWriteTask();

void Flash_ErrorMsg(char *, s32);

BOOL Flash_IsAccess();