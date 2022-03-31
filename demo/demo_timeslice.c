/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:47:43 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:52:02
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "demo_timeslice.h"
#include "../kernel/timeslice.h"
#include "../console/qsh.h"

static TimesilceTaskObj task1;
static TimesilceTaskObj task2;
static TimesilceTaskObj task3;

void task1_hdl(void);
void task2_hdl(void);
void task3_hdl(void);

static QSH_CMD_CREAT(cmd_task);
unsigned char cmd_task_hdl(int argc, char* argv[]);

int demo_timeslice_init()
{
    timeslice_task_init(&task1, "task1", task1_hdl, 1000, "timeslice task 1");
    timeslice_task_del(&task1);

    timeslice_task_init(&task2, "task2", task2_hdl, 5000, "timeslice task 1");
    timeslice_task_del(&task2);

    timeslice_task_init(&task3, "task3", task3_hdl, 200, "timeslice task 3");
    timeslice_task_del(&task3);

    qsh_cmd_init(&cmd_task, "task", 0xff, cmd_task_hdl, "task control command <task run/stop 1/2/all/3>");
    qsh_cmd_add(&cmd_task);

    return 0;
}

void task1_hdl()
{
    printf("\r\n$ task 1 running \r\n");
}

void task2_hdl()
{
    printf("\r\n$ task 2 running \r\n");
}

void task3_hdl()
{
    timeslice_task_del(&task3);
    printf("\r>> task 3 executed\r\n");
}

unsigned char cmd_task_hdl(int argc, char* argv[])
{
    if(argc <= 1)
    {
        QSH_PRINTF(" #! parameter error !\r\n");
        return 1;
    }
    
    if(strcmp(argv[1], "run") == 0)
    {
        if(strcmp(argv[2], "1") == 0)
        {
            timeslice_task_add(&task1);
        }
        else if(strcmp(argv[2], "2") == 0)
        {
            timeslice_task_add(&task2);
        }
        else if(strcmp(argv[2], "all") == 0)
        {
            timeslice_task_add(&task1);
            timeslice_task_add(&task2);
        }
        else if(strcmp(argv[2], "3") == 0)
            timeslice_task_add(&task3);
        else
            printf(" #! parameter error !\r\n");
    }
    else if(strcmp(argv[1], "stop") == 0)
    {
        if(strcmp(argv[2], "1") == 0)
        {
            timeslice_task_del(&task1);
        }
        else if(strcmp(argv[2], "2") == 0)
        {
            timeslice_task_del(&task2);
        }
        else if(strcmp(argv[2], "all") == 0)
        {
            timeslice_task_del(&task1);
            timeslice_task_del(&task2);
        }
        else
            printf(" #! parameter error !\r\n");
    }

    return 0;
}
