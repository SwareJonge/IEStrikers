#define _MSL_WIDE_CHAR
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"

void __prep_buffer(FILE* file) 
{
	file->mBufferPtr      = file->mBuffer;
	file->mBufferLength   = file->mBufferSize;
	file->mBufferLength   = file->mBufferLength - (file->mPosition & file->mBufferAlignment);
	file->mBufferPosition = file->mPosition;
	return;
}

int __load_buffer(FILE *file, u32 *n, int flag)
{
	int res;

	__prep_buffer(file);

	if (flag == 1) {
		file->mBufferLength = file->mBufferSize;
	}

	res = (*file->readFunc)(file->mHandle, file->mBuffer, &file->mBufferLength, file->ref_con);
	if(res == 2) {
		file->mBufferLength = 0;
	}

	if (n) {
		*n = file->mBufferLength;
	}

	if(res != 0) {
		return res;
	}

	
	file->mPosition += file->mBufferLength;

	if (!file->mMode.binary_io)
	{
		int len = file->mBufferLength;
		u8 *buf = file->mBuffer;
		while(len--) {
			if (*buf++ == '\n') {
				file->mPosition++;
			}
		}
	}

	return 0;
}

int __flush_buffer(FILE* file, size_t* length)
{
	size_t bufferLen;
	int writeCode;

	bufferLen = file->mBufferPtr - file->mBuffer;
	if (bufferLen) {
		file->mBufferLength = bufferLen;
		writeCode           = file->writeFunc(file->mHandle, file->mBuffer, &file->mBufferLength, file->ref_con);
		if (length) {
			*length = file->mBufferLength;
		}
		if (writeCode) {
			return writeCode;
		}
		file->mPosition += file->mBufferLength;
	}

	__prep_buffer(file);
	return 0;
}

//unused
void setvbuf(){
}

//unused
void setbuf(){
}
