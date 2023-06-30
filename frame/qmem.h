/*
 * @Author: luoqi 
 * @Date: 2022-02-12 17:08:10 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-08 22:21:45
 */

#ifndef _QMEM_H
#define _QMEM_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef unsigned int    size_m;

#define QMEM_MAX_NEW_64K

#define QMEM_1KB        1024

#define QMEM_HEAP_SIZE  (1024 * QMEM_1KB)  // byte

#define QMEM_NULL       (void*)0

int qmem_init(void);

void *qmalloc(size_m size);

void *qcalloc(size_m size);

void *qrealloc(void *ptr, size_m size);

void qfree(void *ptr);

int qadjust(void);

#ifdef __cplusplus
 }
#endif

#endif

