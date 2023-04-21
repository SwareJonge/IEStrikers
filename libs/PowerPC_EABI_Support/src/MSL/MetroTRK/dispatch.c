#include "PowerPC_EABI_Support/MetroTRK/dispatch.h"

u32 TRK_DispatchMessage(MessageBuffer *messageBuffer)
{
    u32 result = 0x500;
    MessageBuffer* temp = messageBuffer;
    
    TRK_SetBufferPosition(messageBuffer, 0);
    
    switch(temp->commandId){
        case 1:
            result = TRK_DoConnect(temp);
            break;
        case 2:
            result = TRKDoDisconnect(temp);
        	break;
        case 3:
            result = TRKDoReset(temp);
        	break;
        case 7:
            result = TRKDoOverride(temp);
        	break;
        case 16:
            result = TRKDoReadMemory(temp);
        	break;
        case 17:
            result = TRKDoWriteMemory(temp);
        	break;
        case 18:
            result = TRKDoReadRegisters(temp);
        	break;
        case 19:
            result = TRKDoWriteRegisters(temp);
        	break;
        case 24:
            result = TRKDoContinue(temp);
        	break;
        case 25:
            result = TRKDoStep(temp);
        	break;
        case 26:
            result = TRKDoStop(temp);
        	break;
        case 23:
            result = TRKDoSetOption(temp);
        	break;
    }
    return result;
}
