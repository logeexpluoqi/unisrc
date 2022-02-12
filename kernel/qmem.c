/*
 * @Author: luoqi 
 * @Date: 2022-02-12 17:08:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-02-12 17:17:31
 */

#include "qmem.h"

static char qmem_heap[QMEM_HEAP_SIZE];

int qmem_init()
{
    int i;

    for(i = 0; i < QMEM_HEAP_SIZE; i++)
    {
        qmem_heap[i] = 0;
    }

    return 0;
}

void* qmem_malloc(unsigned int size)
{
    
    return QMEM_NULL;
}

void free(void *ptr)
{
    
}
