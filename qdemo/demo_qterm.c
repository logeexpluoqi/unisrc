/*
 * @Author: luoqi 
 * @Date: 2021-08-31 14:45:26 
 * @ Modified by: luoqiuoqi
 * @ Modified time: 2024-08-06 21:093:45:25
 */

#include <string.h>
#include "qdemo.h"
#include "../qshell/qterm.h"
#include "../qlib/qtask.h"

static QCliCmd   cmd_ls;
static int cmd_ls_hdl(int, char **);

int demo_qsh_init()
{
    qterm_attach(&cmd_ls, "ls", cmd_ls_hdl, "/-d");
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
