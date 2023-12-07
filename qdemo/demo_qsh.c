/*
 * @Author: luoqi 
 * @Date: 2021-08-31 14:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-24 23:45:25
 */

#include <string.h>
#include "qdemo.h"
#include "../qshell/qsh.h"
#include "../qlib/qtask.h"

static QCmdObj   cmd_ls;
static int cmd_ls_hdl(int, char **);

int demo_qsh_init()
{
    qcmd_init(&cmd_ls, "ls", cmd_ls_hdl, "/-d");
    qcmd_add(&cmd_ls);
    return 0;
}

int cmd_ls_hdl(int argc, char **argv)
{
    if(argc <= 1) {
        QTaskObj *task;
            uint32_t i;
            uint32_t num = qtask_num();
            QSH(" TASK NUM %u\r\n", num);
            QSH(" [Task name]      [Task ID]   [Run time]      [Timeslice]      [Usage]\r\n");
            QSH(" -----------      ---------   ----------      -----------      -------\r\n");
            for(i = num; i > 0; i--) {
                task = qtask_get(i);
                QSH("  %-15s  %-3u         %-6d          %-6u           %s\r\n",
                       task->name, task->id, task->run_time, task->tick, task->usage);
            }
            return 0;
    } else {
        if(ISARG(argv[1], "-d")) {
            QTaskObj *task;
            uint32_t i;
            uint32_t num = qdtask_num();
            QSH(" TASK NUM %u\r\n", num);
            QSH(" [Task name]      [Task ID]   [Run time]      [Timeslice]      [Usage]\r\n");
            QSH(" -----------      ---------   ----------      -----------      -------\r\n");
            for(i = num; i > 0; i--) {
                task = qdtask_get(i);
                QSH("  %-15s  %-3u         %-6d          %-6u           %s\r\n",
                       task->name, task->id, task->run_time, task->tick, task->usage);
            }
        }
        else {
            return -1;
        }
    }
    return 0;
}
