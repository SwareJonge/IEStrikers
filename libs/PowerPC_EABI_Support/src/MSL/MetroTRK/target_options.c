#include "PowerPC_EABI_Support/MetroTRK/target_options.h"

static u8 bUseSerialIO;


void SetUseSerialIO(unsigned char sio)
{
	bUseSerialIO = sio;
	return;
}

unsigned char GetUseSerialIO(void) { return bUseSerialIO; }
