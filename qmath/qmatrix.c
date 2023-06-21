/*
 * @Author: luoqi
 * @Date: 2021-09-23 14:49:05
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-12 17:56:52
 */

#if QMAT_USING_LIBC
#include <stdlib.h>
#include <string.h>
#endif
#include "qmatrix.h"

#define QMAT_ELEM(mat, i, j)   mat->elem[i * mat->col + j]

mat_s qmat_init(QMat *mat, mat_f *elem, mat_u row, mat_u col) 
{
    mat->row = row;
    mat->col = col;
    mat->elem = elem; 
    return QMAT_ERR_NONE;
}

#if QMAT_USING_LIBC
QMat *qmat_create(mat_u row, mat_u col)
{
    QMat *mat = (QMat *)malloc(sizeof(QMat));
    mat->row = row;
    mat->col = col;
    mat->elem = (mat_f *)malloc(row * col * sizeof(mat_f));
    memset(mat->elem, 0, row * col * sizeof(mat_f));
    return mat;
}

mat_u qmat_delete(QMat *mat)
{
    free(mat->elem);
    free(mat);
    return 0;
}
#endif

mat_s qmat_set(QMat *mat, mat_u row, mat_u col, mat_f num)
{
    QMAT_ELEM(mat, row, col) = num;
    return 0;
}

mat_s qmat_zeros(QMat *mat)
{
    mat_u i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            QMAT_ELEM(mat, i, j) = 0;
        }
    }
    return QMAT_ERR_NONE;
}

mat_s qmat_ones(QMat *mat)
{
    mat_u i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            QMAT_ELEM(mat, i, j) = 1;
        }
    }
    return QMAT_ERR_NONE;
}

mat_s qmat_eyes(QMat *mat)
{
    mat_u i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            if(i == j){
                QMAT_ELEM(mat, i, j) = 1;
            }else{
                QMAT_ELEM(mat, i, j) = 0;
            }
        }
    }
    return QMAT_ERR_NONE;
}

mat_f qmat_elem(QMat *mat, mat_u row, mat_u col) 
{
    if((mat->row >= row) && (mat->col >= col)){
        return QMAT_ELEM(mat, row, col);
    }else{
        return 0;
    }
}


mat_s qmat_mul(QMat *A, QMat *B, QMat *result) 
{
    mat_u i, j, m;
    mat_f tmp = 0;
    if((A->row == B->col) && (A->row == result->row) && (A->col == B->row) && (B->col == result->col)){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->row; j++){
                for(m = 0; m < A->col; m++){
                    tmp += QMAT_ELEM(A, i, m) * QMAT_ELEM(B, m, j);
                }
                QMAT_ELEM(result, i, j) = tmp;
                tmp = 0;
            }
        }
        return QMAT_ERR_NONE;
    }else{
        return QMAT_ERR_DIM;
    }
}

mat_s qmat_muln(QMat *A, mat_f b, QMat *result)
{
    mat_u i, j;

    if((A->row != result->row) || (A->col != result->col)){
        return QMAT_ERR_DIM;
    }
    for(i = 0; i < A->row; i++){
        for(j = 0; j < A->col; j++){
            QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) * b;
        }
    }
    return QMAT_ERR_NONE;
}

mat_s qmat_dotmul(QMat *A, QMat *B, QMat *result) 
{
    mat_u i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->col; j++){
                QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) * QMAT_ELEM(B, i, j);
            }
        }
        return QMAT_ERR_NONE;
    }else{
        return QMAT_ERR_DIM;
    }
}

mat_s qmat_add(QMat *A, QMat *B, QMat *result) 
{
    mat_u i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) + QMAT_ELEM(B, i, j);
            }
        }
        return QMAT_ERR_NONE;
    }else{
        return QMAT_ERR_DIM;
    }
}

mat_s qmat_addn(QMat *A, mat_f b, QMat *result)
{
    mat_u i, j;
    if((A->row != result->row) || (A->col != result->col)){
        return QMAT_ERR_DIM;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) + b;
            }
        }
        return QMAT_ERR_NONE;
    }
}

mat_s qmat_div(QMat *A, QMat *B, QMat *result) 
{
    return QMAT_ERR_NONE;
}

mat_s qmat_divn(QMat *A, mat_f b, QMat *result)
{
    mat_u i, j;
    if((A->row != result->row) || (A->col != result->col)){
        return QMAT_ERR_DIM;
    }else if(b == 0){
        return QMAT_ERR_CALC;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) / b;
            }
        }
        return QMAT_ERR_NONE;
    }
}

mat_s qmat_dotdiv(QMat *A, QMat *B, QMat *result)
{
    mat_u i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                if(QMAT_ELEM(B, i, j) != 0){
                    QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) / QMAT_ELEM(B, i, j);
                }else{
                    return QMAT_ERR_CALC;
                }
            }
        }
        return QMAT_ERR_NONE;
    }else{
        return QMAT_ERR_DIM;
    }
}

mat_s qmat_sub(QMat *A, QMat *B, QMat *result) 
{
    mat_u i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) - QMAT_ELEM(B, i, j);
            }
        }
        return QMAT_ERR_NONE;
    }else{
        return QMAT_ERR_DIM;
    }
}

mat_s qmat_subn(QMat *A, mat_f b, QMat *result) 
{
    mat_u i, j;
    if((A->row != result->row) || (A->col != result->col)){
        return QMAT_ERR_DIM;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) - b;
            }
        }
        return QMAT_ERR_NONE;
    }
}

mat_s qmat_inv(QMat *A, QMat *result)
{

    return QMAT_ERR_NONE;
}
