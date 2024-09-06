/**
 * @ Author: luoqi
 * @ Create Time: 2023-06-29 15:08
 * @ Modified by: luoqi
 * @ Modified time: 2023-12-07 21:42
 * @ Description:
 */

#include <stdlib.h>
#include "qdemo.h"
#include "../qlib/ringbuf.h"
#include "../qshell/qterm.h"

#define BUF_SIZE 10

static RingBuffer rb;
static uint8_t buf[BUF_SIZE] = {0};

static int cmd_ringbuf_hdl(int argc, char **argv);

int demo_ringbuf_init()
{
    rb_init(&rb, buf, BUF_SIZE);
    qcmd_create("rbuf", cmd_ringbuf_hdl, "wr<len>, rd<len>");
    return 0;
}

static int write(int len)
{
    static int w = 1;
    uint8_t *wbuf = (uint8_t *)malloc(len);
    for(int i = 0; i < len; i++){
        *(wbuf + i) = w;
    }
    if(w++ > 0xff){
        w = 1;
    };
    int wr_index = rb_write(&rb, wbuf, len);
    QSH(" sz: %d, wr_index: %d, rd_index: %d, used: %d\r\n", rb.sz, rb.wr_index, rb.rd_index, rb.used);
    free(wbuf);
    return wr_index;
}

static int read(int len)
{
    uint8_t *rbuf = (uint8_t *)malloc(len);
    int rd_index = rb_read(&rb, rbuf, len);
    QSH(" sz: %d, wr_index: %d, rd_index: %d, used: %d\r\n", rb.sz, rb.wr_index, rb.rd_index, rb.used);
    for(int i = 0; i < len; i++){
        QSH(" %02X", *(rbuf + i));
    }
    QSH("\r\n");
    free(rbuf);
    return rd_index;
}

int cmd_ringbuf_hdl(int argc, char **argv)
{
    if(argc == 1){
        QSH(" sz: %d, wr_index: %d, rd_index: %d, used: %d\r\n", rb.sz, rb.wr_index, rb.rd_index, rb.used);
        for(int i = 0; i < BUF_SIZE; i++){
            QSH(" %02X", buf[i]);
        }
        QSH("\r\n");
        return 0;
    }
    if(argc != 3){
        return -1;
    }
    int len = atoi(argv[2]);
    if(ISARG(argv[1], "wr")){
        int wr_index = write(len);
        QSH(" wr_index: %d\r\n", wr_index);
    }else if(ISARG(argv[1], "rd")){
        int rd_index = read(len);
        QSH(" rd_index: %d\r\n", rd_index);
    }else {
        return -1;
    }
    return 0;
}
