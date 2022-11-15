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

static CmdObj cmd_qmat;
static int cmd_qmat_hdl(int, char**);

void demo_qmath_init()
{
    qcmd_init(&cmd_qmat, "qmat", cmd_qmat_hdl, "@ add<A B>, addn<A, b>, mul<A B>, muln<A b>, sub<A B>, subn<A b>, div<A B>, divn<A b>, dotdiv<A b>, inv<A>");
    qcmd_add(&cmd_qmat);
}
int cmd_qmat_hdl(int argc, char **argv)
{
    if(argc > 1) {
        return CMD_PARAM_EXCEED;
    }
    QSH(" qmath test: \r\n");
    return 0;
}
