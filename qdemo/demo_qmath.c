/*
 * @Author: luoqi
 * @Date: 2022-01-26 17:08:38
 * @ Modified by: luoqi
 * @ Modified time: 2024-06-17 22:19
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qdemo.h"
#include "../qmath/qmath.h"
#include "../qmath/qcomplex.h"
#include "../qmath/qtrigf.h"
#include "../qshell/qterm.h"

static QCmdObj cmd_qmat;
static int cmd_qmat_hdl(int, char **);

static QCmdObj cmd_cmplx;
static int cmd_cmplx_hdl(int, char **);

static QCmdObj cmd_m;
static int cmd_m_hdl(int, char **);

void demo_qmath_init()
{
    qterm_attatch(&cmd_cmplx, "cmplx", cmd_cmplx_hdl, "add<a bi c di>, sub<a bi c di>, mul<a bi c di>, div<a bi c di>, abs<a bi>");

    qterm_attatch(&cmd_m, "m", cmd_m_hdl, "pow<a b>, sind<x>, cosd<x>, atan2<y x>");
}

int cmd_cmplx_hdl(int argc, char **argv)
{
    if(argc == 6) {
        QCmplx x, y, c;
        x.re = atof(argv[2]);
        x.im = atof(argv[3]);
        y.re = atof(argv[4]);
        y.im = atof(argv[5]);
        if(ISARG(argv[1], "add")) {
            c = qcmplx_add(x, y);
            QSH(" (%f+%fi) + (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else if(ISARG(argv[1], "sub")) {
            c = qcmplx_sub(x, y);
            QSH(" (%f+%fi) - (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else if(ISARG(argv[1], "mul")) {
            c = qcmplx_mul(x, y);
            QSH(" (%f+%fi) x (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else if(ISARG(argv[1], "div")) {
            c = qcmplx_div(x, y);
            QSH(" (%f+%fi) / (%f+%fi) = (%f+%fi)\r\n", x.re, x.im, y.re, y.im, c.re, c.im);
        } else {
            return -1;
        }
    } else if(argc == 4) {
        QCmplx a;
        a.re = atof(argv[2]);
        a.im = atof(argv[3]);
        if(ISARG(argv[1], "abs")) {
            QSH(" abs(%f+%fi) = %f\r\n", a.re, a.im, qcmplx_abs(a));
        } else if(ISARG(argv[1], "arg")) {
            QSH(" arg(%f+%fi) = %f\r\n", a.re, a.im, qcmplx_arg(a));
        }
    } else {
        return -1;
    }

    return 0;
}

static int cmd_m_hdl(int argc, char **argv)
{
    if(argc <= 1) {
        return -1;
    }
    if(ISARG(argv[1], "pow") && (argc == 4)) {
        float a = atof(argv[2]);
        float b = atof(argv[3]);
        QSH(" %f^%f = %f\r\n", a, b, qpow(a, b));
    } else if(ISARG(argv[1], "sind") && (argc == 3)) {
        float x = atof(argv[2]);
        QSH(" sind(%f) = %f\r\n", x, qfsind(x));
    } else if(ISARG(argv[1], "cosd") && (argc == 3)) {
        float x = atof(argv[2]);
        QSH(" cosd(%f) = %f\r\n", x, qfcosd(x));
    } else if(ISARG(argv[1], "atan2") && (argc == 4)) {
        float y = atof(argv[2]);
        float x = atof(argv[3]);
        QSH(" atan2(%f, %f) = %f\r\n", y, x, qfatan2(y, x));
    } else {
        return -1;
    }
    return 0;
}
