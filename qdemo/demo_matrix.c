/*
 * @Author: luoqi 
 * @Date: 2023-05-11 14:28:42 
 * @Last Modified by: luoqi
 * @Last Modified time: 2023-05-12 16:23:19
 */

#include <stdio.h>
#include <stdlib.h>
#include "demo_matrix.h"
#include "../qmath/qmatrix.h"
#include "../qshell/qsh.h"

static int cmd_mat_hdl(int argc, char **argv);

static mat_f m5x3[5*3] = {
    1, 2, 3,
    2, 5, 1,
    3, 1, 7,
    6, 3, 2,
    1, 2, 5
};
static mat_f m3x5[3*5] = {
    17, 24, 1, 8, 15,
    23, 5, 7, 14, 16,
    4, 6, 13, 20, 22,
};

static mat_f m5x5_a[5*5] = {
    1, 2, 3, 4, 5,
    2, 5, 1, 3, 7,
    3, 1, 7, 4, 2,
    6, 3, 2, 2, 0,
    1, 2, 5, 4, 3
};
static mat_f m5x5_b[5*5] = {
    17, 24, 1, 8, 15,
    23, 5, 7, 14, 16,
    4, 6, 13, 20, 22,
    10, 12, 19, 21, 3,
    11, 18, 25, 2, 9
};

static mat_f m5x5_c[5*5] = {0};
static mat_f m3x3[3*3] = {0};

static QMat M5x3, M3x5, M5x5_A, M5x5_B, M5x5_C, M3x3;

int demo_matrix_init()
{
    qmat_init(&M5x3, m5x3, 5, 3);
    qmat_init(&M3x5, m3x5, 3, 5);
    qmat_init(&M5x5_A, m5x5_a, 5, 5);
    qmat_init(&M5x5_B, m5x5_b, 5, 5);
    qmat_init(&M5x5_C, m5x5_c, 5, 5);
    qmat_init(&M3x3, m3x3, 3, 3);
    qcmd_export("mat", cmd_mat_hdl, "@ add, sub, mul, div, show, gen");
    return 0;
}

static void mat_show(QMat *mat)
{
    for(mat_u i = 0; i < mat->row; i++){
        for(mat_u j = 0; j < mat->col; j++){
            printf(" %f", qmat_elem(mat, i, j));
        }
        printf("\r\n");
    }
    printf("\r\n");
}

static void mat_gen()
{
    QMat *A = qmat_create(3, 5);
    QMat *B = qmat_create(3, 5);
    QSH("-Matrix A\r\n");
    mat_show(A);
    QSH("-Matrix eyes:\r\n");
    qmat_eyes(A);
    mat_show(A);
    QSH("-Matrix addn 2:\r\n");
    qmat_addn(A, 2, B);
    mat_show(B);
    QSH("-Matrix subn 5:\r\n");
    qmat_subn(A, 5, B);
    mat_show(B);
    QSH("-Matrix muln 2:\r\n");
    qmat_muln(A, 2, B);
    mat_show(B);
    QSH("-Matrix divn 3:\r\n");
    qmat_divn(A, 3, B);
    mat_show(B);
    QSH("-Matrix ones:\r\n");
    qmat_ones(A);
    mat_show(A);
    QSH("-Matrix zeros:\r\n");
    qmat_zeros(A);
    mat_show(A);
    qmat_delete(A);
}

int cmd_mat_hdl(int argc, char **argv)
{
    if(ISARG(argv[1], "add")){
        qmat_add(&M5x5_A, &M5x5_B, &M5x5_C);
        mat_show(&M5x5_C);
    }else if(ISARG(argv[1], "sub")){
        qmat_sub(&M5x5_A, &M5x5_B, &M5x5_C);
        mat_show(&M5x5_C);
    }else if(ISARG(argv[1], "mul")){
        QSH("-dotmul:\r\n");
        qmat_dotmul(&M5x5_A, &M5x5_B, &M5x5_C);
        mat_show(&M5x5_C);
        QSH("-mul 5x3 * 3x5:\r\n");
        qmat_mul(&M5x3, &M3x5, &M5x5_C);
        mat_show(&M5x5_C);
        QSH("-mul 3x5 * 5x3:\r\n");
        qmat_mul(&M3x5, &M5x3, &M3x3);
        mat_show(&M3x3);
    }else if(ISARG(argv[1], "div")){
        QSH("-dotdiv:\r\n");
        qmat_dotdiv(&M5x5_A, &M5x5_B, &M5x5_C);
        mat_show(&M5x5_C);
    }else if(ISARG(argv[1], "inv")){

    }else if(ISARG(argv[1], "show")){
        QSH("-M5x5_A:\r\n");
        mat_show(&M5x5_A);
        QSH("-M5x5_B:\r\n");
        mat_show(&M5x5_B);
        QSH("-M5x5_C:\r\n");
        mat_show(&M5x5_C);
        QSH("-M5x3:\r\n");
        mat_show(&M5x3);
        QSH("-M3x5:\r\n");
        mat_show(&M3x5);
        QSH("-M3x3:\r\n");
        mat_show(&M3x3);
    }else if(ISARG(argv[1], "gen")){
        mat_gen();
    }else{
        return CMD_PARAM_ERR;
    }

    return CMD_NO_ERR;
}