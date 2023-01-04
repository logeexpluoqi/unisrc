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
static int cmd_qmat_hdl(int, char **);

static CmdObj cmd_cmplx;
static int cmd_cmplx_hdl(int, char **);

void demo_qmath_init()
{
    qcmd_init(&cmd_qmat, "qmat", cmd_qmat_hdl, "@ add, addn, mul, muln, sub, subn, div, divn, dotdiv, inv");
    qcmd_add(&cmd_qmat);

    qcmd_init(&cmd_cmplx, "cmplx", cmd_cmplx_hdl, "@ add<a bi c di>, sub<a bi c di>, mul<a bi c di>, div<a bi c di>, abs<a bi>");
    qcmd_add(&cmd_cmplx);
}

int cmd_qmat_hdl(int argc, char **argv)
{
    if(argc > 1) {
        return CMD_PARAM_EXCEED;
    }
    QSH(" qmath test: \r\n");
    return CMD_NO_ERR;
}

int cmd_cmplx_hdl(int argc, char **argv)
{
    if(argc == 6) {
        QCmplx x, y, c;
        x.re = atof(argv[2]);
        x.im = atof(argv[3]);
        y.re = atof(argv[4]);
        y.im = atof(argv[5]);
        if (ISARG(argv[1], "add")) {
            c = qcmplx_add(x, y);
            QSH(" (%f+%fi) + (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else if (ISARG(argv[1], "sub")) {
            c = qcmplx_sub(x, y);
            QSH(" (%f+%fi) - (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else if (ISARG(argv[1], "mul")) {
            c = qcmplx_mul(x, y);
            QSH(" (%f+%fi) x (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else if (ISARG(argv[1], "div")) {
            c = qcmplx_div(x, y);
            QSH(" (%f+%fi) / (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else{
            return CMD_PARAM_ERR;
        }
    }else if(argc == 4){
        QCmplx a;
        a.re = atof(argv[2]);
        a.im = atof(argv[3]);
        if(ISARG(argv[1], "abs")){
            QSH(" abs(%f+%fi) = %f\r\n", a.re, a.im, qcmplx_abs(a));
        }else if(ISARG(argv[1], "arg")){
            QSH(" arg(%f+%fi) = %f\r\n", a.re, a.im, qcmplx_arg(a));
        }
    }else{
        return CMD_PARAM_ERR;
    }

    return CMD_NO_ERR;
}
