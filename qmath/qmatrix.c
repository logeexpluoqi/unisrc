/*
 * @Author: luoqi
 * @Date: 2021-09-23 14:49:05
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 15:39:31
 */


#include "qmatrix.h"
#ifdef QMAT_USING_LIBC
#include <stdlib.h>
#endif

void qmat_init(QMat *mat, float **elem, int row, int col) 
{
    mat->row = row;
    mat->col = col;
    mat->elem = elem;
    mat->err = QMAT_ERR_NONE;
}

#ifdef QMAT_USING_LIBC
QMat qmatc(int row, int col)
{
    QMat mat;
    float **elem = malloc(sizeof(float) * row * col);
    qmat_init(&mat, elem, row, col);
    return mat;
}

QMat qmatc_zeros(int row, int col)
{
    QMat mat;
    int i, j;
    float **elem = malloc(sizeof(float) * row * col);
    qmat_init(&mat, elem, row, col);
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            mat.elem[i][j] = 0.0;
        }
    }
    return mat;
}

QMat qmatc_ones(int row, int col)
{
    QMat mat;
    int i, j;
    float **elem = malloc(sizeof(float) * row * col);
    qmat_init(&mat, elem, row, col);
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            if(i == j){
                mat.elem[i][j] = 1.0;
            }else{
                mat.elem[i][j] = 0.0;
            }
        }
    }
    return mat;
}

QMat qmatc_eyes(int row, int col)
{
    QMat mat;
    int i, j;
    float **elem = malloc(sizeof(float) * row * col);
    qmat_init(&mat, elem, row, col);
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            if(i == j){
                mat.elem[i][j] = 1.0;
            }else{
                mat.elem[i][j] = 0.0;
            }
        }
    }
    return mat;
}
#endif

void qmat_zeros(QMat *mat)
{
    int i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            mat->elem[i][j] = 0.0;
        }
    }
    mat->err = QMAT_ERR_NONE;
}

void qmat_ones(QMat *mat)
{
    int i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            mat->elem[i][j] = 1.0;
        }
    }
    mat->err = QMAT_ERR_NONE;
}

void qmat_eyes(QMat *mat)
{
    int i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            if(i == j){
                mat->elem[i][j] = 1.0;
            }else{
                mat->elem[i][j] = 0.0;
            }
        }
    }
    mat->err  =QMAT_ERR_NONE;
}

float qmat_elem(QMat *mat, int row, int col) 
{
    if(mat->row >= row && mat->col >= col){
        mat->err = QMAT_ERR_NONE;
        return mat->elem[row][col];
    }else{
        mat->err = QMAT_ERR_DIM;
    }
}


int qmat_mul(QMat *A, QMat *B, QMat *result) 
{
    int i, j;

    if(A->row == B->col && A->row == result->row && A->col == B->row && B->col == result->col){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] * B->elem[j][i];
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }else{
        result->err = QMAT_ERR_DIM;
        return -1;
    }
}

int qmat_muln(QMat *A, float b, QMat *result)
{
    int i, j;

    if(A->row != result->row || A->col != result->col){
        result->err = QMAT_ERR_DIM;
        return -1;
    }

    for(i = 0; i < A->row; i++){
        for(j = 0; j < A->col; j++){
            result->elem[i][j] = A->elem[i][j] * b;
        }
    }
    result->err = QMAT_ERR_NONE;
    return 0;
}

int qmat_dotmul(QMat *A, QMat *B, QMat *result) 
{
    int i, j;
    if(A->row == B->row && A->col == B->col && A->row == result->row && A->col == result->col){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] * B->elem[i][j];
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }else{
        result->err = QMAT_ERR_DIM;
        return -1;
    }
}

int qmat_add(QMat *A, QMat *B, QMat *result) 
{
    int i, j;
    if(A->row == B->row && A->col == B->col && A->row == result->row && A->col == result->col){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] + B->elem[i][j];
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }else{
        result->err = QMAT_ERR_DIM;
        return -1;
    }
}

int qmat_addn(QMat *A, float b, QMat *result)
{
    int i, j;
    if(A->row != result->row || A->col != result->col){
        result->err = QMAT_ERR_DIM;
        return -1;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] + b;
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }
}

int qmat_div(QMat *A, QMat *B, QMat *result) 
{
    return 0;
}

int qmat_divn(QMat *A, float b, QMat *result)
{
    int i, j;
    if(A->row != result->row || A->col != result->col){
        result->err = QMAT_ERR_DIM;
        return -1;
    }else if(b == 0.0){
        result->err = QMAT_ERR_CALC;
        return -1;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] - b;
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }
}

int qmat_dotdiv(QMat *A, QMat *B, QMat *result)
{
    int i, j;
    if(A->row == B->row && A->col == B->col && A->row == result->row && A->col == result->col){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                if(B->elem[i][j] != 0){
                    result->elem[i][j] = A->elem[i][j] / B->elem[i][j];
                }else{
                    result->err = QMAT_ERR_CALC;
                    return -1;
                }
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }else{
        result->err = QMAT_ERR_DIM;
        return -1;
    }
    return 0;
}

int qmat_sub(QMat *A, QMat *B, QMat *result) 
{
    int i, j;
    if(A->row == B->row && A->col == B->col && A->row == result->row && A->col == result->col){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] - B->elem[i][j];
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }else{
        result->err = QMAT_ERR_DIM;
        return -1;
    }
    return 0;
}

int qmat_subn(QMat *A, float b, QMat *result) 
{
    int i, j;
    if(A->row != result->row || A->col != result->col){
        result->err = QMAT_ERR_DIM;
        return -1;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] - b;
            }
        }
        result->err = QMAT_ERR_NONE;
        return 0;
    }
}
