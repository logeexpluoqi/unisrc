/*
 * @Author: luoqi 
 * @Date: 2023-05-11 14:28:42 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-12 11:47:02
 */

#include <stdio.h>
#include <stdlib.h>
#include "demo_matrix.h"
#include "../qmath/qmatrix.h"
#include "../qshell/qsh.h"

static int cmd_mat_hdl(int argc, char **argv);

static mat_f a[5][5] = {0};
static mat_f b[5][5] = {0};

static QMat A, B;

int demo_matrix_init()
{
    qmat_init(&A, a, 5, 5);
    qmat_init(&B, b, 5, 5);
    qcmd_export("mat", cmd_mat_hdl, "@ add, sub, mul, div, zeros, ones, eyes");
    return 0;
}

static int mat_show(QMat *mat)
{
    for(mat_u i = 0; i < mat->row; i++){
        for(mat_u j = 0; j < mat->col; j++){
            printf(" %f", mat->elem[i * mat->col + j]);
        }
        printf("\r\n");
    }
    printf("\r\n");
}

int cmd_mat_hdl(int argc, char **argv)
{
    if(ISARG(argv[1], "ones")){
        qmat_ones(&A);
        mat_show(&A);
    }else if(ISARG(argv[1], "eyes")){
        qmat_eyes(&A);
        mat_show(&A);
    }else if(ISARG(argv[1], "zeros")){
        qmat_zeros(&A);
        mat_show(&A);
    }else {
        return CMD_PARAM_ERR;
    }

    return CMD_NO_ERR;
}
