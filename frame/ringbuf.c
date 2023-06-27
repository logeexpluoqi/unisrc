/**
 * @ Author: luoqi
 * @ Create Time: 2023-06-27 14:17
 * @ Modified by: luoqi
 * @ Modified time: 2023-06-27 17:57
 * @ Description:
 */

#include "ringbuf.h"

static void *mem_cpy(void *dst, void *src, uint32_t len)
{
    char *d;
    const char *s;
    if((dst > (src + len)) || (dst < src)){
        d = dst;
        s = src;
        while(len --){
            *d++ = *s++;
        }
    }else{
        d = (char *)(dst + len - 1);
        s = (char *)(src + len - 1);
        while(len --){
            *d-- = *s--;
        }
    }
    return dst;
}

int ringbuf_init(RingBufObj *ring, uint8_t *buf, uint32_t size)
{
    ring->rindex = 0;
    ring->windex = 0;
    ring->bufsz = size;
    ring->buf = buf;
    return 0;
}

int ringbuf_write(RingBufObj *ring, uint8_t *data, uint32_t len)
{
    if((ring->windex + len) % ring->bufsz >= ring->rindex){
        return -1;
    }else if((ring->windex + len) >= ring->bufsz){

    }else{
        
    }
}

int ringbuf_read(RingBufObj *ring, uint8_t *rdata, uint32_t len)
{
    
}
