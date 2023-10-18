/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:47:43 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-25 00:27:49
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "qdemo.h"
#include "../qlib/qtask.h"
#include "../qshell/qsh.h"

static QTaskObj task1;
static QTaskObj task2;
static QTaskObj task3;

void task1_hdl(void);
void task2_hdl(void);
void task3_hdl(void);

static CmdObj cmd_task;
static int cmd_task_hdl(int , char**);

int demo_qtask_init()
{
    qtask_init(&task1, "task1", task1_hdl, 1000, "timeslice task 1");
    qtask_del(&task1);

    qtask_init(&task2, "task2", task2_hdl, 5000, "timeslice task 1");
    qtask_del(&task2);

    qtask_init(&task3, "task3", task3_hdl, 200, "timeslice task 3");
    qtask_del(&task3);

    qcmd_init(&cmd_task, "task", cmd_task_hdl, "run/stop<1/2/3/3>");
    qcmd_add(&cmd_task);

    return 0;
}

void task1_hdl()
{
    QSH("\r\n$ task 1 running \r\n");
}

void task2_hdl()
{
    QSH("\r\n$ task 2 running \r\n");
}

void task3_hdl()
{
    qtask_del(&task3);
    QSH("\r>> task 3 executed\r\n");
}

int cmd_task_hdl(int argc, char **argv)
{
    if(argc <= 1){
        return CMD_PARAM_ERR;
    }
    
    if(ISARG(argv[1], "run")){
        if(ISARG(argv[2], "1")){
            qtask_add(&task1);
        }else if(ISARG(argv[2], "2")){
            qtask_add(&task2);
        }else if(ISARG(argv[2], "all")){
            qtask_add(&task1);
            qtask_add(&task2);
        }
        else if(ISARG(argv[2], "3")){
            qtask_add(&task3);
        }else{
            return CMD_PARAM_ERR;
        }
    }else if(ISARG(argv[1], "stop")){
        if(ISARG(argv[2], "1")){
            qtask_del(&task1);
        }else if(ISARG(argv[2], "2")){
            qtask_del(&task2);
        }else if(ISARG(argv[2], "all")){
            qtask_del(&task1);
            qtask_del(&task2);
        }else{
            return CMD_PARAM_ERR;
        }
    }

    return 0;
}
