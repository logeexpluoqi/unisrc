/*
 * @Author: luoqi 
 * @Date: 2021-08-31 14:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 19:22:14
 */

#include <string.h>
#include "demo_qsh.h"
#include "../console/qsh.h"
#include "../frame/timeslice.h"

static QshCmd   qcmd1;
static int q1_handle(int, char **);
static QshCmd   qcmd2;
static int q2_handle(int, char **);
static QshCmd   cmd_ls;
static int cmd_ls_hdl(int, char **);

int demo_qsh_init()
{
    qsh_cmd_init(&qcmd1, "qcmd1", q1_handle, "qsh test 1");
    qsh_cmd_add(&qcmd1);
    
    qsh_cmd_init(&qcmd2, "qcmd2", q2_handle, "qsh test 2");
    qsh_cmd_add(&qcmd2);

    qsh_cmd_init(&cmd_ls, "ls", cmd_ls_hdl, "@ task, dtask");
    qsh_cmd_add(&cmd_ls);
    return 0;
}

int q1_handle(int argc, char* argv[])
{
    if(argc > 1) {
        QSH(QSH_MSG_PARAM_ERR);
        return -1;
    }
    QSH(">> qsh test 1\r\n");
    return 0;
}

int q2_handle(int argc, char* argv[])
{
    if(argc > 1) {
        QSH(QSH_MSG_PARAM_ERR);
        return -1;
    }
    QSH(">> qsh test 2\r\n");
    return 0;
}

int cmd_ls_hdl(int argc, char *argv[])
{
    if(argc <= 1) {
        QSH(QSH_MSG_PARAM_ERR);
        return 0;
    } else {
        if(QSH_ISARG(argv[1], "task")) {
            TimesilceTaskObj *task;
            unsigned int i;
            unsigned int num = timeslice_task_num_get();
            QSH(" TASK NUM %u\r\n", num);
            QSH(" [Task name]      [Task ID]   [Run time]      [Timeslice]      [Usage]\r\n");
            QSH(" -----------      ---------   ----------      -----------      -------\r\n");
            for(i = num; i > 0; i--) {
                task = timeslice_obj_get(i);
                QSH("  %-15s  %-3u         %-6lld          %-6u           %s\r\n",
                       task->name, task->id, task->run_time, task->timeslice_len, task->usage);
            }
        } else if(QSH_ISARG(argv[1], "dtask")) {
            TimesilceTaskObj *task;
            unsigned int i;
            unsigned int num = timeslice_del_task_num_get();
            QSH(" TASK NUM %u\r\n", num);
            QSH(" [Task name]      [Task ID]   [Run time]      [Timeslice]      [Usage]\r\n");
            QSH(" -----------      ---------   ----------      -----------      -------\r\n");
            for(i = num; i > 0; i--) {
                task = timeslice_del_obj_get(i);
                QSH("  %-15s  %-3u         %-6lld          %-6u           %s\r\n",
                       task->name, task->id, task->run_time, task->timeslice_len, task->usage);
            }
        }
        else {
            QSH(QSH_MSG_PARAM_ERR);
        }
    }
    return 0;
}
