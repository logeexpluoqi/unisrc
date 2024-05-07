/**
 * @ Author: luoqi
 * @ Create Time: 2024-05-07 22:00
 * @ Modified by: luoqi
 * @ Modified time: 2024-05-07 22:01
 * @ Description:
 */

#include "qfs.h"
#include "qlibc.h"

int qfs_init(QFs *fs, uint32_t base_addr, uint32_t size, int (*write)(uint32_t, const void *, uint32_t), int(*read)(uint32_t, void *, uint32_t))
{
    fs->base_addr = base_addr;
    fs->offset = 0;
    fs->size = size;
    fs->write = write;
    fs->read = read;
    qlist_init(&fs->file_list);
    return 0;
}

int qfile_mount(QFs *fs, QFile *file, const char *name, uint32_t size)
{
    if(!qlist_isexist(&fs->file_list, &file->file_node)) {
        QList *_node;
        QFile *_file;
        QLIST_ITERATOR(_node, &fs->file_list) {
            _file = QLIST_ENTRY(_node, QFile, file_node);
            if(qstrcmp(_file->name, name) == 0) {
                return -1;
            } else {
                continue;
            }
        }
        file->addr = fs->base_addr + fs->offset;
        file->size = size;
        file->name = name;
        fs->offset += size;
        qlist_insert_after(&fs->file_list, &file->file_node);
        return 0;
    } else {
        return -1;
    }
}

int qfile_unmount(QFile *file)
{
    qlist_remove(&file->file_node);
    return 0;
}

int qfs_write(QFs *fs, const char *filename, uint32_t offset, const void *data, uint32_t size)
{
    QList *_node;
    QFile *_file;
    int ret = 0;
    QLIST_ITERATOR(_node, &fs->file_list) {
        _file = QLIST_ENTRY(_node, QFile, file_node);
        if(qstrcmp(_file->name, filename) == 0) {
            if(size > _file->size){
                ret = fs->write(_file->addr + offset, data, _file->size);
                if(ret < 0){
                    return ret - 1;
                }
            } else {
                ret = fs->write(_file->addr + offset, data, size);
                if(ret < 0){
                    return ret;
                }
            }
        } else {
            continue;
        }
    }
    return ret;
}

int qfs_read(QFs *fs, const char *filename, uint32_t offset, void *data, uint32_t size)
{
    QList *_node;
    QFile *_file;
    int ret = 0;
    QLIST_ITERATOR(_node, &fs->file_list) {
        _file = QLIST_ENTRY(_node, QFile, file_node);
        if(qstrcmp(_file->name, filename) == 0) {
            if(size > _file->size){
                ret = fs->read(_file->addr + offset, data, _file->size);
                if(ret < 0){
                    return ret - 1;
                }
            } else {
                ret = fs->read(_file->addr + offset, data, size);
                if(ret < 0){
                    return ret;
                }
            }
        } else {
            continue;
        }
    }
    return ret;
}
