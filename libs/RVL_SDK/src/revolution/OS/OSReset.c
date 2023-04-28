#include <revolution/DVD.h>
#include <revolution/OS.h>
#include <revolution/PAD.h>
#include <revolution/SC.h>
#include <revolution/VI.h>
#include <string.h>

static OSShutdownFunctionQueue ShutdownFunctionQueue;
BOOL __OSIsReturnToIdle;
static BOOL bootThisDol;

static void KillThreads(void);

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info) {
    OSShutdownFunctionInfo* it;
    OSShutdownFunctionInfo* prev;
    OSShutdownFunctionInfo* tail;

    for (it = ShutdownFunctionQueue.head; it != NULL && it->prio <= info->prio;
         it = it->next) {
        ;
    }

    if (it == NULL) {

        tail = ShutdownFunctionQueue.tail;
        if (tail == NULL) {
            ShutdownFunctionQueue.head = info;
        } else {
            ShutdownFunctionQueue.tail->next = info;
        }

        info->prev = tail;
        info->next = NULL;

        it = ShutdownFunctionQueue.head;
        ShutdownFunctionQueue.tail = info;
    } else {
        info->next = it;

        prev = it->prev;
        it->prev = info;
        info->prev = prev;

        if (prev == NULL) {
            ShutdownFunctionQueue.head = info;
        } else {
            prev->next = info;
        }
    }
}

BOOL __OSCallShutdownFunctions(u32 pass, u32 event) {
    OSShutdownFunctionInfo* iter;
    BOOL failure;
    u32 prio;

    prio = 0;
    failure = FALSE;

    for (iter = ShutdownFunctionQueue.head; iter != NULL; iter = iter->next) {
        if (failure && prio != iter->prio) {
            break;
        }

        failure |= !iter->func(pass, event);
        prio = iter->prio;
    }

    failure |= !__OSSyncSram();

    return !failure;
}

void __OSShutdownDevices(u32 event) {
    BOOL padIntr;
    BOOL osIntr;
    BOOL keepEnable;

    switch (event) {
    case 0:
    case 5:
    case 6:
        keepEnable = FALSE;
        break;
    case 1:
    case 2:
    case 3:
    default:
        keepEnable = TRUE;
        break;
    }

    __OSStopAudioSystem();

    if (!keepEnable) {
        padIntr = __PADDisableRecalibration(TRUE);
    }

    while (!__OSCallShutdownFunctions(OS_SD_PASS_FIRST, event)) {
        ;
    }

    while (!__OSSyncSram()) {
        ;
    }

    osIntr = OSDisableInterrupts();
    __OSCallShutdownFunctions(OS_SD_PASS_SECOND, event);
    LCDisable();

    if (!keepEnable) {
        __PADDisableRecalibration(padIntr);
    }

    KillThreads();
}

void __OSGetDiscState(u8* out) {
    u8 state;
    u32 flags;

    state = *out;

    if (__DVDGetCoverStatus() != 2) {
        state = 3;
    } else {
        if (state == 1 && __OSGetRTCFlags(&flags) && flags == 0) {
            state = 1;
        } else {
            state = 2;
        }
    }
    *out = state;
}

static void KillThreads(void) {
    OSThread* iter;
    OSThread* next;

    for (iter = OS_THREAD_QUEUE.head; iter != NULL; iter = next) {
        next = iter->nextActive;

        switch (iter->state) {        
        case OS_THREAD_STATE_READY:
        case OS_THREAD_STATE_SLEEPING:
            OSCancelThread(iter);
            break;
        }
    }
}

void OSShutdownSystem(void) {
    SCIdleMode idleMode;
    OSStateFlags stateFlags;
    OSIOSRev iosRev;

    memset(&idleMode, 0, sizeof(SCIdleMode));
    SCInit();
    while (SCCheckStatus() == SC_STATUS_1) {
        ;
    }
    SCGetIdleMode(&idleMode);

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&stateFlags);

    __OSGetDiscState(&stateFlags.discState);
    if (idleMode.wc24 == TRUE) {
        stateFlags.BYTE_0x5 = 5;
    } else {
        stateFlags.BYTE_0x5 = 1;
    }

    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);
    __OSGetIOSRev(&iosRev);

    if (idleMode.wc24 == TRUE) {
        __OSIsReturnToIdle = TRUE;
        OSDisableScheduler();
        __OSShutdownDevices(5);
        OSEnableScheduler();
        __OSLaunchMenu();
    } else {
        OSDisableScheduler();
        __OSShutdownDevices(2);
        __OSShutdownToSBY();
    }
}

