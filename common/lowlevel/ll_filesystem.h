#include "filesystem.h"

/**
 * @addtogroup lowlevel
 */
/*@{*/

/**
 * @defgroup ll_filesystem Filesystem (LowLevel)
 * Low level functions for the \ref filesystem module
 */
/*@}*/

/**
 * @addtogroup ll_filesystem
 */
/*@{*/


bool ll_filesystem_init();

DIRECTORY_STRUCT* ll_filesystem_dir_open(const char* path);
void ll_filesystem_dir_close(DIRECTORY_STRUCT* dir);
FILE_HANDLE* ll_filesystem_file_open(const char* filename);
void ll_filesystem_file_close(FILE_HANDLE* handle);
FILE_STATUS ll_filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset);
FILE_STATUS ll_filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size);
FILE_STATUS ll_filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size);

/*@}*/
