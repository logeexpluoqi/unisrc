/*
 * @Author: luoqi
 * @Date: 2021-08-31 14:45:26
 * @ Modified by: luoqi
 * @ Modified time: 2024-08-06 21:45
 */

#include <string.h>
#include "qdemo.h"
#include "../qshell/qterm.h"
#include "../qlib/qtask.h"
#include "../qlib/qlist.h"

static QCliCmd   cmd_ls;
static int cmd_ls_hdl(int, char **);

extern QTaskScheduler qtask_scheduler;

int demo_qsh_init()
{
    qterm_attach(&cmd_ls, "ls", cmd_ls_hdl, "/-d");
    return 0;
}

int cmd_ls_hdl(int argc, char **argv)
{
    if(argc <= 1) {
        QList *node;
        QTaskObj *task;
        uint32_t i;
        uint32_t num = qlist_length(&qtask_scheduler.sched_list);
        QSH(" TASK NUM %u\r\n", num);
        QSH(" [Task name]      [Task ID]   [Run time]      [Timeslice]      [Usage]\r\n");
        QSH(" -----------      ---------   ----------      -----------      -------\r\n");

        QLIST_ITERATOR(node, &qtask_scheduler.sched_list)
        {
            task = QLIST_ENTRY(node, QTaskObj, task_node);
            QSH("  %-15s  %-3u          %-6u           %s\r\n",
                task->name, task->id, task->tick, task->usage);
        }
        return 0;
    } else {
        if(ISARG(argv[1], "-d")) {
            QList *node;
            QTaskObj *task;
            uint32_t i;
            uint32_t num = qlist_length(&qtask_scheduler.sched_list);
            QSH(" TASK NUM %u\r\n", num);
            QSH(" [Task name]      [Task ID]   [Run time]      [Timeslice]      [Usage]\r\n");
            QSH(" -----------      ---------   ----------      -----------      -------\r\n");
            QLIST_ITERATOR(node, &qtask_scheduler.unsched_list)
            {
                task = QLIST_ENTRY(node, QTaskObj, task_node);
                QSH("  %-15s  %-3u          %-6u           %s\r\n",
                    task->name, task->id, task->tick, task->usage);
            }
        } else {
            return -1;
        }
    }
    return 0;
}
