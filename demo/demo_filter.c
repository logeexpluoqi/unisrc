/*
 * @Author: luoqi 
 * @Date: 2022-03-15 10:35:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:21:14
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "demo_filter.h"
#include "../console/qsh.h"
#include "../filter/sliding_average_filter.h"
#include "../filter/lpf_first_order.h"
#include "../filter/kalman_filter.h"
#include "../filter/kalman_filter.h"

#define NDATA_SIZE    10000

SlidAveFilterObj sldave_filter;
static void demo_filter_sliding_average(void);

LpfFirstOderObj lpf_1st;
static void demo_lpf_1st(void);

static QshCmd cmd_filter;
static int cmd_filter_hdl(int argc, char *argv[]);

static KF1DimObj kf_1dim;
static void demo_kf_1dim(void);

static float ndata[NDATA_SIZE]; 
static void nsig_show(void);

void demo_filter_init()
{
    for(int i = 0; i < NDATA_SIZE; i++) {
        ndata[i] = sinf(0.001 * i) + (rand() % 10000) / 10000.0 - 0.5 + cos(0.005 * i);
    }

    sliding_average_filter_init(&sldave_filter, 100);
    lpf_first_order_init(&lpf_1st, 0.01);
    kf_1dim_init(&kf_1dim, 0, 1, 0, 1, 0, 0.002, 0.5);

    qsh_cmd_init(&cmd_filter, "filter", cmd_filter_hdl, "@ nsig, sldave, lpf1st, kf1dim");
    qsh_cmd_add(&cmd_filter);
}

int cmd_filter_hdl(int argc, char *argv[])
{
    if(argc <= 1) {
        QSH(" #! parameter error !\r\n");
        return -1;
    }
    
    if(QSH_ISARG(argv[1], "nsig")) {
        nsig_show();
    } else if(QSH_ISARG(argv[1], "sldave")) {
        demo_filter_sliding_average();
    } else if(QSH_ISARG(argv[1], "lpf1st")) {
        demo_lpf_1st();
    } else if(QSH_ISARG(argv[1], "kf1dim")) {
        demo_kf_1dim();
    } else {
        QSH(" #! parameter error !\r\n");
    }

    return 0;
}

void nsig_show()
{
    for(int i = 0; i < NDATA_SIZE; i++) {
        if(i % 9) {
            QSH("\r\n");
        }
        QSH(" % 7.6f", ndata[i]);
    }
    QSH("\r\n");
}

void demo_filter_sliding_average()
{
    float fdata[NDATA_SIZE] = {0};
    for(int i = 0; i < NDATA_SIZE - 100; i++) {
        fdata[i] = sliding_average_filter_calcu(&sldave_filter, ndata[i]);
        if(i % 9) {
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
        fdata[i] = lpf_first_order_calcu(&lpf_1st, ndata[i]);
        if(i % 9) {
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
        if(i % 9) {
            QSH("\r\n");
        }
        QSH(" % 7.6f", fdata[i]);
    }
    QSH("\r\n");
}
