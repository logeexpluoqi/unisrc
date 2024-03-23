/*
 * @Author: luoqi
 * @Date: 2021-09-23 14:49:05
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-12 17:56:52
 */

#include "qmatrix.h"
#if QMAT_USING_DMEM
#include <stdlib.h>
#include <string.h>
#endif

#define QMAT_ELEM(mat, i, j)   mat->elem[i * mat->col + j]

#define QMAT_DIM_ISEQUAL(mat_a, mat_b)  ((mat_a)->row == (mat_b)->row && (mat_a)->col == (mat_b)->col)

#define QMAT_DIM_ISMATCH(mat_a, mat_b)  ((mat_a)->row == (mat_b)->col && (mat_a)->col == (mat_b)->row)

mats qmat_init(QMat *mat, matf *elem, matu row, matu col) 
{
    mat->row = row;
    mat->col = col;
    mat->elem = elem; 
    return QMAT_ERR_NONE;
}

#if QMAT_USING_DMEM
QMat *qmat_create(matu row, matu col)
{
    QMat *mat = (QMat *)malloc(sizeof(QMat));
    mat->row = row;
    mat->col = col;
    mat->elem = (matf *)malloc(row * col * sizeof(matf));
    memset(mat->elem, 0, row * col * sizeof(matf));
    return mat;
}

matu qmat_delete(QMat *mat)
{
    free(mat->elem);
    free(mat);
    return 0;
}
#endif

mats qmat_set(QMat *mat, matu row, matu col, matf num)
{
    QMAT_ELEM(mat, row, col) = num;
    return 0;
}

mats qmat_zeros(QMat *mat)
{
    matu i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            QMAT_ELEM(mat, i, j) = 0;
        }
    }
    return QMAT_ERR_NONE;
}

mats qmat_ones(QMat *mat)
{
    matu i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            QMAT_ELEM(mat, i, j) = 1;
        }
    }
    return QMAT_ERR_NONE;
}

mats qmat_eyes(QMat *mat)
{
    matu i, j;
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

mats qmat_isequal(QMat *A, QMat *B)
{
    matu i, j;
    if(!QMAT_DIM_ISEQUAL(A, B)){
        return QMAT_ERR_DIM;
    }
    for(i = 0; i < A->row; i++){
        for(j = 0; j < A->col; j++){
            if(QMAT_ELEM(A, i, j) != QMAT_ELEM(B, i, j)){
                return 0;
            }
        }
    }
    return 1;
}

mats qmat_copy(QMat *src, QMat *dst)
{
    matu i, j;
    if(!QMAT_DIM_ISEQUAL(src, dst)){
        return QMAT_ERR_DIM;
    }
    for(i = 0; i < src->row; i++){
        for(j = 0; j < src->col; j++){
            QMAT_ELEM(dst, i, j) = QMAT_ELEM(src, i, j);
        }
    }
}

matf qmat_elem(QMat *mat, matu row, matu col) 
{
    if((mat->row >= row) && (mat->col >= col)){
        return QMAT_ELEM(mat, row, col);
    }else{
        return QMAT_NAN;
    }
}

mats qmat_mul(QMat *A, QMat *B, QMat *result) 
{
    matu i, j, m;
    matf tmp;
    if(QMAT_DIM_ISMATCH(A, B) && (A->row == result->row) && (B->col == result->col)){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->row; j++){
                tmp = 0;
                for(m = 0; m < A->col; m++){
                    tmp += QMAT_ELEM(A, i, m) * QMAT_ELEM(B, m, j);
                }
                QMAT_ELEM(result, i, j) = tmp;
            }
        }
        return QMAT_ERR_NONE;
    }else{
        return QMAT_ERR_DIM;
    }
}

mats qmat_muln(QMat *A, matf b, QMat *result)
{
    matu i, j;

    if(QMAT_DIM_ISEQUAL(A, result)){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->col; j++){
                QMAT_ELEM(result, i, j) = QMAT_ELEM(A, i, j) * b;
            }
        }
        return QMAT_ERR_NONE;
    }else{
        return QMAT_ERR_DIM;
    }
}

mats qmat_dotmul(QMat *A, QMat *B, QMat *result) 
{
    matu i, j;
    if(QMAT_DIM_ISEQUAL(A, B) && QMAT_DIM_ISEQUAL(A, result)){
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

mats qmat_add(QMat *A, QMat *B, QMat *result) 
{
    matu i, j;
    if(QMAT_DIM_ISEQUAL(A, B) && QMAT_DIM_ISEQUAL(A, result)){
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

mats qmat_addn(QMat *A, matf b, QMat *result)
{
    matu i, j;
    if(!QMAT_DIM_ISEQUAL(A, result)){
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

mats qmat_div(QMat *A, QMat *B, QMat *result) 
{
    return QMAT_ERR_NONE;
}

mats qmat_divn(QMat *A, matf b, QMat *result)
{
    matu i, j;
    if(!QMAT_DIM_ISEQUAL(A, result)){
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

mats qmat_dotdiv(QMat *A, QMat *B, QMat *result)
{
    matu i, j;
    if(QMAT_DIM_ISEQUAL(A, B) && QMAT_DIM_ISEQUAL(A, result)){
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

mats qmat_sub(QMat *A, QMat *B, QMat *result) 
{
    matu i, j;
    if(QMAT_DIM_ISEQUAL(A, B) && QMAT_DIM_ISEQUAL(A, result)){
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

mats qmat_subn(QMat *A, matf b, QMat *result) 
{
    matu i, j;
    if(!QMAT_DIM_ISEQUAL(A, result)){
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

mats qmat_lu(QMat *A, QMat *L, QMat *U)
{
    matu i, j;
    if(A->row != L->row || A->col != L->col){
        return QMAT_ERR_DIM;
    }else if(A->row != U->row || A->col != U->col){
        return QMAT_ERR_DIM;
    }else{
        qmat_zeros(L);
        qmat_zeros(U);
    }
    qmat_copy(A, U);

    return QMAT_ERR_NONE;
}

mats qmat_inv(QMat *A, QMat *inv)
{

    return QMAT_ERR_NONE;
}
