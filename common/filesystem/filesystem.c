#include "filesystem.h"
#include "ll_filesystem.h"

bool filesystem_init() {
	return ll_filesystem_init();
}

DIRECTORY_STRUCT* filesystem_dir_open(const char* path) {
	return ll_filesystem_dir_open(path);
}

void filesystem_dir_close(DIRECTORY_STRUCT* dir) {
	filesystem_dir_close(dir);
}

FILE_HANDLE* filesystem_file_open(const char* filename) {
	return ll_filesystem_file_open(filename);
}

void filesystem_file_close(FILE_HANDLE* handle) {
	ll_filesystem_file_close(handle);
}

FILE_STATUS filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset) {
	return ll_filesystem_file_seek(handle,offset);
}

FILE_STATUS filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
	return ll_filesystem_file_read(handle,buf,size);
}

FILE_STATUS filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
	return ll_filesystem_file_write(handle,buf,size);
}




