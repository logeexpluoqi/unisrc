/*
 * @Author: luoqi 
 * @Date: 2022-09-05 11:27:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-05 16:11:56
 */
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "qdemo.h"
#include "../linuxbase/mthread.h"
#include "../linuxbase/udp_server.h"
#include "../qshell/qterm.h"

static MThread udpserver;
static void *udpserver_hdl(void *);

static int cmd_udps_hdl(int argc, char **argv);

static struct sockaddr_in addr;
static int addr_len = sizeof(addr);
static int fd = -1;

int demo_udp_server_init(void)
{
    mthread_init(&udpserver, "udpserver", 10, 1000, udpserver_hdl, "udp server recv");
    mthread_start(&udpserver);
    qcmd_create("udps", cmd_udps_hdl, "port<port>, del<fd>, send<data>, recv");
    return 0;
}

void *udpserver_hdl(void *args)
{
    for(;;){
        mthread_task_begin(&udpserver);
        char buf[20] = {0};
        if(fd > 0){
            int ret = udp_server_recv(fd, buf, 20, (struct sockaddr*)&addr, &addr_len, 10000);
            if(ret > 0){
                QSH(" recv: %s\r\n", buf);
            }
        }
        mthread_task_end(&udpserver);
    }
}

int cmd_udps_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return -2;
    }
    if(ISARG(argv[1], "port")){
        fd = udp_server_creat(atoi(argv[2]));
        if(fd < 0){
            QSH(" #! udp server creat faild !\r\n");
        }else{
            QSH(" udp server port: %d, fd: %d\r\n", atoi(argv[2]), fd);
        }
    }else if(ISARG(argv[1], "send")){
        if(fd > 0){
            int ret = udp_server_send(fd, argv[2], strlen(argv[2]), (struct sockaddr*)&addr, (socklen_t)addr_len);
            QSH(" send data return: %d\r\n", ret);
        }else{
            QSH(" #! udp server not created !\r\n");
        }
    }else if(ISARG(argv[1], "recv")){
        char buf[10] = {0}; 
        int ret = udp_server_recv(fd, buf, 10, (struct sockaddr*)&addr, &addr_len, 5000);
        if(ret > 0){
            QSH(" recv: %s\r\n", buf);
        }else{
            QSH(" #! recv error !\r\n");
        }
    }else if(ISARG(argv[1], "del")){
        int ret = udp_server_delete(atoi(argv[2]));
        QSH(" closed udp server return: %d, issuccess: %d\r\n", atoi(argv[2]), ret);
    }else{
        return -1;
    }
    return 0;
}
