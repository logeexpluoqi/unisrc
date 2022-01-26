/*
 * @Author: luoqi 
 * @Date: 2022-01-26 17:08:38 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 17:23:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo_qmath.h"
#include "../qmath/qmath.h"
#include "../qmath/qcomplex.h"
#include "../console/qsh.h"

static QSH_CMD_CREAT(cmd_qmath);
unsigned char cmd_qmath_hdl(int argc, char* argv[]);

void demo_qmath_init()
{
    qsh_cmd_init(&cmd_qmath, "qmath", 0xff, cmd_qmath_hdl, "qmath test");
    qsh_cmd_add(&cmd_qmath);
}

unsigned char cmd_qmath_hdl(int argc, char* argv[])
{
    printf(" qmath test: \r\n");
    return 0;
}
