/*
 * @Author: luoqi 
 * @Date: 2022-02-12 17:08:10 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-02-12 17:48:45
 */

#ifndef _QMEM_H
#define _QMEM_H

#ifdef __cplusplus
 extern "C" {
#endif

#define QMEM_HEAP_SIZE    1024 * 1024
#define QMEM_NULL         (void*)0

int qmem_init(void);

void* qmem_malloc(unsigned int size);

void qmem_free(void *ptr);

#ifdef __cplusplus
 }
#endif

#endif

