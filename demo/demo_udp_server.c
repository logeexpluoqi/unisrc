/*
 * @Author: luoqi 
 * @Date: 2022-09-05 11:27:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-05 16:11:56
 */
#include <stdlib.h>
#include "demo_udp_server.h"
#include "../linuxbase/udp_server.h"
#include "../console/qsh.h"

static int cmd_udps_hdl(int argc, char **argv);

int demo_udp_server_init(void)
{
    qsh_export("udps", cmd_udps_hdl, "@ new<ip port>, del<fd>, send<data>, recv");

    return 0;
}


int cmd_udps_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return CMD_PARAM_LESS;
    }
    if(QSH_ISARG(argv[1], "new")){
        int fd = udp_server_creat(argv[2], atoi(argv[3]));
        if(fd < 0){
            QSH(" udp server creat faild \r\n");
        }else{
            QSH(" udp server addr: %s, port: %d, fd: %d\r\n", argv[2], atoi(argv[3]), fd);
        }
    }else if(QSH_ISARG(argv[1], "send")){
        struct sockaddr dst;
        int ret = udp_send(fd, argv[2], &dst, sizeof(dst));
        QSH(" send data fd: %d\r\n", ret);
    }else if(QSH_ISARG(argv[1], "recv")){

    }else if(QSH_ISARG(argv[1], "del")){
        int ret = udp_server_delete(atoi(argv[2]));
        QSH(" closed udp server fd: %d, issuccess: %d\r\n", atoi(argv[2]), ret);
    }else{
        return CMD_PARAM_ERR;
    }

    return 0;
}
