/**
 * @ Author: luoqi
 * @ Create Time: 2023-06-27 14:17
 * @ Modified by: luoqi
 * @ Modified time: 2023-06-29 18:10
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
    ring->tail = 0;
    ring->head = 0;
    ring->msgsz = 0;
    ring->bufsz = size;
    ring->buf = buf;
    return 0;
}

int ringbuf_write(RingBufObj *ring, uint8_t *data, uint32_t len)
{
    if(ring->head >= ring->tail){
        ring->msgsz = ring->head - ring->tail;
    }else{
        ring->msgsz = (ring->bufsz - ring->tail) + ring->head;
    }
    
    if((ring->bufsz - ring->msgsz) < len){
        return -1;
    }else if((ring->head + len) > ring->bufsz){
        mem_cpy(ring->buf + ring->head, data, ring->bufsz - ring->head);
        mem_cpy(ring->buf, data + (ring->bufsz - ring->head), len - (ring->bufsz - ring->head));
    }else{
        mem_cpy(ring->buf + ring->head, data, len);
    }
    ring->head = (ring->head + len) % ring->bufsz;
    return ring->head;
}

int ringbuf_read(RingBufObj *ring, uint8_t *rdata, uint32_t len)
{
    if(ring->msgsz < len){
        return -1;
    }else if(ring->head < ring->tail){
        mem_cpy(rdata, ring->buf + ring->tail, ring->bufsz - ring->tail);
        mem_cpy(rdata + (ring->bufsz - ring->tail), ring->buf, len - (ring->bufsz - ring->tail));      
    }else{
        mem_cpy(rdata, ring->buf + ring->tail, len);
    }
    ring->tail = (ring->tail + len) % ring->bufsz;
    return ring->tail;
}
