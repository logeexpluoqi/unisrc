/**
 * @ Author: luoqi
 * @ Create Time: 2023-10-18 19:35
 * @ Modified by: luoqi
 * @ Modified time: 2023-10-18 19:43
 * @ Description:
 */

#include "qlibc.h"

void *qmemcpy(void *dst, void *src, uint32_t len)
{
    char *d;
    const char *s;
    if((dst > (src + len)) || (dst < src)) {
        d = dst;
        s = src;
        while(len--) {
            *d++ = *s++;
        }
    } else {
        d = (char *)(dst + len - 1);
        s = (char *)(src + len - 1);
        while(len--) {
            *d-- = *s--;
        }
    }
    return dst;
}

void *qmemset(void *dest, int c, uint32_t n)
{
    if((dest == QNULL) || n < 0) {
        return QNULL;
    } else {
        char *pdest = (char *)dest;
        while(n--) {
            *pdest++ = c;
        }
        return dest;
    }
}

uint32_t qstrlen(const char *s)
{
    uint32_t len = 0;
    while(*s++ != '\0') {
        len++;
    }
    return len;
}

char *qstrcpy(char *dest, const char *src)
{
    if((dest == QNULL) || (src == QNULL)) {
        return QNULL;
    }
    char *addr = dest;
    while((*dest++ = *src++) != '\0');
    return addr;
}

int qstrcmp(const char *s1, const char *s2)
{
    uint32_t i = 0;

    while((*(s1 + i) != '\0') || (*(s2 + i) != '\0')) {
        if(*(s1 + i) != *(s2 + i)) {
            return 1;
        } else {
            i++;
        }
    }
    return 0;
}
