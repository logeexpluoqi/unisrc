/**
 * @ Author: luoqi
 * @ Create Time: 2024-07-17 11:39
 * @ Modified by: luoqi
 * @ Modified time: 2024-10-29 22:44
 * @ Description:
 */

#include "ringbuf.h"

static inline void *_memcpy(void *dst, const void *src, uint32_t len)
{
    char *d;
    const char *s;
    if(((uint32_t *)dst > ((uint32_t *)src + len)) || (dst < src)) {
        d = (char *)dst;
        s = (char *)src;
        while(len--) {
            *d++ = *s++;
        }
    } else {
        d = (char *)((uint32_t *)dst + len - 1);
        s = (char *)((uint32_t *)src + len - 1);
        while(len--) {
            *d-- = *s--;
        }
    }
    return dst;
}

int rb_init(RingBuffer *rb, uint8_t *buf, uint32_t size)
{
    rb->rd_index = 0;
    rb->wr_index = 0;
    rb->used = 0;
    rb->sz = size;
    rb->buf = buf;
    return 0;
}

uint32_t rb_write_force(RingBuffer *rb, const uint8_t *data, uint32_t len)
{
    uint32_t wr_len = 0;
    uint32_t wr_times = (len / rb->sz) + 1;
    uint32_t wrd = 0;

    for(uint32_t i = 0; i < wr_times; i++) {
        wr_len = (len > rb->sz) ? rb->sz : len;
        if(rb->wr_index + wr_len > rb->sz) {
            _memcpy(rb->buf + rb->wr_index, data + wrd, rb->sz - rb->wr_index);
            _memcpy(rb->buf, data + wrd + (rb->sz - rb->wr_index), wr_len - (rb->sz - rb->wr_index));
        } else {
            _memcpy(rb->buf + rb->wr_index, data + wrd, wr_len);
        }
        rb->wr_index = (rb->wr_index + wr_len) % rb->sz;
        rb->used = (rb->used + wr_len);
        len = len - wr_len;
        wrd = wrd + wr_len;
    }

    rb->used = (rb->used > rb->sz) ? rb->sz : rb->used;
    return wrd;
}

uint32_t rb_write(RingBuffer *rb, const uint8_t *data, uint32_t len)
{
    len = len > (rb->sz - rb->used) ? (rb->sz - rb->used) : len;
    return rb_write_force(rb, data, len);
}

uint32_t rb_read(RingBuffer *rb, uint8_t *rdata, uint32_t len)
{
    len = len > rb->used ? rb->used : len;

    if(rb->rd_index + len > rb->sz) {
        _memcpy(rdata, rb->buf + rb->rd_index, rb->sz - rb->rd_index);
        _memcpy(rdata + (rb->sz - rb->rd_index), rb->buf, len - (rb->sz - rb->rd_index));
    } else {
        _memcpy(rdata, rb->buf + rb->rd_index, len);
    }
    rb->used = rb->used - len;
    rb->rd_index = (rb->rd_index + len) % rb->sz;
    return len;
}

uint32_t rb_used(RingBuffer *rb)
{
    return rb->used;
}

void rb_clear(RingBuffer *rb)
{
    rb->wr_index = 0;
    rb->rd_index = 0;
    rb->used = 0;
}
