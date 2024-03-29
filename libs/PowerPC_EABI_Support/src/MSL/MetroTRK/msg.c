#include "PowerPC_EABI_Support/MetroTRK/trk.h"

TRKPacketSeq gPacketSeq;

TRKResult TRK_MessageSend(TRK_Msg *msg)
{
    u32 write_val;
    u16 val = gPacketSeq.unk0;
    if(val == 0) val = 1;
    msg->unk12 = val;
    gPacketSeq.unk0 = (val & 0xFFFF) + 1;

    write_val = TRK_WriteUARTN(&msg->mMsg, msg->mMsgLength);
    if(write_val != 0) OSReport("MetroTRK - TRK_WriteUARTN returned %ld\n", write_val);
	return TRKSuccess;
}
