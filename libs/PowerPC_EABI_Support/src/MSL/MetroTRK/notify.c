#include "types.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"


void TRKWaitForACK(void)
{
	// UNUSED FUNCTION
}

s32 TRKDoNotifyStopped(s32 arg0)
{
	s32 sp8;
	s32 spC;
	s32 sp10;
	s32 retval; // r31

	retval = TRK_GetFreeBuffer(&spC, &sp10);

	if (retval == 0) {
		if (arg0 == 0x90) {
			TRKTargetAddStopInfo(sp10);
		} else {
			TRKTargetAddExceptionInfo(sp10);
		}

		retval = TRK_RequestSend(sp10, &sp8);

		if (retval == 0) {
			TRK_ReleaseBuffer(sp8);
		}
		TRK_ReleaseBuffer(spC);
	}
	return retval;
}
