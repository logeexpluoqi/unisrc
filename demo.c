/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:09:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-25 00:30:42
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "linuxbase/mthread.h"
#include "qshell/qterm.h"
#include "qlib/qtask.h"
#include "qdemo/qdemo.h"

static MThread task_qterm;

static void *task_qsh_hdl(void *);

static MThread qtasks_tick;

static void *qtasks_tick_hdl(void *);

static int close_all = 0;

int main()
{
    qterm_init();
    mthread_init(&task_qterm, "task_qterm", 50, 1000, task_qsh_hdl, "qterm task");
    mthread_start(&task_qterm);
    mthread_init(&qtasks_tick, "qtasks_tick", 10, 1000, qtasks_tick_hdl, "qtasks_tick");
    mthread_start(&qtasks_tick);

    qdemo_init();

    for (;;) {
        if (close_all == 0) {
            qtask_exec();
            usleep(10);
        } else {
            return 0;
        }
    }
    return 0;
}

void *task_qsh_hdl(void *param)
{
    char ch;
    for (;;) {
        mthread_task_begin(&task_qterm);
        if(system("stty raw -echo") < 0){
            printf(" #! system call error !\r\n");
        }
        ch = getchar();
        ch = (ch == 127) ? 8 : ch;
        if (ch != 3) {
            qterm_exec(ch);
        } else {
            if(system("stty -raw echo") < 0){
                printf(" #! system call error !\r\n");
            }
            printf("\33[2K");
            printf("\033[H\033[J");
            printf(" \r\n#! qsh input thread closed !\r\n\r\n");
            close_all = 1;
            mthread_stop(&task_qterm);
        }
        mthread_task_end(&task_qterm);
    }
}

void *qtasks_tick_hdl(void *param)
{
    for (;;) {
        mthread_task_begin(&qtasks_tick);

        qtask_tick();

        mthread_task_end(&qtasks_tick);
    }
}

