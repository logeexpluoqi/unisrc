/*
 * @Author: luoqi 
 * @Date: 2021-08-31 14:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:42:58
 */

#include "demo_qsh.h"
#include "../console/qsh.h"

static QSH_CMD_CREAT(qcmd1);
static QSH_CMD_CREAT(qcmd2);

unsigned char q1_handle(int argc, char* argv[])
{
    if(argc > 1)
    {
        QSH_PRINTF(" #! parameter error !\r\n");
        return 1;
    }
    printf(">> qsh test 1\r\n");
    return 0;
}

unsigned char q2_handle(int argc, char* argv[])
{
    if(argc > 1)
    {
        QSH_PRINTF(" #! parameter error !\r\n");
        return 1;
    }
    printf(">> qsh test 2\r\n");
    return 0;
}

int demo_qsh_init()
{
    qsh_cmd_init(&qcmd1, "qcmd1", 0xff, q1_handle, "qsh test 1");
    qsh_cmd_add(&qcmd1);
    
    qsh_cmd_init(&qcmd2, "qcmd2", 0xff, q2_handle, "qsh test 2");
    qsh_cmd_add(&qcmd2);
    return 0;
}
