#include <revolution/NAND.h>
#include <revolution/OS.h>

#define BOOTINFO_PATH "/shared2/sys/NANDBOOTINFO"

static s32 CheckSum(s32* array) {
    s32* var_r8;
    s32 total;
    s32 i;
    var_r8 = array + 1;
    total = 0;

    for (i = 0; i < 0x407; i++) {
        total += *var_r8;
        var_r8++;
    };

    return total;
}

BOOL __OSCreateNandbootInfo(void) {
    NANDStatus status;
    NANDResult result = NANDPrivateGetStatus(BOOTINFO_PATH, &status);

    if (result == NAND_RESULT_OK && status.perm == 0x3F) {
        return TRUE;
    }

    if (result == NAND_RESULT_OK && status.perm != 0x3F) {
        if (NANDPrivateDelete(BOOTINFO_PATH)) {
            return FALSE;
        }
    } else if (result != NAND_RESULT_NOEXISTS) {
        return FALSE;
    }

    if (NANDPrivateCreate(BOOTINFO_PATH, 0x3F, 0)) {
        return FALSE;
    }

    return TRUE;
}

BOOL __OSWriteNandbootInfo(s32* arg0) {
    NANDFileInfo fileInfo;

    s32 checksum = CheckSum(arg0);
    *arg0 = checksum;

    if (NANDPrivateOpen(BOOTINFO_PATH, &fileInfo, 2U) == NAND_RESULT_OK) {
        if (NANDWrite(&fileInfo, arg0, 0x1020U) != 0x1020U) {
            NANDClose(&fileInfo);
            return FALSE;
        }
        return NANDClose(&fileInfo) == NAND_RESULT_OK;
    }
    return FALSE;
}