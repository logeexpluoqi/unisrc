/*
 * @Author: luoqi
 * @Date: 2021-09-23 14:49:05
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 15:39:31
 */


#include "qmatrix.h"

int qmat_init(QMat *mat, float **elem, int row, int col) 
{
    mat->row = row;
    mat->col = col;
    mat->elem = elem;
    mat->err = QMAT_ERR_NONE;
    return QMAT_ERR_NONE;
}

int qmat_zeros(QMat *mat)
{
    int i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            mat->elem[i][j] = 0.0;
        }
    }
    mat->err = QMAT_ERR_NONE;
    return QMAT_ERR_NONE;
}

int qmat_ones(QMat *mat)
{
    int i, j;
    for(i = 0; i < mat->row; i++){
        for(j = 0; j < mat->col; j++){
            mat->elem[i][j] = 1.0;
        }
    }
    mat->err = QMAT_ERR_NONE;
    return QMAT_ERR_NONE;
}

int qmat_eyes(QMat *mat)
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
    mat->err = QMAT_ERR_NONE;
    return QMAT_ERR_NONE;
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
    int i, j, m;
    float tmp = 0.0;
    if((A->row == B->col) && (A->row == result->row) && (A->col == B->row) && (B->col == result->col)){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->col; j++){
                for(m = 0; m < A->row; m++){
                    tmp += A->elem[i][m] * B->elem[m][i];
                }
                result->elem[i][j] = tmp;
                tmp = 0.0;
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }else{
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }
}

int qmat_muln(QMat *A, float b, QMat *result)
{
    int i, j;

    if((A->row != result->row) || (A->col != result->col)){
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }

    for(i = 0; i < A->row; i++){
        for(j = 0; j < A->col; j++){
            result->elem[i][j] = A->elem[i][j] * b;
        }
    }
    result->err = QMAT_ERR_NONE;
    return QMAT_ERR_NONE;
}

int qmat_dotmul(QMat *A, QMat *B, QMat *result) 
{
    int i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i < A->row; i++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] * B->elem[i][j];
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }else{
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }
}

int qmat_add(QMat *A, QMat *B, QMat *result) 
{
    int i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] + B->elem[i][j];
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }else{
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }
}

int qmat_addn(QMat *A, float b, QMat *result)
{
    int i, j;
    if((A->row != result->row) || (A->col != result->col)){
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] + b;
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }
}

int qmat_div(QMat *A, QMat *B, QMat *result) 
{
    return QMAT_ERR_NONE;
}

int qmat_divn(QMat *A, float b, QMat *result)
{
    int i, j;
    if((A->row != result->row) || (A->col != result->col)){
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }else if(b == 0.0){
        result->err = QMAT_ERR_CALC;
        return QMAT_ERR_CALC;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] - b;
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }
}

int qmat_dotdiv(QMat *A, QMat *B, QMat *result)
{
    int i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                if(B->elem[i][j] != 0){
                    result->elem[i][j] = A->elem[i][j] / B->elem[i][j];
                }else{
                    result->err = QMAT_ERR_CALC;
                    return QMAT_ERR_CALC;
                }
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }else{
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }
    return QMAT_ERR_NONE;
}

int qmat_sub(QMat *A, QMat *B, QMat *result) 
{
    int i, j;
    if((A->row == B->row) && (A->col == B->col) && (A->row == result->row) && (A->col == result->col)){
        for(i = 0; i< A->row; i++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] - B->elem[i][j];
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }else{
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }
    return QMAT_ERR_NONE;
}

int qmat_subn(QMat *A, float b, QMat *result) 
{
    int i, j;
    if((A->row != result->row) || (A->col != result->col)){
        result->err = QMAT_ERR_DIM;
        return QMAT_ERR_DIM;
    }else{
        for(i = 0; i < A->row; i ++){
            for(j = 0; j < A->col; j++){
                result->elem[i][j] = A->elem[i][j] - b;
            }
        }
        result->err = QMAT_ERR_NONE;
        return QMAT_ERR_NONE;
    }
}
