#include <Shade/shd.h>
#include <Shade/wii.h>
#include <Shade/wii_file.h>
#include <string.h>

char **shd_ftyp_fname;
int shd_ftyp_max;
static int bk_ftyp;
static int bk_ofs;
static int bk_size;
static u8 *bk_buff;
static int bk_mcb_ofs;

void shdFileInit(char **fname, int max)
{
    shd_ftyp_fname = fname;
    shd_ftyp_max = max;
    shdsys.mcbFlags = 0;
    wiiFileInit();
}

int getMcbFileOffset(int ftyp, int offset, int size)
{
    u8 *mcbBuf = shdsys.mcbBuf;
loop: // ?
    if (READ_LE32(mcbBuf, 0) != 0x7fff)
    {
        if (ftyp == READ_LE32(mcbBuf, 0) && offset == READ_LE32(mcbBuf, 4) && size == READ_LE32(mcbBuf, 8))
        {
            return mcbBuf - shdsys.mcbBuf;
        }

        mcbBuf = mcbBuf + ((u32)((READ_LE32(mcbBuf, 8) >> 2) + 3) << 2);
        goto loop;
        
    }
    return -1;
}

void shdFileLoadBegin(int ftyp, int offset, int size, u8 *buff)
{
    if (!IS_ALIGNED(offset, 2048))
        sys_err_prt("file offset need *2048");
    if (!IS_ALIGNED(size, 2048))
        sys_err_prt("file size need *2048");

    bk_ftyp = ftyp;
    bk_ofs = offset;
    bk_size = size;
    bk_buff = buff;

    if (shdsys.mcbFlags == 1 || shdsys.mcbFlags == -2)
    {
        bk_mcb_ofs = getMcbFileOffset(ftyp, offset, size);

        if (bk_mcb_ofs >= 0)
        {
            cprintf(" mcb");

            u8 *buf2 = (shdsys.mcbBuf + bk_mcb_ofs);
            u8 *dst = bk_buff;
            buf2 += 8;
            int len = READ_LE32(buf2, 0);

            memcpy(dst, buf2 + 4, len);
            shdFlushChache(dst, len);
            return;
        }
    }
    wiiFileLoadBegin(ftyp, offset, size, buff);
}

int shdFileLoadSync(int async)
{
    if (((shdsys.mcbFlags == 1) || (shdsys.mcbFlags == -2)) && (bk_mcb_ofs >= 0))
    {
        return bk_size;
    }
    return wiiFileLoadSync(async);
}

void shdMcbInit(char *mcb0path, char *mcb1path, u8 *buf, int mcbBufSize, u32 *ftimeBuf, int max_entries, u8 *tblBuf)
{
    shdsys.mcbTblBufSize = shdMcbCalTblSz(max_entries);
    shdsys.mcbTblSize = max_entries;
    shdsys.mcbTblBuf = (mcb0Entry *)tblBuf;
    shdsys._168 = 0;
    shdsys.mcbFlags = 0;
    shdsys.openMcbId = 0;
    shdsys.mcb0Path = mcb0path;
    shdsys.mcb1Path = mcb1path;
    shdsys.mcbBuf = buf;
    shdsys.mcbBufSize = mcbBufSize;
    shdsys.ftyp_ftime = ftimeBuf;

    if (shdSysFileExist(mcb0path) != false)
    {
        shdSysFileLoad(shdsys.mcb0Path, 0, shdsys.mcbTblBuf, shdsys.mcbTblBufSize);
        if ((shdwk.flags & 0x200) == 0)
        {
            u32 *szPtr = &shdsys.mcbTblBuf[shdsys.mcbTblSize - 1].size;
            for (int i = 0; i < shd_ftyp_max; szPtr--, i++)
            {
                if (shdsys.ftyp_ftime[i] != READ_LE32(szPtr, 0))
                {
                    cprintf("mcb invalid\n");
                    goto error; // i feel like this could be done with a break? not sure
                }
            }
        }
        cprintf("mcb initialize\n");
        return;
    }

    cprintf("mcb none\n");
error:
    if (((shdwk.video_flags & 1) != 0) && ((shdwk.flags & 0x40) == 0))
    {
        sys_err_prt("No MCB (init)");
    }

    for (int i = 0; i < shdsys.mcbTblSize; i++)
    {
        shdsys.mcbTblBuf[i].id = 0;
        shdsys.mcbTblBuf[i].offset = 0;
        shdsys.mcbTblBuf[i].size = 0;
    }
    memset(shdsys.mcbTblBuf, 0, shdsys.mcbTblBufSize);

    if ((shdwk.flags & 0x40) != 0)
    {
        u32 *szPtr = &shdsys.mcbTblBuf[shdsys.mcbTblSize - 1].size;
        for (int i = 0; i < shd_ftyp_max; i++)
        {
            *szPtr-- = READ_LE32(&shdsys.ftyp_ftime[i], 0);
        }

        shdSysFileSave(shdsys.mcb0Path, shdsys.mcbTblBuf, shdsys.mcbTblBufSize);
        shdSysFileSave(shdsys.mcb1Path, shdsys.mcbTblBuf, 0);
        cprintf("mcb create\n");
    }
}

