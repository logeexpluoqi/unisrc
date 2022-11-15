/*
 * @Author: luoqi 
 * @Date: 2022-04-23 19:08:48 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-05-13 22:12:54
 */

#include <stdlib.h>
#include "demo_sort.h"
#include "../sort/quick_sort.h"
#include "../sort/bubble_sort.h"
#include "../qshell/qsh.h"

#define DEMO_SORT_DATA_SIZE     50

static CmdObj cmd_sort;
static int cmd_sort_hdl(int, char **);
static float quicksortdata[DEMO_SORT_DATA_SIZE];

static void demo_quick_sort(void);
static void demo_bubbule_sort(void);

int demo_sort_init()
{
    for(int i = 0; i < DEMO_SORT_DATA_SIZE; i++) {
        quicksortdata[i] = (float)rand() / RAND_MAX;
    }

    qcmd_init(&cmd_sort, "sort", cmd_sort_hdl, "@ quick, bub");
    qcmd_add(&cmd_sort);

    return 0;
}

int cmd_sort_hdl(int argc, char **argv)
{
    if(argc <= 1) {
        return CMD_PARAM_LESS;
    }

    if(ISARG(argv[1], "quick")){
        demo_quick_sort();
    } else if(ISARG(argv[1], "bub")){
        demo_bubbule_sort();
    }else {
        return CMD_PARAM_ERR;
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

void demo_bubbule_sort()
{
    QSH("org data: \r\n");
    for(int i = 0; i < DEMO_SORT_DATA_SIZE; i ++) {
        QSH(" %-6.6f,", quicksortdata[i]);
    }
    QSH("\r\n");

    bubble_sort(quicksortdata, DEMO_SORT_DATA_SIZE);
    QSH("sort data: \r\n");
    for(int i = 0; i < DEMO_SORT_DATA_SIZE; i ++) {
        QSH(" %-6.6f,", quicksortdata[i]);
    }
    QSH("\r\n");
}
