#include <string.h>
#include <revolution/IPC.h>
#include <revolution/OS.h>

#define PATH_SIZE 48
#define REQUEST_MAX 96

static s32 IpcNumUnIssuedReqs;
static s32 IpcNumPendingReqs;

static IPCRequest IpcReqArray[REQUEST_MAX] = {}; // forces IpcReqArray to be at the top of bss
static IPCRequestEx* IpcReqPtrArray[REQUEST_MAX];
static s64 IpcStartTimeArray[REQUEST_MAX];

static char IpcHandlePathBuf[128][PATH_SIZE];
static char IpcOpenPathBuf[REQUEST_MAX][PATH_SIZE];

static void AddReqInfo(IPCRequestEx* req);
static void DelReqInfo(IPCRequestEx* req);

void IPCiProfInit(void) {
    u32 i;

    IpcNumPendingReqs = 0;
    IpcNumUnIssuedReqs = 0;

    for (i = 0; i < REQUEST_MAX; i++) {
        IpcReqPtrArray[i] = NULL;
        IpcStartTimeArray[i] = 0;
    }

    memset(IpcHandlePathBuf, 0, sizeof(IpcHandlePathBuf));
    memset(IpcOpenPathBuf, 0, sizeof(IpcOpenPathBuf));
    memset(IpcReqArray, 0, sizeof(IpcReqArray));
}

void IPCiProfQueueReq(IPCRequestEx* req, s32 fd) {
    IpcNumPendingReqs++;
    IpcNumUnIssuedReqs++;
    AddReqInfo(req);
}

void IPCiProfAck(void) { IpcNumUnIssuedReqs--; }

void IPCiProfReply(IPCRequestEx* req, s32 fd) {
    IpcNumPendingReqs--;
    DelReqInfo(req);
}

static void AddReqInfo(IPCRequestEx* req) {
    u32 i;
    BOOL enable;

    for (i = 0; i < REQUEST_MAX; i++) {
        if (IpcReqPtrArray[i] == NULL) {
            enable = OSDisableInterrupts();
            IpcReqPtrArray[i] = req;
            IpcReqArray[i] = IpcReqPtrArray[i]->base;

            IpcStartTimeArray[i] = OSGetTime();
            if (IpcReqArray[i].type == IPC_REQ_OPEN) {
                strncpy(IpcOpenPathBuf[i],
                        OSPhysicalToCached((u32)IpcReqArray[i].open.path),
                        PATH_SIZE - 1);
                IpcOpenPathBuf[i][PATH_SIZE - 1] = '\0';
                IpcReqArray[i].open.path = IpcOpenPathBuf[i];
            }

            OSRestoreInterrupts(enable);
            return;
        }
    }
}

static void DelReqInfo(IPCRequestEx* req) {
    u32 i;
    BOOL enable;

    for (i = 0; i < REQUEST_MAX; i++) {
        if (req == IpcReqPtrArray[i] && req->base.fd == IpcReqArray[i].type) {
            enable = OSDisableInterrupts();

            if (IpcReqArray[i].type == IPC_REQ_OPEN && 0 <= req->base.ret) {
                strncpy(IpcHandlePathBuf[req->base.ret],
                        IpcReqArray[i].open.path, PATH_SIZE - 1);
                IpcHandlePathBuf[req->base.ret][PATH_SIZE - 1] = '\0';
                memset(&IpcOpenPathBuf[i], 0, PATH_SIZE);
            }
            if (IpcReqArray[i].type == IPC_REQ_CLOSE) {
                memset(&IpcHandlePathBuf[IpcReqArray[i].fd], 0, PATH_SIZE);
            }

            IpcReqPtrArray[i] = NULL;
            memset(&IpcReqArray[i], 0, sizeof(IPCRequest));
            IpcStartTimeArray[i] = 0;

            OSRestoreInterrupts(enable);
            return;
        }
    }
}
