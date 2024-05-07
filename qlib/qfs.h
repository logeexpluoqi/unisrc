/**
 * @ Author: luoqi
 * @ Create Time: 2024-05-07 22:00
 * @ Modified by: luoqi
 * @ Modified time: 2024-05-07 22:01
 * @ Description:
 */

#ifndef _QFS_H
#define _QFS_H

#include <stdint.h>
#include "qlist.h"

typedef struct {
    uint32_t base_addr;
    uint32_t offset;
    uint32_t size;
    int (*write)(uint32_t offset, const void *data, uint32_t size);
    int (*read)(uint32_t offset, void *data, uint32_t size);
    QList file_list;
} QFs;

typedef struct {
    const char *name;
    uint32_t addr;
    uint32_t size;
    QList file_node;
} QFile;

int qfs_init(QFs *fs, uint32_t base_addr, uint32_t size, int (*write)(uint32_t, const void*, uint32_t), int(*read)(uint32_t, void *, uint32_t));

int qfile_mount(QFs *fs, QFile *file, const char *name, uint32_t size);

int qfile_unmount(QFile *file);

int qfs_write(QFs *fs, const char *filename, uint32_t offset, const void *data, uint32_t size);

int qfs_read(QFs *fs, const char *filename, uint32_t offset, void *data, uint32_t size);

#endif
