/*
 * @Author: luoqi 
 * @Date: 2022-01-26 17:08:38 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-12 09:42:29
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qdemo.h"
#include "../qmath/qmath.h"
#include "../qmath/qcomplex.h"
#include "../qshell/qsh.h"

static CmdObj cmd_qmat;
static int cmd_qmat_hdl(int, char **);

static CmdObj cmd_cmplx;
static int cmd_cmplx_hdl(int, char **);

static CmdObj cmd_m;
static int cmd_m_hdl(int, char **);

void demo_qmath_init()
{
    qcmd_init(&cmd_cmplx, "cmplx", cmd_cmplx_hdl, "add<a bi c di>, sub<a bi c di>, mul<a bi c di>, div<a bi c di>, abs<a bi>");
    qcmd_add(&cmd_cmplx);

    qcmd_init(&cmd_m, "m", cmd_m_hdl, "pow<a b>");
    qcmd_add(&cmd_m);
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

    return CMD_EOK;
}

static int cmd_m_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return CMD_PARAM_ERR;
    }
    if(ISARG(argv[1], "pow") && (argc == 4)) {
        float a = atof(argv[2]); 
        float b = atof(argv[3]); 
        QSH(" %f^%f = %f\r\n", a, b, qpow(a, b));
    }else{
        return CMD_PARAM_ERR;
    }
    return CMD_EOK;
}
