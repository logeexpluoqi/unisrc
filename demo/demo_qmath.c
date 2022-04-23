/*
 * @Author: luoqi 
 * @Date: 2022-01-26 17:08:38 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 17:40:56
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "demo_qmath.h"
#include "../qmath/qmath.h"
#include "../qmath/qcomplex.h"
#include "../console/qsh.h"

static QshCmd cmd_qmath;
unsigned char cmd_qmath_hdl(int, char**);

void demo_qmath_init()
{
    qsh_cmd_init(&cmd_qmath, "qmath", cmd_qmath_hdl, "qmath test");
    qsh_cmd_add(&cmd_qmath);
}

unsigned char cmd_qmath_hdl(int argc, char* argv[])
{
    if(argc > 1)
    {
        QSH(" #! parameter error !\r\n");
        return 1;
    }
    printf(" qmath test: \r\n");
    return 0;
}
