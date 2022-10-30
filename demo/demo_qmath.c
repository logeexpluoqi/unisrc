/*
 * @Author: luoqi 
 * @Date: 2022-01-26 17:08:38 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:21:58
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo_qmath.h"
#include "../qmath/qmath.h"
#include "../qmath/qcomplex.h"
#include "../qshell/qsh.h"

static CmdObj cmd_qmath;
static int cmd_qmath_hdl(int, char**);

void demo_qmath_init()
{
    qcmd_init(&cmd_qmath, "qmath", cmd_qmath_hdl, "@ test");
    qcmd_add(&cmd_qmath);
}
int cmd_qmath_hdl(int argc, char* argv[])
{
    if(argc > 1) {
        return CMD_PARAM_EXCEED;
    }
    QSH(" qmath test: \r\n");
    return 0;
}
