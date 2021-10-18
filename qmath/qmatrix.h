/*
 * @Author: luoqi 
 * @Date: 2021-09-23 14:48:27 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-09-23 14:49:03
 */

#ifndef _QMATRIX_H
#define _QMATRIX_H

typedef enum _qmat_calcu_state
{
    QMAT_CALCU_OK,
    QMAT_CALCU_DIM_ERR,

} QMatCalcuState;

typedef struct _qmatrix_obj
{
    unsigned int row;
    unsigned int col;
    
    float *elem[];
} QMatObj;

void qmat_init(QMatObj* mat, float* elem[], unsigned int row, unsigned int col);

float qmat_get_elem(QMatObj* mat, unsigned int row, unsigned int col);

QMatCalcuState qmat_multiply(QMatObj* mat_a, QMatObj* mat_b, QMatObj* mat_result);

QMatCalcuState qmat_dotmultiply(QMatObj* mat_a, QMatObj* mat_b, QMatObj* mat_result);

QMatCalcuState qmat_add(QMatObj* mat_a, QMatObj* mat_b, QMatObj* mat_result);

QMatCalcuState qmat_add(QMatObj* mat_a, QMatObj* mat_b, QMatObj* mat_result);

#endif
