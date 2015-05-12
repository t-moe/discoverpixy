#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	F_DIR=1,
	F_RDO=2,
	F_HID=4,
	F_SYS=8,
	F_ARC=16
} FILE_ATTRIBUTES;


typedef struct {
	unsigned year : 7; //year from 1980 (0..127)
	unsigned month: 4; //month (1..12)
	unsigned day: 5; //day (1..31)
} FILE_DATE_STRUCT;


typedef struct {
	unsigned hour : 5; //hour (0..23)
	unsigned min: 6; //minute (0..59
	unsigned sec: 5; //second/2 (0..29)
} FILE_TIME_STRUCT;


typedef struct {
	uint32_t fsize;        /* File size */
    	FILE_DATE_STRUCT fdate;        /* Last modified date */
    	FILE_TIME_STRUCT ftime;        /* Last modified time */
    	uint8_t fattrib;      /* Attribute */
        char*    fname;    /* File name */
} FILE_STRUCT;

typedef struct {
	const char* path;
	uint16_t num_files;
        FILE_STRUCT* files;
} DIRECTORY_STRUCT;

typedef struct {
	const char* fname;
	uint32_t fpos;
        uint32_t fsize;
} FILE_HANDLE;

typedef enum {
	F_OK,
	F_EOF,
        F_EACCESS,
        F_INVALIDPARAM,
        F_DISKERROR
} FILE_STATUS;


bool filesystem_init();

DIRECTORY_STRUCT* filesystem_dir_open(const char* path);
void filesystem_dir_close(DIRECTORY_STRUCT* dir);
FILE_HANDLE* filesystem_file_open(const char* filename);
void filesystem_file_close(FILE_HANDLE* handle);
FILE_STATUS filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset);
FILE_STATUS filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size);
FILE_STATUS filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size);


#endif /* FILESYSTEM_H */

