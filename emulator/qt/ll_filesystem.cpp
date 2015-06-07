extern "C" {
    #include "ll_filesystem.h"
}
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfoList>
#include <QDateTime>

QDir rootdir ("./emulated"); //Create a QDir which points to the "root" of the emulated filesystem

bool ll_filesystem_init() {
    if(!rootdir.exists()) { //if our root dir is nonexistent
        qWarning() << "Filesystem can not be emulated because the 'emulated' folder does not exist";
        return false; //mark an error
    }
    return true;
}

DIRECTORY_STRUCT* ll_filesystem_dir_open(const char* path) {
    QDir d(rootdir); //Make a copy of the rootdir QDir instance
    d.cd(path); //Change the directory to the passed path
    if(!d.exists()) {
        return NULL; //mark an error
    }

    DIRECTORY_STRUCT* directory = new DIRECTORY_STRUCT();
    //get all files and directories which are important to us. Filter out . and .. symlinks (linux)
    QFileInfoList entries = d.entryInfoList(QDir::NoDotAndDotDot|QDir::Files|QDir::Dirs|QDir::Readable|QDir::Writable|QDir::Hidden|QDir::System);
    //Fill the directory structure for the user
    directory->path = path;
    directory->num_files = entries.count();
    directory->files =  new FILE_STRUCT[directory->num_files]; //allocate array of file structs

    for(int i=0; i<entries.count(); i++){ //foreach file that we found
        QFileInfo fi = entries.at(i);
        FILE_STRUCT* entry = &(directory->files[i]); //get the pointer to the current filestruct (which should be filled)
        entry->fattrib = 0;
        entry->fname = new char[fi.fileName().length()+1]; //reserve memory for filename
        strcpy(entry->fname,fi.fileName().toStdString().c_str()); //copy filename into struct
        if(fi.isDir()) { //it's a direcory
            entry->fattrib|=F_DIR; //set directory attribute
            entry->fsize = 0;
        } else { //it's a file
            entry->fsize = fi.size(); //set filesize
        }
        if(fi.isHidden()) { //the file is hidden
            entry->fattrib|=F_HID;
        }
        if(!fi.isWritable()) { //the file is not writable
            entry->fattrib|=F_RDO; //set readonly attribue
        }

        //Set date & time of file in structure
        QDateTime dt = fi.lastModified();
        entry->fdate.year = dt.date().year()-1980; //year is realtive to 1980
        entry->fdate.month = dt.date().month();
        entry->fdate.day = dt.date().day();
        entry->ftime.hour = dt.time().hour();
        entry->ftime.min = dt.time().minute();
        entry->ftime.sec = dt.time().second()/2;

    }

    return directory; //return filled directory struct
}

void ll_filesystem_dir_close(DIRECTORY_STRUCT* dir) {
    if(dir!=NULL) { //passed handle is valid
        for(int i=0; i<dir->num_files; i++) { //foreach file
            delete dir->files[i].fname; //delete filename buffer
        }
        delete[] dir->files; //delete file array
        delete dir; //delete structure itself
    }
}

//Struct that represents a file handle for the emulator
struct QT_FILE_HANDLE : FILE_HANDLE { //..derived from the FILE_HANDLE (of the Filesystem modul)
    QFile* file; //Pointer to the open QFile* instance
};


FILE_HANDLE* ll_filesystem_file_open(const char* filename) {
    if(!rootdir.exists()) {
        return NULL;
    }
    QString filepath = rootdir.absoluteFilePath(filename); //get the absolute path to the requested file
    QFile* f = new QFile(filepath); //create a QFile instance to the requested file
    if(!f->exists()) { //File does not exist
        return NULL; //mark error
    }

    if(!f->open(QFile::ReadWrite)) { //try to open the file, it it fails then ...
        return NULL; //... mark error
    }

    //At this point we have a valid QFile instance, pointing to an existing file

    QT_FILE_HANDLE* fh = new QT_FILE_HANDLE(); //Create Structure to return to user
    fh->file = f;
    fh->fname = filename;
    fh->fpos =0;
    fh->fsize = f->size();
    return fh;
}

void ll_filesystem_file_close(FILE_HANDLE* handle) {
    if(handle!=NULL) { //passed handle is valid
        QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle); //cast pointer to QT_FILE_HANDLE
        if(fh->file->isOpen()) { //if the file is still open
            fh->file->close(); //close the file
        }
        delete fh->file; //delete QFile instance
        delete fh; //delete the fle
    }
}

FILE_STATUS ll_filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset) {
    if(handle==NULL) {
        return F_INVALIDPARAM;
    }
    QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle); //cast pointer to QT_FILE_HANDLE
    if(!fh->file->isOpen()) { //file is not open
        return F_DISKERROR;
    }
    if(offset>=fh->file->size()) { //offset exeeds filesize
        return F_INVALIDPARAM;
    }

    if(fh->file->seek(offset)) { //try to seek to desired offset
        fh->fpos = offset; //update offset in FILE_HANDLE (for user)
        return F_OK;
    } else { //seek failed
        return F_DISKERROR;
    }
}

FILE_STATUS ll_filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
    if(handle==NULL || buf==NULL) {
        return F_INVALIDPARAM;
    }
    QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle); //cast pointer to QT_FILE_HANDLE
    if(!fh->file->isOpen()) { //file is not open
        return F_DISKERROR;
    }
    if(!fh->file->isReadable()) { //file is not readable
        return F_EACCESS;
    }
    qint64 bytesRead = fh->file->read((char*)buf,size); //try to read desired amount of bytes
    if(bytesRead<0) { //read failed
        return F_DISKERROR;
    }
    fh->fpos+=bytesRead; //increase file position (for user)
    if(bytesRead!=size) { //we got less bytes than expected
        return F_EOF; //we reached the end of the file
    } else {
        return F_OK;
    }
}

FILE_STATUS ll_filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
    if(handle==NULL) {
        return F_INVALIDPARAM;
    }
    QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle); //cast pointer to QT_FILE_HANDLE
    if(!fh->file->isOpen()) { //file is not open
        return F_DISKERROR;
    }
    if(!fh->file->isWritable()) { //file is not writable
        return F_EACCESS;
    }
    qint64 bytesWritten = fh->file->write((char*)buf,size); //try to write desired amount of bytes
    if(bytesWritten<0) { //write failed
        return F_DISKERROR;
    }
    fh->fpos+=bytesWritten; //increase file position (for user)
    if(bytesWritten!=size) { //we wrote less bytes than expected
        return F_EOF; //we reached the end of the file
    } else {
        return F_OK;
    }
}
                                                                                                                                                                               
