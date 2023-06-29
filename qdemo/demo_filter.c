/*
 * @Author: luoqi 
 * @Date: 2022-03-15 10:35:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:21:14
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "qdemo.h"
#include "../qshell/qsh.h"
#include "../filter/lpf_sa.h"
#include "../filter/lpf_1st.h"
#include "../filter/hpf_1st.h"
#include "../filter/kf.h"

#define NDATA_SIZE    10000

LpfSaObj sldave_filter;
static void demo_filter_sa(void);

Lpf1stObj lpf_1st;
static void demo_lpf_1st(void);

Hpf1stObj hpf_1st;
static void demo_hpf_1st(void);

static CmdObj cmd_filter;
static int cmd_filter_hdl(int argc, char **argv);

static KF1DimObj kf_1dim;
static void demo_kf_1dim(void);

static float ndata[NDATA_SIZE]; 
static void nsig_show(void);

void demo_filter_init()
{
    for(int i = 0; i < NDATA_SIZE; i++) {
        ndata[i] = sinf(0.001 * i) + (rand() % 10000) / 10000.0 - 0.5 + cos(0.005 * i);
    }

    lpf_sa_init(&sldave_filter, 100);
    lpf_1st_init(&lpf_1st, 0.01, 0.01);
    hpf_1st_init(&hpf_1st, 1, 0.01);
    kf_1dim_init(&kf_1dim, 0, 1, 0, 1, 0, 0.002, 0.5);

    qcmd_init(&cmd_filter, "filter", cmd_filter_hdl, "@ nsig, sa, lpf1st, hpf1st, kf1dim");
    qcmd_add(&cmd_filter);
}

int cmd_filter_hdl(int argc, char **argv)
{
    if(argc <= 1) {
        return CMD_PARAM_LESS;
    }
    
    if(ISARG(argv[1], "nsig")) {
        nsig_show();
    } else if(ISARG(argv[1], "sa")) {
        demo_filter_sa();
    } else if(ISARG(argv[1], "lpf1st")) {
        demo_lpf_1st();
    } else if(ISARG(argv[1], "hpf1st")) {
        demo_hpf_1st();
    } else if(ISARG(argv[1], "kf1dim")) {
        demo_kf_1dim();
    }else {
        return CMD_PARAM_ERR;
    }

    return 0;
}

void nsig_show()
{
    for(int i = 0; i < NDATA_SIZE; i++) {
        if(i % 10) {
            QSH("\r\n");
        }
        QSH(" % 7.6f", ndata[i]);
    }
    QSH("\r\n");
}

void demo_filter_sa()
{
    float fdata[NDATA_SIZE] = {0};
    for(int i = 0; i < NDATA_SIZE - 100; i++) {
        fdata[i] = lpf_sa_calcu(&sldave_filter, ndata[i]);
        if((i % 10) == 0) {
            QSH("\r\n");
        }
        QSH(" % 7.6f", fdata[i]);
    }
    QSH("\r\n");
}

void demo_hpf_1st()
{
    float fdata[NDATA_SIZE] = {0};
    for(int i = 0; i < NDATA_SIZE; i++) {
        fdata[i] = hpf_1st_calcu(&hpf_1st, ndata[i]);
        if((i % 10) == 0) {
            QSH("\r\n");
        }
        QSH(" % 7.6f", fdata[i]);
    }
    QSH("\r\n");
}

void demo_lpf_1st()
{
    float fdata[NDATA_SIZE] = {0};
    for(int i = 0; i < NDATA_SIZE; i++) {
        fdata[i] = lpf_1st_calcu(&lpf_1st, ndata[i]);
        if((i % 10) == 0) {
            QSH("\r\n");
        }
        QSH(" % 7.6f", fdata[i]);
    }
    QSH("\r\n");
}

void demo_kf_1dim()
{
    float fdata[NDATA_SIZE] = {0};
    for(int i = 0; i < NDATA_SIZE; i++) {
        fdata[i] = kf_1dim_calcu(&kf_1dim, ndata[i]);
        if((i % 9) == 0) {
            QSH("\r\n");
        }
        QSH(" % 7.6f", fdata[i]);
    }
    QSH("\r\n");
}
