/*
 * @Author: luoqi 
 * @Date: 2022-02-12 17:08:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-08 22:21:28
 */

#include "qmem.h"

static char mheap[QMEM_HEAP_SIZE];

int qmem_init()
{
     for(int i = 0; i < QMEM_HEAP_SIZE; i++){
        if((i % QMEM_PAGE_SIZE) == 0){
            mheap[i] = 1; 
        }else{
            mheap[i] = 0;
        }
    }

    return 0;
}

void *qmalloc(unsigned int size)
{
    
    return QMEM_NULL;
}

void *qcalloc(unsigned int size)
{

}

void *qrealloc(void *ptr, unsigned int size)
{

}

int qadjust(void)
{

}

void qfree(void *ptr)
{
    
}
