#include "PowerPC_EABI_Support/MetroTRK/trk.h"

#pragma dont_inline on

BOOL gTRKBigEndian;

TRKResult TRK_InitializeNub(void)
{
	TRKResult TVar1;
	TRKResult TVar2;

	TVar1 = TRK_InitializeEndian();

	if (TVar1 == TRKSuccess) {
		TVar1 = TRKInitializeEventQueue();
	}
	if (TVar1 == TRKSuccess) {
		TVar1 = TRK_InitializeMessageBuffers();
	}
	InitializeProgramEndTrap();
	if (TVar1 == TRKSuccess) {
		TVar1 = TRKInitializeSerialHandler();
	}
	if (TVar1 == TRKSuccess) {
		TVar1 = TRKInitializeTarget();
	}
	if (TVar1 == TRKSuccess) {

		TVar2 = TRK_InitializeIntDrivenUART(1, 0, &gTRKInputPendingPtr);
		TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
		if (TVar2 != TRKSuccess) {
			TVar1 = TVar2;
		}
	}
	return TVar1;
}

extern TRKResult TRKTerminateSerialHandler(void);
TRKResult TRK_TerminateNub(void)
{
	TRKTerminateSerialHandler();
	return TRKSuccess;
}

extern void TRK_board_display(char*);
void TRK_NubWelcome(void)
{
	TRK_board_display("MetroTRK for Revolution v0.4");
	return;
}

BOOL TRK_InitializeEndian(void)
{
	u8 bendian[4];
	TRKResult result = FALSE;
	gTRKBigEndian = TRUE;

	bendian[0] = 0x12;
	bendian[1] = 0x34;
	bendian[2] = 0x56;
	bendian[3] = 0x78;

	if (*(u32*)bendian == 0x12345678) {
		gTRKBigEndian = TRUE;
	} else if (*(u32*)bendian == 0x78563412) {
		gTRKBigEndian = FALSE;
	} else {
		result = TRUE;
	}
	return result;
}
