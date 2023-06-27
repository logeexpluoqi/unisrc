/**
 * @ Author: luoqi
 * @ Create Time: 2023-06-27 14:20
 * @ Modified by: luoqi
 * @ Modified time: 2023-06-27 17:15
 * @ Description:
 */

#ifndef _RINGBUF_H
#define _RINGBUF_H

#include <stdint.h>

typedef struct 
{
    uint32_t windex;    // write index
    uint32_t rindex;    // read index
    uint32_t bufsz;
    uint8_t  *buf;
} RingBufObj;

int ringbuf_init(RingBufObj *ring, uint8_t *buf, uint32_t size);

int ringbuf_write(RingBufObj *ring, uint8_t *data, uint32_t len);

int ringbuf_read(RingBufObj *ring, uint8_t *rdata, uint32_t len);

#endif

