/*
 * @Author: luoqi 
 * @Date: 2021-09-23 14:48:27 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-12 15:18:04
 */

#ifndef _QMATRIX_H
#define _QMATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#define QMAT_USING_LIBC   1

typedef float           mat_f;
typedef int             mat_s;
typedef unsigned int    mat_u;

typedef enum _qmat_err {
    QMAT_ERR_DIM = -1,
    QMAT_ERR_NONE = 0,
    QMAT_ERR_CALC = 1,
} QMatErr;

typedef struct _qmatrix {
    mat_u row;
    mat_u col;
    mat_f *elem;
} QMat;

#if QMAT_USING_LIBC

QMat *qmat_create(mat_u row, mat_u col);

mat_u qmat_delete(QMat *mat);

#endif

mat_s qmat_init(QMat *mat, mat_f **elem, mat_u row, mat_u col);

mat_s qmat_zeros(QMat *mat);

mat_s qmat_ones(QMat *mat);

mat_s qmat_eyes(QMat *mat);

mat_s qmat_set(QMat *mat, mat_u row, mat_u col, mat_f num);

mat_f qmat_elem(QMat *mat, mat_u row, mat_u col);

/* A + B */
mat_s qmat_mul(QMat *A, QMat *B, QMat *result);

/* A + b */
mat_s qmat_muln(QMat *A, mat_f b, QMat *result);

/* A .* B */
mat_s qmat_dotmul(QMat *A, QMat *B, QMat *result);

/* A + B */
mat_s qmat_add(QMat *A, QMat *B, QMat *result);

/* A + b */
mat_s qmat_addn(QMat *A, mat_f b, QMat *result);

/* A / B */
mat_s qmat_div(QMat *A, QMat *B, QMat *result);

/* A / b */
mat_s qmat_divn(QMat *A, mat_f b, QMat *result);

/* A ./ B */
mat_s qmat_dotdiv(QMat *A, QMat *B, QMat *result);

/* A - B */
mat_s qmat_sub(QMat *A, QMat *B, QMat *result);

/* A - b */
mat_s qmat_subn(QMat *A, mat_f b, QMat *result);

/* 1 / A */
mat_s qmat_inv(QMat *A, QMat *result);

#ifdef __cplusplus
}
#endif

#endif
