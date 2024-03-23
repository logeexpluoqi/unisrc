/*
 * @Author: luoqi 
 * @Date: 2022-01-26 09:38:25 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 09:38:52
 */

#ifndef _QCOMPLEX_H
#define _QCOMPLEX_H

#ifdef __cplusplus
extern "C" {
#endif

#define QCMPLX_INF  (1.0f/0.0f)
#define QCMPLX_IINF (-1.0f/0.0f)
#define QCMPLX_NAN  (0.0f/0.0f)

typedef struct _qcomplex {
    float re;
    float im;
} QCmplx;

/* addition */
QCmplx qcmplx_add(QCmplx a, QCmplx b);

/* subtraction */
QCmplx qcmplx_sub(QCmplx a, QCmplx b);

/* multiplication */
QCmplx qcmplx_mul(QCmplx a, QCmplx b);

/* division */
QCmplx qcmplx_div(QCmplx a, QCmplx b);

float qcmplx_arg(QCmplx a);

float qcmplx_abs(QCmplx a);

#ifdef __cplusplus
}
#endif

#endif
