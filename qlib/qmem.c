/*
 * @Author: luoqi
 * @Date: 2022-02-12 17:08:41
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-08 22:21:28
 */

#include "qmem.h"

static char heap[QMEM_HEAP_SIZE];

static void *_qmem_set(void *ptr, int val, qsize_t size)
{
    if(ptr == QMEM_NULL) {
        return QMEM_NULL;
    }
    int i;
    for(i = 0; i < size; i++) {
        ((char *)ptr)[i] = val;
    }
    return ptr;
}

static void *_qmem_cpy(void *dst, void *src, qsize_t size)
{
    if(dst == QMEM_NULL || src == QMEM_NULL) {
        return QMEM_NULL;
    }
    char *d;
    const char *s;
    if((dst > (src + size)) || (dst < src)) {
        d = dst;
        s = src;
        while(size--) {
            *d++ = *s++;
        }
    } else {
        d = (char *)(dst + size - 1);
        s = (char *)(src + size - 1);
        while(size--) {
            *d-- = *s--;
        }
    }
    return dst;
}

struct _mem_l {
    struct _mem_l *next;
};

static struct _mem_l mem_list = { .next = QMEM_NULL };


int qmem_init()
{
    _qmem_set(heap, 0, QMEM_HEAP_SIZE);
    
    return 0;
}

void *qmalloc(qsize_t size)
{

    return QMEM_NULL;
}

void *qcalloc(qsize_t size)
{

}

void *qrealloc(void *ptr, qsize_t size)
{

}

int qadjust(void)
{

}

void qfree(void *ptr)
{

}
