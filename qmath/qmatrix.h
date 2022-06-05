/*
 * @Author: luoqi 
 * @Date: 2021-09-23 14:48:27 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:21:41
 */

#ifndef _QMATRIX_H
#define _QMATRIX_H

#ifdef __cplusplus
 extern "C" {
#endif

#define QMAT_USING_LIBC     1

typedef enum _qmat_err
{
    QMAT_ERR_NONE,
    QMAT_ERR_DIM,
    QMAT_ERR_CALC,
}QMatErr;

typedef struct _qmatrix
{
    unsigned int row;
    unsigned int col;
    float **elem;
    QMatErr err;
} QMat;

#ifdef QMAT_USING_LIBC
/* creat a matrix */
QMat qmatc(int row, int col);

/* zeros */
QMat qmatc_zeros(int row, int col);

/* ones */
QMat qmatc_ones(int row, int col);

/* eyes */
QMat qmatc_eyes(int row, int col);

/* A * B */
QMat qmatc_mul(QMat A, QMat B);

/* A * b */
QMat qmatc_muln(QMat A, float b);

/* A .* B */
QMat qmatc_dotmul(QMat A, QMat B);

/* A + B */
QMat qmatc_add(QMat A, QMat B);

/* A + b */
QMat qmatc_addn(QMat A, float b);

/* A - B */
QMat qmatc_sub(QMat A, QMat B);

/* A - b */
QMat qmatc_subn(QMat A, float b);

/* A / b */
QMat qmatc_div(QMat A, QMat B);

/* A / b */
QMat qmatc_divn(QMat A, float b);

/* A ./ b */
QMat qmatc_dotdiv(QMat A, QMat B);

/* 1 / A */
QMat qmatc_inv(QMat A);

#endif

void qmat_init(QMat *mat, float **elem, int row, int col);

void qmat_zeros(QMat *mat);

void qmat_ones(QMat *mat);

void qmat_eyes(QMat *mat);

float qmat_elem(QMat *mat, int row, int col);

/* A + B */
int qmat_mul(QMat *A, QMat *B, QMat *result);

/* A + b */
int qmat_muln(QMat *A, float b, QMat *result);

/* A .* B */
int qmat_dotmul(QMat *A, QMat *B, QMat *result);

/* A + B */
int qmat_add(QMat *A, QMat *B, QMat *result);

/* A + b */
int qmat_addn(QMat *A, float b, QMat *result);

/* A / B */
int qmat_div(QMat *A, QMat *B, QMat *result);

/* A / b */
int qmat_divn(QMat *A, float b, QMat *result);

/* A ./ B */
int qmat_dotdiv(QMat *A, QMat *B, QMat *result);

/* A - B */
int qmat_sub(QMat *A, QMat *B, QMat *result);

/* A - b */
int qmat_subn(QMat *A, float b, QMat *result);

int qmat_inv(QMat *A, QMat *result);

#ifdef __cplusplus
 }
#endif

#endif
