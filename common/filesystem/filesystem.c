/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/filesystem/filesystem.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-05-10	timolang@gmail.com	e2bce8f	Added filesystem module, tests and implementation for it in emulator.
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
*
**************************************************************************************************************************************/

#include "filesystem.h"
#include "ll_filesystem.h"

bool filesystem_init()
{
    return ll_filesystem_init();
}

DIRECTORY_STRUCT* filesystem_dir_open(const char* path)
{
    return ll_filesystem_dir_open(path);
}

void filesystem_dir_close(DIRECTORY_STRUCT* dir)
{
    ll_filesystem_dir_close(dir);
}

FILE_HANDLE* filesystem_file_open(const char* filename)
{
    return ll_filesystem_file_open(filename);
}

void filesystem_file_close(FILE_HANDLE* handle)
{
    ll_filesystem_file_close(handle);
}

FILE_STATUS filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset)
{
    return ll_filesystem_file_seek(handle, offset);
}

FILE_STATUS filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size)
{
    return ll_filesystem_file_read(handle, buf, size);
}

FILE_STATUS filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size)
{
    return ll_filesystem_file_write(handle, buf, size);
}
