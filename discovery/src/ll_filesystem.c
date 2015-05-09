#include "ll_filesystem.h"
#include <stdio.h>

bool ll_filesystem_init() {
	return false;
}

DIRECTORY_STRUCT* ll_filesystem_dir_open(const char* path) {
	return NULL;
}

void ll_filesystem_dir_close(DIRECTORY_STRUCT* dir) {
	

}

FILE_HANDLE* ll_filesystem_file_open(const char* filename) {
	return NULL;	
}

void ll_filesystem_file_close(FILE_HANDLE* handle) {
	
}

FILE_STATUS ll_filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset) {
	return F_DISKERROR;
}

FILE_STATUS ll_filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
	return F_DISKERROR;
}

FILE_STATUS ll_filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
	return F_DISKERROR;
}
                                                                                                                                                                               


