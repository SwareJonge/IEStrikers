#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"
#include "stdlib.h"

//unused
void remove(){
}

//unused
void rename(){
}

//unused
void tmpnam(){
}

//unused
void tmpnam_s(){
}

//unused
void tmpfile(){
}

//unused
void tmpfile_s(){
}

int fclose(FILE* file)
{
	int flush_result, close_result;

	if (file == NULL)
		return (-1);
	if (file->mMode.file_kind == __closed_file)
		return (0);

	flush_result = fflush(file);

	close_result = (*file->closeFunc)(file->mHandle);

	file->mMode.file_kind = __closed_file;
	file->mHandle         = 0;

	if (file->mState.free_buffer)
		free(file->mBuffer);
	return ((flush_result || close_result) ? -1 : 0);
}

int fflush(FILE* file)
{
	int pos;

	if (file == NULL) {
		return __flush_all();
	}

	if (file->mState.error != 0 || file->mMode.file_kind == __closed_file) {
		return -1;
	}

	if (file->mMode.io_mode == 1) {
		return 0;
	}

	if (file->mState.io_state >= 3) {
		file->mState.io_state = 2;
	}

	if (file->mState.io_state == 2) {
		file->mBufferLength = 0;
	}

	if (file->mState.io_state != 1) {
		file->mState.io_state = 0;
		return 0;
	}

	if (file->mMode.file_kind != __disk_file) {
		pos = 0;
	} else {
		pos = ftell(file);
	}

	if (__flush_buffer(file, 0) != 0) {
		file->mState.error  = 1;
		file->mBufferLength = 0;
		return -1;
	}

	file->mState.io_state = 0;
	file->mPosition       = pos;
	file->mBufferLength   = 0;
	return 0;
}

FILE *fopen(char *filename, char *mode) {
	// TODO
	//return freopen(filename, mode, __find_unopened_file());
}

//unused
void fopen_s(){
}

FILE *freopen(){
	// TODO
}

//unused
void freopen_s(){
}

//unused
void __reopen(){
}

//unused
void __handle_open(){
}

//unused
void __handle_reopen(){
}

void __get_file_modes(){
	// TODO
}

//unused
void __set_ref_con(){
}

//unused
void __set_idle_proc(){
}

int __msl_strnicmp(const char *s1, const char *s2,	size_t len) {
	// TODO
	return 0;
}

//unused
void __msl_strrev(){
}

//unused
void __msl_itoa(){
}

//unused
void __msl_strdup(){
}
