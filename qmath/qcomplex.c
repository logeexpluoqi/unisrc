/*
 * @Author: luoqi 
 * @Date: 2022-01-26 13:13:47 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 13:53:30
 */
#include <math.h>
#include "qcomplex.h"

QCmplx qcmplx_add(QCmplx a, QCmplx b)
{
    QCmplx c;
    c.re = a.re + b.re;
    c.im = a.im + b.im;

    return c;
}

QCmplx qcmplx_sub(QCmplx a, QCmplx b)
{
    QCmplx c;
    c.re = a.re - b.re;
    c.im = a.im - b.im;

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
    float denom = b.re * b.re + b.im * b.im;
    if(denom == 0.f){
        c.re = QCMPLX_INF;
        c.im = QCMPLX_INF;
        return c;
    }
    c.re = (a.re * b.re + a.im * b.im) / (b.re * b.re + b.im * b.im);
    c.im = (b.re * a.im - a.re * b.im) / (b.re * b.re + b.im * b.im);
    return c;
}

float qcmplx_arg(QCmplx a)
{
    return atanf(a.im / a.re);
}

float qcmplx_abs(QCmplx a)
{
    return sqrtf(a.re * a.re + a.im * a.im);
}
