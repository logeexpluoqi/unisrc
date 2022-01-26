/*
 * @Author: luoqi
 * @Date: 2021-09-23 14:49:05
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 15:39:31
 */

#include "qmatrix.h"

void qmatrix_init(QMatrix *mat, float *elem[], unsigned int row, unsigned int col) 
{
    // mat->elem = elem;
    mat->row = row;
    mat->col = col;
}

float qmatrix_elem(QMatrix *mat, unsigned int row, unsigned int col) 
{
    if(mat->row >= row && mat->col >= col)
        return mat->elem[row][col];
    else
        return 3.4e38;
}

QMatrix qmatrix_mul(QMatrix *A, QMatrix *B) 
{
    int i, j;
    QMatrix mat;


    if((A->row == B->col) && (A->col == B->row))
    {
        mat.row = A->row;
        mat.col = B->col;
        
        return mat;
    }
    else
    {
        
    }
}

QMatrix qmatrix_dotmul(QMatrix *A, QMatrix *B) 
{
    int i, j;
    QMatrix mat;
    if((A->row == B->row) && (A->col == B->col))
    {
        mat.row = A->row;
        mat.col = B->col;
        for(i = 0; i < A->row; i++)
        {
            for(j = 0; j < A->col; j++)
            {
                mat.elem[i][j] = A->elem[i][j] * B->elem[i][j];
            }
        }
        return mat;
    }
    else
    {
        mat.row = 0;
        mat.col = 0;
        return mat;
    }
}

QMatrix qmatrix_add(QMatrix *A, QMatrix *B) 
{
    QMatrix mat;

    return mat;
}

QMatrix qmatrix_div(QMatrix *A, QMatrix *B) 
{
    QMatrix mat;

    return mat;
}

QMatrix qmatrix_sub(QMatrix *A, QMatrix *B) 
{
    QMatrix mat;

    return mat;
}
