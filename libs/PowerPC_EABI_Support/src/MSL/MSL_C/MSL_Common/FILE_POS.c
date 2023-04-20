#include "types.h"

#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h"
#include "errno.h"

int _ftell(FILE* file)
{
	int charsInUndoBuffer = 0;
	int position;

	u8 tmp_kind = file->mMode.file_kind;
	if (!(tmp_kind == __disk_file || tmp_kind == __console_file) || file->mState.error) {
		errno = EFPOS;
		return -1;
	}

	if (file->mState.io_state == __neutral)
		return (file->mPosition);

	position = file->mBufferPosition + (file->mBufferPtr - file->mBuffer);

	if (file->mState.io_state >= __rereading) {
		charsInUndoBuffer = file->mState.io_state - __rereading + 1;
		position -= charsInUndoBuffer;
	}

	if (!file->mMode.binary_io) {
		int n = file->mBufferPtr - file->mBuffer - charsInUndoBuffer;
		u8* p = (u8*)file->mBuffer;

		while (n--)
			if (*p++ == '\n')
				position++;
	}

	return (position);
}

int ftell(FILE* stream)
{
	int retval;

	retval = (long)_ftell(stream);
	return retval;
}

//unused
void fgetpos(){
}

int _fseek(FILE* file, u32 offset, int whence)
{
	int bufferCode;
	int pos;
	int adjust;
	u32 state;
	int buffLen;

	char* ptr;

	if (file->mMode.file_kind != 1 || file->mState.error != 0) {
		errno = EFPOS;
		return -1;
	}

	if (file->mState.io_state == 1) {
		if (__flush_buffer(file, nullptr) != 0) {
			file->mState.error  = 1;
			file->mBufferLength = 0;
			errno = EFPOS;
			return -1;
		}
	}

    if(whence == SEEK_CUR){
        whence = SEEK_SET;
        offset += _ftell(file);
    }

	if ((whence != SEEK_END) && (file->mMode.io_mode != 3) && (file->mState.io_state == 2 || file->mState.io_state == 3)) {
		if ((offset >= file->mPosition) || !(offset >= file->mBufferPosition)) {
			file->mState.io_state = 0;
		} else {
			file->mBufferPtr      = file->mBuffer + (offset - file->mBufferPosition);
			file->mBufferLength   = file->mPosition - offset;
			file->mState.io_state = 2;
		}
	} else {
		file->mState.io_state = 0;
	}

	if (file->mState.io_state == 0) {
		if (file->positionFunc != nullptr && (int)file->positionFunc(file->mHandle, &offset, whence, file->ref_con)) {
			file->mState.error  = 1;
			file->mBufferLength = 0;
			errno = EFPOS;
			return -1;
		} else {
			file->mState.eof = 0;
			file->mPosition = offset;
			file->mBufferLength = 0;
		}
	}

	return 0;
}

int fseek(FILE* stream, u32 offset, int whence)
{
	int code;
	code = _fseek(stream, offset, whence); // 0 if successful, -1 if error
	return code;
}

//unused
void fsetpos(){
}

void rewind(FILE *stream)
{
	stream->mState.error = 0;
	_fseek(stream, 0, 0);
	stream->mState.error = 0;
}