CW_FORCE_STRINGS(shd_file_1,"mcb clr\n")

int shdMcbCalTblSz(int entries)
{
    int fileSize = entries * 0xC;
    return ((2048 - 1) + fileSize) / 2048 * 2048; // macro?
}

inline int shdgetOpenIdx(u32 mcb_id) // fabricated
{
    for (int i = 0; i < shdsys.mcbTblSize - 0x10; i++)
    {
        u32 id = READ_LE32(&shdsys.mcbTblBuf[i].id, 0);

        if (id == 0)
        {
            return -1;
        }

        if (id == mcb_id)
            return i;
    }
    return -1;
}

int shdMcbGetSize(u32 mcb_id)
{
    if (mcb_id == 0)
        sys_err_prt("mcb_id 0 is bad.");
    int openId = shdgetOpenIdx(mcb_id);
    if (openId >= 0)
        return READ_LE32(&shdsys.mcbTblBuf[openId].size, 0);

    return -1;
}

CW_FORCE_STRINGS(shd_file_2,
                 "mcb has already been opened.",
                 "\n$$$$$ shdMcbPreOpen dif $$$$$\n\n",
                 "mcb sz over.",
                 "mcb pre open id=%x sz=0x%x\n")

void shdMcbOpen(u32 mcb_id)
{
    wiiLockMutexMcb();

    if (((shdsys.mcbFlags != 1) && (shdsys.mcbFlags != -2)) || (bk_mcb_ofs < 0))
    {
        wiiFileLoadSync(1);
    }

    if (shdsys.mcbFlags != 0)
    {
        sys_err_prt("mcb has already been opened.");
    }

    if (mcb_id == 0)
    {
        sys_err_prt("mcb_id 0 is bad.");
    }

    if (shdsys.openMcbId == mcb_id)
    {
        shdsys.mcbFlags = '\x01';
        cprintf("mcb open id=%x exist\n", mcb_id);
    }
    else
    {
        shdsys.openMcbId = mcb_id;
        int openId = shdgetOpenIdx(mcb_id);

        if (openId >= 0)
        {

            if (shdsys._168 != mcb_id)
            {
                int offset = READ_LE32(&shdsys.mcbTblBuf[openId].offset, 0);
                u32 sz = READ_LE32(&shdsys.mcbTblBuf[openId].size, 0);

                if (sz > shdsys.mcbBufSize)
                {
                    sys_err_prt("mcb sz over.");
                }

                shdSysFileLoad(shdsys.mcb1Path, offset, shdsys.mcbBuf, sz);
                cprintf("mcb open id=%x sz=0x%x\n", mcb_id, sz);
            }
            shdsys._168 = 0;
            shdsys.mcbFlags = 1;
        }
        else
        {
            if (((shdwk.video_flags & 1)) && ((shdwk.flags & 0x40) == 0))
            {
                sys_err_prt("No MCB (ID=0x%x)", mcb_id);
            }
            *(u32 *)shdsys.mcbBuf = 0xff7f0000; // change to void*?
            if ((shdwk.flags & 0x40))
            {
                shdsys.mcbFlags = -2;
                cprintf("mcb open id=%x make\n", mcb_id);
            }
            else
            {
                shdsys.mcbFlags = -1;
                cprintf("mcb open id=%x none\n", mcb_id);
            }            
        }
        shdsys._168 = 0;
    }

    wiiUnlockMutexMcb();
}

void shdMcbClose()
{
    if (shdsys.mcbFlags != 0)
    {
        wiiLockMutexMcb();
        if (shdsys.mcbFlags != -2)
        {
            shdsys.mcbFlags = 0;
            cprintf("mcb close id=%x\n", shdsys.openMcbId);
        }
        else
        {
            shdsys.mcbFlags = 0;
            // is this loop supposed to do something?
            for (int i = 0; i < shdsys.mcbTblSize - 0x10; i++)
            { // - 0x10 to take the key data or whatever data at the end into account?
                if (shdsys.mcbTblBuf[i].id == 0)
                    break;
            }
        }
        wiiUnlockMutexMcb();
    }
}

void shdMcbOvrCntClr() {}