/*
 * @Author: luoqi 
 * @Date: 2021-08-31 14:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-01-26 16:42:58
 */

#include <string.h>
#include "demo_qsh.h"
#include "../console/qsh.h"
#include "../kernel/timeslice.h"

static QSH_CMD_CREAT(qcmd1);
static QSH_CMD_CREAT(qcmd2);
static QSH_CMD_CREAT(cmd_ls);

unsigned char q1_handle(int argc, char* argv[])
{
    if(argc > 1)
    {
        QSH_PRINTF(" #! parameter error !\r\n");
        return 1;
    }
    printf(">> qsh test 1\r\n");
    return 0;
}

unsigned char q2_handle(int argc, char* argv[])
{
    if(argc > 1)
    {
        QSH_PRINTF(" #! parameter error !\r\n");
        return 1;
    }
    printf(">> qsh test 2\r\n");
    return 0;
}

unsigned char cmd_ls_hdl(int argc, char *argv[])
{
    if(argc <= 1)
    {
        QSH_PRINTF(" #! parameter error !\r\n");
        return 0;
    }
    else
    {
        if(strcmp(argv[1], "task") == 0)
        {
            TimesilceTaskObj *task;
            unsigned int i;
            unsigned int num = timeslice_task_num_get();
            QSH_PRINTF(" TASK NUM %u\r\n", num);
            QSH_PRINTF(" [Task name]                [Task ID]      [Run time]         [Timeslice]            [Usage]\r\n");
            for(i = num; i > 0; i--)
            {
                task = timeslice_obj_get(i);
                QSH_PRINTF("  %-20s       %-6u         %-6lld             %-6u                 %s\r\n",
                            task->name, task->id, task->run_time, task->timeslice_len, task->usage);
            }
        }
        else if(strcmp(argv[1], "dtask") == 0)
        {
            TimesilceTaskObj *task;
            unsigned int i;
            unsigned int num = timeslice_del_task_num_get();
            QSH_PRINTF(" TASK NUM %u\r\n", num);
            QSH_PRINTF(" [Task name]                [Task ID]      [Run time]         [Timeslice]            [Usage]\r\n");
            for(i = num; i > 0; i--)
            {
                task = timeslice_del_obj_get(i);
                QSH_PRINTF("  %-20s       %-6u         %-6lld             %-6u                 %s\r\n",
                            task->name, task->id, task->run_time, task->timeslice_len, task->usage);
            }
        }
        else
            QSH_PRINTF(" #! parameter error !\r\n");
    }
    return 0;
}

int demo_qsh_init()
{
    qsh_cmd_init(&qcmd1, "qcmd1", 0xff, q1_handle, "qsh test 1");
    qsh_cmd_add(&qcmd1);
    
    qsh_cmd_init(&qcmd2, "qcmd2", 0xff, q2_handle, "qsh test 2");
    qsh_cmd_add(&qcmd2);

    qsh_cmd_init(&cmd_ls, "ls", 0xff, cmd_ls_hdl, "list items <task / dtask>");
    qsh_cmd_add(&cmd_ls);
    return 0;
}
