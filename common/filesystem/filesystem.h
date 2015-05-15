#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup filesystem Filesystem
 * The Filesystem Module provides access to files and directories of a the native filesystem.
 */
/*@{*/

/**
 * File Attributes used by implementation
 * See http://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#attributes for detailed description
 */
typedef enum {
	F_RDO=0x01,//!< File is readonly. You cannot write to it
	F_HID=0x02,//!< File is hidden
	F_SYS=0x04,//!< File is a system file
	F_DIR=0x10,//!< It's a directory and not a file
	F_ARC=0x20 //!< File has the archive flag set (probably unused)
} FILE_ATTRIBUTES;

/**
 * Structure which represents last modified date of a file / directory
 */
typedef struct {
	unsigned year : 7; //!< year from 1980 (0..127)
	unsigned month: 4; //!< month (1..12)
	unsigned day: 5; //!< day (1..31)
} FILE_DATE_STRUCT;

/**
 * Structure which represents last modified time of a file / directory
 */
typedef struct {
	unsigned hour : 5; //!< hour (0..23)
	unsigned min: 6; //!< minute (0..59
	unsigned sec: 5; //!< second/2 (0..29)
} FILE_TIME_STRUCT;

/**
 * Structure which represents a file/directory entry. \sa DIRECTORY_STRUCT
 */
typedef struct {
	uint32_t fsize; //!< File size in bytes. 0 for directories
	FILE_DATE_STRUCT fdate; //!< Last modified date
	FILE_TIME_STRUCT ftime; //!< Last modified time
	uint8_t fattrib; //!< File/Directory Attributes
	char*    fname; //!< File/Directory name
} FILE_STRUCT;

/**
 * Structure which represents an open directory with all it's entries. \sa filesystem_dir_open
 */
typedef struct {
	const char* path; //!< Directory path (absolute)
	uint16_t num_files; //!< Number of files/directories in this directory
	FILE_STRUCT* files; //!< An array with \ref num_files FILE_STRUCT entries
} DIRECTORY_STRUCT;

/**
 * Structure which represents an open file. \sa filesystem_file_open
 */
typedef struct {
	const char* fname; //!< The absolute file name
	uint32_t fpos; //!< The current byte-position in the file. \sa filesystem_file_seek
    uint32_t fsize; //!< The total file size in bytes
} FILE_HANDLE;

/**
 * Enum to represent the success or error-code of the filesystem_file_* functions
 */
typedef enum {
	F_OK,          //!< Everything ok
	F_EOF,         //!< The write/read operation tried to write/read past the end of the file. This is not a fatal error.
	F_EACCESS,     //!< The file can not be read/written due to access problems. This is a fatal error.
	F_INVALIDPARAM,//!< You passed invalid parameters to the function
	F_DISKERROR    //!< A lowlevel disk-error occoured. This is a fatal error.
} FILE_STATUS;

/**
 * Initializes the filesystem.
 * Call this method before using any filesystem_* functions
 * @return true on success
 */
bool filesystem_init();

/**
 * Opens a directory and returns a structure which contains all files/subdirectories. \sa filesystem_dir_close()
 * @param path The absolute path to the directory to open/read
 * @return A Pointer to an initialized DIRECTORY_STRUCT on success, NULL on error
 */
DIRECTORY_STRUCT* filesystem_dir_open(const char* path);

/**
 * Closes a previously opened directory. Free's all allocated resources.
 * @param dir A Pointer to a DIRECTORY_STRUCT obtained by filesystem_dir_open().
 */
void filesystem_dir_close(DIRECTORY_STRUCT* dir);

/**
 * Opens a file for read/writing. \note Depending on the implementation you may only open one file at a time
 * @param filename The absolute file path
 * @return A Pointer to a FILE_HANDLE on success, NULL on error.
 */
FILE_HANDLE* filesystem_file_open(const char* filename);

/**
 * Closes a file.
 * @param handle The FILE_HANDLE obtained by filesystem_file_open()
 */
void filesystem_file_close(FILE_HANDLE* handle);

/**
 * Set's the read/write position to a new position
 * @param handle The FILE_HANDLE obtained by filesystem_file_open()
 * @param offset The new read/write position in bytes (absolute).
 * @return \ref F_OK on success, an error Code otherwise.
 */
FILE_STATUS filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset);

/**
 * Reads some bytes from an open file.
 * @param handle The FILE_HANDLE obtained by filesystem_file_open()
 * @param buf The Buffer to write the bytes to
 * @param size The number of bytes to read
 * @return \ref F_OK on success, \ref F_EOF if less than \p size bytes could be read, an error Code otherwise.
 */
FILE_STATUS filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size);

/**
 * Writes some bytes to a open file.
 * \note Depending on the implementation the file may not be shrinked or expanded.
 * @param handle The FILE_HANDLE obtained by filesystem_file_open()
 * @param buf The Buffer to take the bytes from
 * @param size The number of bytes to write
 * @return \ref F_OK on success, \ref F_EOF if less than \p size bytes could be written, an error Code otherwise.
 */
FILE_STATUS filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size);


/*@}*/

#endif /* FILESYSTEM_H */

