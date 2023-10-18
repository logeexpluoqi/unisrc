/**
 * @ Author: luoqi
 * @ Create Time: 2023-10-18 19:35
 * @ Modified by: luoqi
 * @ Modified time: 2023-10-18 19:42
 * @ Description:
 */

#ifdef __cplusplus
 extern "C"{
#endif

#ifndef _QLIBC_H
#define _QLIBC_H
#endif

#include <stdint.h>

#define QNULL   (void*)0

void *qmemcpy(void *dst, void *src, uint32_t len);

void *qmemset(void *dst, int c, uint32_t n);

uint32_t qstrlen(const char *s);

char *qstrcpy(char *dst, const char *src);

int qstrcmp(const char *s1, const char *s2);

#ifdef __cplusplus
 }
#endif
