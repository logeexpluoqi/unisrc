/*
 * @Author: luoqi 
 * @Date: 2021-08-31 14:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-08-31 17:17:35
 */

#include "demo_qsh.h"
#include "../console/qsh.h"

unsigned char uni_handle(int argc, char* argv[])
{
    printf(">> qsh test\r\n");
    return 0;
}

int demo_qsh_init()
{
    qsh_cmd_add("uni", 0xff, uni_handle, "qsh test");
}
