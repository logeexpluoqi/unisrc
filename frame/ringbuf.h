/**
 * @ Author: luoqi
 * @ Create Time: 2023-06-27 14:20
 * @ Modified by: luoqi
 * @ Modified time: 2023-06-29 14:45
 * @ Description:
 */

#ifndef _RINGBUF_H
#define _RINGBUF_H

#include <stdint.h>

typedef struct 
{
    uint32_t head;    // ring buffer head index
    uint32_t tail;    // ring buffer tail index
    uint32_t msgsz;
    uint32_t bufsz;
    uint8_t  *buf;
} RingBufObj;

int ringbuf_init(RingBufObj *ring, uint8_t *buf, uint32_t size);

int ringbuf_write(RingBufObj *ring, uint8_t *data, uint32_t len);

int ringbuf_read(RingBufObj *ring, uint8_t *rdata, uint32_t len);

#endif