void OSRestart(int arg) {
    u8 apptype = OSGetAppType();
    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    if (apptype == 0x81) {
        OSDisableScheduler();
        __OSShutdownDevices(4);
        OSEnableScheduler();
        __OSRelaunchTitle(arg);
    } else if (apptype == 0x80) {
        OSDisableScheduler();
        __OSShutdownDevices(4);
        OSEnableScheduler();
        __OSReboot(arg, bootThisDol);
    }
    OSDisableScheduler();
    __OSShutdownDevices(1);
    __OSHotResetForError();
}

static void __OSReturnToMenu(int state) {
    OSStateFlags stateFlags;
    void* mem;
    s32 playtime1, playtime2;

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();

    __OSReadStateFlags(&stateFlags);
    __OSGetDiscState(&stateFlags.discState);
    stateFlags.BYTE_0x5 = 3;
    stateFlags.BYTE_0x7 = state;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);

    OSSetArenaLo((void*)0x81280000); // I Assume there are macros for these?
    OSSetArenaHi((void*)0x812f0000);

    if (ESP_InitLib()) {
        __OSReturnToMenuForError();
    }

    mem = OSAllocFromMEM1ArenaLo(0xe0, 0x20); // TODO: struct
    if(mem == NULL) {
        __OSReturnToMenuForError();
    }
    memset(mem, 0, 0xe0);

    if (!ESP_DiGetTicketView(0, mem) && OSPlayTimeIsLimited()) {
        playtime1 = 0;
        playtime2 = -1;
        __OSGetPlayTime(mem, &playtime1, &playtime2);
        if (playtime2 == 0) {
            __OSWriteExpiredFlagIfSet();
        }
    }

    OSDisableScheduler();
    __OSShutdownDevices(5);

    OSEnableScheduler();    
    __OSLaunchMenu();
    OSDisableScheduler();

    __VISetRGBModeImm();
    __OSHotResetForError();
}

void OSReturnToMenu(void) {
    __OSReturnToMenu(0);
#line 895
    OSError("OSReturnToMenu(): Falied to boot system menu.\n");
}

void OSReturnToDataManager() {
    __OSReturnToMenu(1);
#line 913
    OSError("OSReturnToDataManager(): Falied to boot system menu.\n");
}

void __OSReturnToMenuForError() {
    OSStateFlags stateFlags;
    __OSReadStateFlags(&stateFlags);

    stateFlags.discState = 2;
    stateFlags.BYTE_0x5 = 3;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);
    __OSLaunchMenu();
    OSDisableScheduler();
    __VISetRGBModeImm();
    __OSHotResetForError();
#line 1010
    OSError("__OSReturnToMenu(): Falied to boot system menu.\n");
}

void __OSHotResetForError() {
    if (__OSInNandBoot || __OSInReboot) {
        __OSInitSTM();
    }
    __OSHotReset();
#line 1034
    OSError("__OSHotReset(): Falied to reset system.\n");
}

u32 OSGetResetCode(void) {
    if (__OSRebootParams.WORD_0x0 != 0) {
        return __OSRebootParams.WORD_0x4 | 0x80000000;
    }

    return PI_HW_REGS[PI_RESET] >> 3;
}

CW_FORCE_STRINGS(OSReset_c, "Calendar/Calendar_index.html",
                 "Display/Display_index.html", "Sound/Sound_index.html",
                 "Parental_Control/Parental_Control_index.html",
                 "Internet/Internet_index.html",
                 "WiiConnect24/Wiiconnect24_index.html",
                 "Update/Update_index.html",
                 "OSReturnToSetting(): You can't specify %d.  \n")

void OSResetSystem(u32 arg0, u32 arg1, u32 arg2) {
#pragma unused(arg0)
#pragma unused(arg1)
#pragma unused(arg2)

#line 1185
    OSError("OSResetSystem() is obsoleted. It doesn't work any longer.\n");
}

CW_FORCE_STRINGS(OSReset_c_2,
                 "OSSetBootDol() is obsoleted. It doesn't work any longer.\n")