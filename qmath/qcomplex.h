/*
 * @Author: luoqi 
 * @Date: 2022-01-26 09:38:25 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 09:38:52
 */

#ifndef _QCOMPLEX_H
#define _QCOMPLEX_H

typedef struct _qcomplex
{
    float re;
    float im;
} QComplex;

/* addition */
QComplex qcomplex_add(QComplex a, QComplex b);

/* subtraction */
QComplex qcomplex_sub(QComplex a, QComplex b);

/* multiplication */
QComplex qcomplex_mul(QComplex a, QComplex b);

/* division */
QComplex qcomplex_div(QComplex a, QComplex b);

#endif

