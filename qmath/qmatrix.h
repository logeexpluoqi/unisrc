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

#define QMAT_USING_LIBC

#ifdef QMAT_USING_LIBC
#ifndef QMAT_USING_DMEM
#define QMAT_USING_DMEM   1
#endif
#else
#ifndef QMAT_USING_DMEM
#define QMAT_USING_DMEM   0
#endif
#endif

typedef float           matf;
typedef int             mats;
typedef unsigned int    matu;

#define QMAT_INF         (1.0f / 0.0f)
#define QMAT_IINF        (-1.0f / 0.0f)
#define QMAT_NAN         (0.0f / 0.0f)

typedef enum _qmat_err {
    QMAT_ERR_CALC = -2,
    QMAT_ERR_DIM = -1,
    QMAT_ERR_NONE = 0,
} QMatErr;

typedef struct _qmatrix {
    matu row;
    matu col;
    matf *elem;
} QMat;

#if QMAT_USING_DMEM

QMat *qmat_create(matu row, matu col);

matu qmat_delete(QMat *mat);

#endif

#define QMAT_MEM(mem, row, col)     matf mem[row * col] 

mats qmat_init(QMat *mat, matf *elem, matu row, matu col);

mats qmat_zeros(QMat *mat);

mats qmat_ones(QMat *mat);

mats qmat_eyes(QMat *mat);

/* swap mat raw1 and raw2 */
mats qmat_raw_trans(QMat *mat, matu raw1, matu raw2);

/* swap mat col1 and col2 */
mats qmat_col_trans(QMat *mat, matu col1, matu col2);

mats qmat_isequal(QMat *A, QMat *B);

mats qmat_copy(QMat *src, QMat *dst);

mats qmat_set(QMat *mat, matu row, matu col, matf num);

matf qmat_elem(QMat *mat, matu row, matu col);

/* A + B */
mats qmat_mul(QMat *A, QMat *B, QMat *result);

/* A + b */
mats qmat_muln(QMat *A, matf b, QMat *result);

/* A .* B */
mats qmat_dotmul(QMat *A, QMat *B, QMat *result);

/* A + B */
mats qmat_add(QMat *A, QMat *B, QMat *result);

/* A + b */
mats qmat_addn(QMat *A, matf b, QMat *result);

/* A / B */
mats qmat_div(QMat *A, QMat *B, QMat *result);

/* A / b */
mats qmat_divn(QMat *A, matf b, QMat *result);

/* A ./ B */
mats qmat_dotdiv(QMat *A, QMat *B, QMat *result);

/* A - B */
mats qmat_sub(QMat *A, QMat *B, QMat *result);

/* A - b */
mats qmat_subn(QMat *A, matf b, QMat *result);

/* LUP decompose */
mats qmat_lup(QMat *A, QMat *L, QMat *U);

/* 1 / A */
mats qmat_inv(QMat *A, QMat *inv);

#ifdef __cplusplus
}
#endif

#endif
