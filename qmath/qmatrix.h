/*
 * @Author: luoqi 
 * @Date: 2021-09-23 14:48:27 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:21:41
 */

#ifndef _QMATRIX_H
#define _QMATRIX_H

typedef struct _qmatrix
{
    unsigned int row;
    unsigned int col;
    float *elem[];
} QMatrix;

void qmatrix_init(QMatrix *mat, float *elem[], unsigned int row, unsigned int col);

float qmatrix_elem(QMatrix *mat, unsigned int row, unsigned int col);

QMatrix qmatrix_mul(QMatrix *A, QMatrix *B);

QMatrix qmatrix_dotmul(QMatrix *A, QMatrix *B);

QMatrix qmatrix_add(QMatrix *A, QMatrix *B);

QMatrix qmatrix_div(QMatrix *A, QMatrix *B);

QMatrix qmatrix_dotdiv(QMatrix *A, QMatrix *B);

QMatrix qmatrix_sub(QMatrix *A, QMatrix *B);

#endif
