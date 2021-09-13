/*
 * @Author: luoqi 
 * @Date: 2021-08-31 14:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-08-31 17:17:35
 */

#include "demo_qsh.h"
#include "../console/qsh.h"

static QshCmd qcmd1;
static QshCmd qcmd2;

unsigned char q1_handle(int argc, char* argv[])
{
    printf(">> qsh test 1\r\n");
    return 0;
}

unsigned char q2_handle(int argc, char* argv[])
{
    printf(">> qsh test 2\r\n");
    return 0;
}

int demo_qsh_init()
{
    qsh_cmd_creat(&qcmd1, "qcmd1", 0xff, q1_handle, " qsh test 1");
    qsh_cmd_creat(&qcmd2, "qcmd2", 0xff, q2_handle, " qsh test 2");
    return 0;
}
