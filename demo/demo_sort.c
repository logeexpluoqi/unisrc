/*
 * @Author: luoqi 
 * @Date: 2022-04-23 19:08:48 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-30 23:26:37
 */

#include <stdlib.h>
#include "demo_sort.h"
#include "../sort/quick_sort.h"
#include "../console/qsh.h"

#define DEMO_SORT_DATA_SIZE     50

static QshCmd cmd_sort;
static int cmd_sort_hdl(int, char **);
static float quicksortdata[DEMO_SORT_DATA_SIZE];

static void demo_quick_sort(void);

int demo_sort_init()
{
    for(int i = 0; i < DEMO_SORT_DATA_SIZE; i++) {
        quicksortdata[i] = (float)rand() / RAND_MAX;
    }

    qsh_cmd_init(&cmd_sort, "sort", cmd_sort_hdl, "@ quick");
    qsh_cmd_add(&cmd_sort);

    return 0;
}

int cmd_sort_hdl(int argc, char **argv)
{
    if(argc <= 1) {
        QSH(QSH_MSG_PARAM_ERR);
        return -1;
    }

    if(QSH_ISARG(argv[1], "quick")){
        demo_quick_sort();
    } else {
        QSH(QSH_MSG_PARAM_ERR);
    }
    return 0;
}

void demo_quick_sort()
{
    QSH("org data: \r\n");
    for(int i = 0; i < DEMO_SORT_DATA_SIZE; i ++) {
        QSH(" %-6.6f,", quicksortdata[i]);
    }
    QSH("\r\n");

    quick_sort_recu(quicksortdata, DEMO_SORT_DATA_SIZE);
    QSH("sort data: \r\n");
    for(int i = 0; i < DEMO_SORT_DATA_SIZE; i ++) {
        QSH(" %-6.6f,", quicksortdata[i]);
    }
    QSH("\r\n");
}
