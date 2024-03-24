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

typedef unsigned int    qsize_t;

#ifndef QMEM_HEAP_SIZE
#define QMEM_HEAP_SIZE  (1024ul * 64ul)
#endif

#define QMEM_NULL       (void*)0

int qmem_init(void);

void *qmalloc(qsize_t size);

void *qcalloc(qsize_t size);

void *qrealloc(void *ptr, qsize_t size);

void qfree(void *ptr);

int qadjust(void);

#ifdef __cplusplus
 }
#endif

#endif

