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

typedef enum _qmat_err
{
    QMAT_ERR_DIM  = -1,
    QMAT_ERR_NONE = 0,
    QMAT_ERR_CALC = 1,
}QMatErr;

typedef struct _qmatrix
{
    unsigned int row;
    unsigned int col;
    float **elem;
    QMatErr err;
} QMat;

int qmat_init(QMat *mat, float **elem, int row, int col);

int qmat_zeros(QMat *mat);

int qmat_ones(QMat *mat);

int qmat_eyes(QMat *mat);

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

/* 1 / A */
int qmat_inv(QMat *A, QMat *result);

#ifdef __cplusplus
 }
#endif

#endif
