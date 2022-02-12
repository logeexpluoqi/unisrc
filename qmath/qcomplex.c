/*
 * @Author: luoqi 
 * @Date: 2022-01-26 13:13:47 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 13:53:30
 */

#include "qcomplex.h"

QCmplx qcmplx_add(QCmplx a, QCmplx b)
{
    QCmplx c;
    c.re = a.re + b.re;
    c.im = b.im + b.im;

    return c;
}

QCmplx qcmplx_sub(QCmplx a, QCmplx b)
{
    QCmplx c;
    c.re = a.re - b.re;
    c.im = b.im - b.im;

    return c;
}

QCmplx qcmplx_mul(QCmplx a, QCmplx b)
{
    QCmplx c;
    c.re = a.re * b.re - a.im * b.im;
    c.im = a.re * b.im + b.re * a.im;

    return c;
}

QCmplx qcmplx_div(QCmplx a, QCmplx b)
{
    QCmplx c;
    c.re = (a.re * b.re + a.im * b.im) / (a.re * a.re + b.re * b.re);
    c.im = (a.im * b.re - a.re * b.im) / (a.re * a.re + b.re * b.re);

    return c;
}
