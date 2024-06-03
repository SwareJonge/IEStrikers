#include <Shade/shd.h>
#include <Shade/wii.h>
#include <Shade/wii_file.h>
#include <Shade/wii_debug.h>
#include <Shade/shd_memory.h>
#include <Shade/std/shd_cstring.h>
#include <revolution/OS/OSCache.h>

struct shade_dvd_info : DVDFileInfo // was it really a custom struct? or did DVDFileInfo get changed
{
    int go, away; 
};

static shade_dvd_info g_fileInfo;
static int entrynum[16];

static char readf;
static int readsize;
DVDFileInfo *last_dvd_info;
char wiiCNTerrEmu;
s16 wiiErrEmuRate = 1;

static void dvd_callback(s32, DVDFileInfo *info);

void wiiFileInit()
{
    char path[256];
    for (int i = 0; i < shd_ftyp_max; i++)
    {
        hstrcpy(path, "/");
        hstrcat(path, shd_ftyp_fname[i]);

        entrynum[i] = DVDConvertPathToEntrynum(path);
        chk_nand_err(entrynum[i]);
        if (entrynum[i] < 0)
            sys_err_prt("File not found : %s\n", path);
    }
}

CW_FORCE_STRINGS(wii_file_1, "File Open Error : %s\n",
                 "SysFile Read Error : %s(0x%X)\n")

void shdFlushChache(void *buf, int n) { DCFlushRange(buf, n); }

int shdSysFileLoad(const char *path, int offset, void *dst, int size)
{
    DVDFileInfo info;

    int unk = 0;

    if (DVDOpen(path, &info) != 0)
    {
        if (size == 0)
            size = info.size;

        for (int readSize; unk < size;)
        {
            readSize = unk + 0x10000 <= size ? 0x10000 : size - unk;

            DVDReadAsyncPrio(&info, dst, readSize, offset, dvd_callback, 2);

            int intervals = wiiwk.intervals; // no idea what this should've been

            do
            {
            } while (DVDGetCommandBlockStatus(&info.block) != 0);

            last_dvd_info = NULL;
            unk += readSize;
            offset += readSize;
            dst = (u8 *)dst + readSize;
        }

        DVDClose(&info);
    }

    return unk;
}

bool shdSysFileExist(const char *path)
{
    return DVDConvertPathToEntrynum(path) >= 0;
}

int shdSysFileSize(const char *path)
{
    DVDFileInfo info;
    if (DVDOpen(path, &info) == 0)
        return 0;

    int size = ROUND_UP(info.size, 32);
    DVDClose(&info);
    return size;
}

void shdSysFileSave(const char *path, void *buf, int size)
{
    FILE *f = fopen(path, "wb");
    if (f == NULL)
    {
        cprintf("*** Can not create %s file. ***\n", path);
        return;
    }

    shdFlushChache(buf, size);

    if (fwrite(buf, 1, size, f) < size)
    {
        cprintf("*** Failed file write %s file. ***\n", path);
    }
    fclose(f);
}

void wiiFileLoadBegin(int ftyp, int offset, int size, u8 *address)
{
    if (!IS_ALIGNED((u32)address, 32))
        sys_err_prt("address must be aligned with 32 byte boundaries : %s\n", shd_ftyp_fname[ftyp]);

    if (readf)
        sys_err_prt("Now File Open yet %s", shd_ftyp_fname[ftyp]);

    wiiFileLoadSync(1);

    while (true)
    {
        last_dvd_info = &g_fileInfo;
        if (DVDFastOpen(entrynum[ftyp], &g_fileInfo) == 1)
            break;
        cprintf("File Open Error : %s\n", shd_ftyp_fname[ftyp]);
    }

    while (true)
    {
        if (DVDReadAsyncPrio(&g_fileInfo, address, size, offset, dvd_callback, 2) == 1)
            break;
        cprintf("File Read Error : %s(0x%X)\n", shd_ftyp_fname[ftyp], offset);
    }

    readsize = size;
    readf = 1;
}

int wiiFileLoadSync(int async)
{
    int oldsize = readsize;
    if (!readf)
    {
        readsize = 0;
        return 0;
    }

    if (shd_init_flag & DVD_THING)
    {
        readf = 0;
        last_dvd_info = NULL;
        DVDClose(&g_fileInfo);
        readsize = 0;
        return oldsize;
    }

    if (async != 0)
    {
        int intervals = wiiwk.intervals; // no idea what this should've been

        do
        {
        } while (DVDGetCommandBlockStatus(&g_fileInfo.block) != 0);

        readf = 0;
        last_dvd_info = NULL;
        DVDClose(&g_fileInfo);
        return oldsize;
    }

    if (DVDGetCommandBlockStatus(&g_fileInfo.block) == 0)
    {

        readf = 0;
        last_dvd_info = NULL;
        DVDClose(&g_fileInfo);
        return oldsize;
    }

    return -1;
}

static void dvd_callback(s32, DVDFileInfo *info)
{
    last_dvd_info = info;
}

void chk_nand_err(int err)
{
    int error = err;
    static const int errcode[4] = {
        -5001,
        -5011,
        -5012,
        -5010};

    if (shdwk.flags & 1 && wiiCNTerrEmu)
    {
        if (wiiErrEmuRate - 1 == shdRndi(0, wiiErrEmuRate + -1))
            error = errcode[wiiCNTerrEmu - 1];
    }

    if (error) // thanks vetri for this solution, not thanks shade devs
        return;
}

CW_FORCE_STRINGS(wii_file_2, "error=%d\n") // was this supposed to be used in the previous function?