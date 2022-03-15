/*
 * @Author: luoqi 
 * @Date: 2022-03-15 10:35:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-03-15 15:45:48
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "demo_filter.h"
#include "../console/qsh.h"
#include "../filter/sliding_average_filter.h"

#define NDATA_SIZE    10000

SlidAveFilterObj sldave_filter;

static QSH_CMD_CREAT(cmd_filter);
static unsigned char cmd_filter_hdl(int argc, char *argv[]);

static float ndata[NDATA_SIZE]; 

static void nsig_show(void);
void filter_sliding_average(void);

void demo_filter_init()
{
    for(int i = 0; i < NDATA_SIZE; i++)
    {
        ndata[i] = sinf(0.001 * i) + (rand() % 10000) / 10000.0 - 0.5;
    }

    sliding_average_filter_init(&sldave_filter, 100);
    qsh_cmd_init(&cmd_filter, "filter", 0xff, cmd_filter_hdl, "filter <nsig sldave>");
    qsh_cmd_add(&cmd_filter);
}

unsigned char cmd_filter_hdl(int argc, char *argv[])
{
    if(strcmp(argv[1], "nsig") == 0)
        nsig_show();
    else if(strcmp(argv[1], "sldave") == 0)
        filter_sliding_average();
    else
        QSH_PRINTF(" #! parameter error !\r\n");

    return 0;
}

void nsig_show()
{
    for(int i = 0; i < NDATA_SIZE; i++)
    {
        if(i % 9 == 0)
            QSH_PRINTF("\r\n");
        QSH_PRINTF(" % 7.6f", ndata[i]);
    }
    QSH_PRINTF("\r\n");
}

void filter_sliding_average()
{
    float fdata[NDATA_SIZE] = {0};
    for(int i = 0; i < NDATA_SIZE - 100; i++)
    {
        fdata[i] = sliding_average_filter(&sldave_filter, ndata[i]);
        if(i % 9 == 0)
            QSH_PRINTF("\r\n");
        QSH_PRINTF(" % 7.6f", fdata[i]);
    }
    QSH_PRINTF("\r\n");
}
