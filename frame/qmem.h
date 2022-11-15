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

#define QMEME_1KB         1024

#define QMEM_HEAP_SIZE    (1024 * QMEME_1KB)  // byte
#define QMEM_PAGE_SIZE    128
#define QMEM_NULL         (void*)0

int qmem_init(void);

void *qmalloc(unsigned int size);

void *qcalloc(unsigned int size);

void *qrealloc(void *ptr, unsigned int size);

void qfree(void *ptr);

int qadjust(void);

#ifdef __cplusplus
 }
#endif

#endif

