/**
 * @ Author: luoqi
 * @ Create Time: 2023-06-29 15:08
 * @ Modified by: luoqi
 * @ Modified time: 2023-06-29 18:18
 * @ Description:
 */

#include <stdlib.h>
#include "qdemo.h"
#include "../frame/ringbuf.h"
#include "../qshell/qsh.h"

#define BUF_SIZE 10

static RingBufObj ring;
static uint8_t buf[BUF_SIZE] = {0};

static int cmd_ringbuf_hdl(int argc, char **argv);

int demo_ringbuf_init()
{
    ringbuf_init(&ring, buf, BUF_SIZE);
    qcmd_export("rbuf", cmd_ringbuf_hdl, "@ wr<len>, rd<len>");
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
    int head = ringbuf_write(&ring, wbuf, len);
    free(wbuf);
    return head;
}

static int read(int len)
{
    uint8_t *rbuf = (uint8_t *)malloc(len);
    int tail = ringbuf_read(&ring, rbuf, len);
    for(int i = 0; i < len; i++){
        QSH(" %02X", rbuf[i]);
    }
    QSH("\r\n");
    free(rbuf);
    return tail;
}

int cmd_ringbuf_hdl(int argc, char **argv)
{
    if(argc == 1){
        QSH(" bufsz: %d, head: %d, tail: %d, msgsz: %d\r\n", ring.bufsz, ring.head, ring.tail, ring.msgsz);
        for(int i = 0; i < BUF_SIZE; i++){
            QSH(" %02X", buf[i]);
        }
        QSH("\r\n");
        return CMD_NO_ERR;
    }
    if(argc != 3){
        return CMD_PARAM_ERR;
    }
    int len = atoi(argv[2]);
    if(ISARG(argv[1], "wr")){
        int head = write(len);
        QSH(" head: %d\r\n", head);
    }else if(ISARG(argv[1], "rd")){
        int tail = read(len);
        QSH(" tail: %d\r\n", tail);
    }else {
        return CMD_PARAM_ERR;
    }
    return CMD_NO_ERR;
}
