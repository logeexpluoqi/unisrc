/*
 * @Author: luoqi 
 * @Date: 2022-01-26 13:13:47 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 13:53:30
 */

#include "qcomplex.h"

QComplex qcomplex_add(QComplex a, QComplex b)
{
    QComplex c;
    c.re = a.re + b.re;
    c.im = b.im + b.im;

    return c;
}

QComplex qcomplex_sub(QComplex a, QComplex b)
{
    QComplex c;
    c.re = a.re - b.re;
    c.im = b.im - b.im;

    return c;
}

QComplex qcomplex_mul(QComplex a, QComplex b)
{
    QComplex c;
    c.re = a.re * b.re - a.im * b.im;
    c.im = a.re * b.im + b.re * a.im;

    return c;
}

QComplex qcomplex_div(QComplex a, QComplex b)
{
    QComplex c;
    c.re = (a.re * b.re + a.im * b.im) / (a.re * a.re + b.re * b.re);
    c.im = (a.im * b.re - a.re * b.im) / (a.re * a.re + b.re * b.re);

    return c;
}
