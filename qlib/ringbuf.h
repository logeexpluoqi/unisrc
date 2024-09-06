/**
 * @ Author: luoqi
 * @ Create Time: 2023-06-27 14:20
 * @ Modified by: luoqi
 * @ Modified time: 2023-06-29 15:10
 * @ Description:
 */

#ifndef _RINGBUF_H
#define _RINGBUF_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

typedef struct 
{
    uint32_t wr_index;    // ring buffer write index
    uint32_t rd_index;    // ring buffer read index
    uint32_t used;        // ring buffer used size
    uint32_t sz;          // ring buffer size
    uint8_t  *buf;
} RingBuffer;

int rb_init(RingBuffer *rb, uint8_t *buf, uint32_t size);

uint32_t rb_write(RingBuffer *rb, uint8_t *data, uint32_t len);

uint32_t rb_write_force(RingBuffer *rb, uint8_t *data, uint32_t len);

uint32_t rb_read(RingBuffer *rb, uint8_t *rdata, uint32_t len);

uint32_t rb_used(RingBuffer *rb);

void rb_clear(RingBuffer *rb);

#ifdef __cplusplus
}
#endif

#endif
