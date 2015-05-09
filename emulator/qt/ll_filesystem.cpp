extern "C" {
    #include "ll_filesystem.h"
}
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfoList>
#include <QDateTime>

QDir rootdir ("./emulated");

bool ll_filesystem_init() {
    if(!rootdir.exists()) {
        qWarning() << "Filesystem can not be emulated because the 'emulated' folder does not exist";
        return false;
    }
    return true;
}

DIRECTORY_STRUCT* ll_filesystem_dir_open(const char* path) {
    QDir d(rootdir);
    d.cd(path);
    if(!d.exists()) {
        return NULL;
    }

    DIRECTORY_STRUCT* directory = new DIRECTORY_STRUCT();
    QFileInfoList entries = d.entryInfoList(QDir::NoDotAndDotDot|QDir::Files|QDir::Dirs|QDir::Readable|QDir::Writable|QDir::Hidden|QDir::System);
    directory->path = path;
    directory->num_files = entries.count();
    directory->files =  new FILE_STRUCT[directory->num_files];


    for(int i=0; i<entries.count(); i++){
        QFileInfo fi = entries.at(i);
        FILE_STRUCT* entry = &(directory->files[i]);
        entry->fattrib = 0;
        entry->fname = new char[fi.fileName().length()+1];
        strcpy(entry->fname,fi.fileName().toStdString().c_str());
        if(fi.isDir()) {
            entry->fattrib|=F_DIR;
            entry->fsize = 0;
        } else {
            entry->fsize = fi.size();
        }
        if(fi.isHidden()) {
            entry->fattrib|=F_HID;
        }
        if(!fi.isWritable()) {
            entry->fattrib|=F_RDO;
        }

        QDateTime dt = fi.lastModified();
        entry->fdate.year = dt.date().year()-1980;
        entry->fdate.month = dt.date().month();
        entry->fdate.day = dt.date().day();
        entry->ftime.hour = dt.time().hour();
        entry->ftime.min = dt.time().minute();
        entry->ftime.sec = dt.time().second()/2;

    }

    return directory;
}

void ll_filesystem_dir_close(DIRECTORY_STRUCT* dir) {
    if(dir!=NULL) {
        for(int i=0; i<dir->num_files; i++) {
            delete dir->files[i].fname;
        }
        delete[] dir->files;
        delete dir;
    }
}


struct QT_FILE_HANDLE : FILE_HANDLE {
    QFile* file;
};


FILE_HANDLE* ll_filesystem_file_open(const char* filename) {
    if(!rootdir.exists()) {
        return NULL;
    }
    QString filepath = rootdir.absoluteFilePath(filename);
    QFile* f = new QFile(filepath);
    if(!f->exists()) {
        return NULL;
    }

    if(!f->open(QFile::ReadWrite)) {
        return NULL;
    }

    QT_FILE_HANDLE* fh = new QT_FILE_HANDLE();
    fh->file = f;
    fh->fname = filename;
    fh->fpos =0;
    fh->fsize = f->size();
    return fh;
}

void ll_filesystem_file_close(FILE_HANDLE* handle) {
    if(handle!=NULL) {
        QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle);
        if(fh->file->isOpen()) {
            fh->file->close();
        }
        delete fh;
    }
}

FILE_STATUS ll_filesystem_file_seek(FILE_HANDLE* handle, uint32_t offset) {
    if(handle==NULL) {
        return F_INVALIDPARAM;
    }
    QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle);
    if(!fh->file->isOpen()) {
        return F_DISKERROR;
    }
    if(offset>=fh->file->size()) {
        return F_INVALIDPARAM;
    }

    if(fh->file->seek(offset)) {
        fh->fpos = offset;
        return F_OK;
    } else {
        return F_DISKERROR;
    }
}

FILE_STATUS ll_filesystem_file_read(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
    if(handle==NULL || buf==NULL) {
        return F_INVALIDPARAM;
    }
    QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle);
    if(!fh->file->isOpen()) {
        return F_DISKERROR;
    }
    if(!fh->file->isReadable()) {
        return F_EACCESS;
    }
    qint64 bytesRead = fh->file->read((char*)buf,size);
    if(bytesRead<0) {
        return F_DISKERROR;
    }
    fh->fpos+=bytesRead;
    if(bytesRead!=size) {
        return F_EOF;
    } else {
        return F_OK;
    }
}

FILE_STATUS ll_filesystem_file_write(FILE_HANDLE* handle, uint8_t* buf, uint32_t size) {
    if(handle==NULL) {
        return F_INVALIDPARAM;
    }
    QT_FILE_HANDLE* fh = static_cast<QT_FILE_HANDLE*>(handle);
    if(!fh->file->isOpen()) {
        return F_DISKERROR;
    }
    if(!fh->file->isWritable()) {
        return F_EACCESS;
    }
    qint64 bytesWritten = fh->file->write((char*)buf,size);
    if(bytesWritten<0) {
        return F_DISKERROR;
    }
    fh->fpos+=bytesWritten;
    if(bytesWritten!=size) {
        return F_EOF;
    } else {
        return F_OK;
    }
}
                                                                                                                                                                               


