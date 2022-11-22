/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:09:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-22 22:56:28
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "linuxbase/mthread.h"
#include "qshell/qsh.h"
#include "frame/qtask.h"
#include "qdemo/qdemo.h"

static MThread task_qsh;
static void* task_qsh_hdl(void*);

static MThread qtasks_tick;
static void* qtasks_tick_hdl(void*);

static int close_all = 0;

int main()
{
    mthread_basic_init();
    mthread_init(&task_qsh, "task_qsh", 50, 1000, task_qsh_hdl, "qsh task");
    mthread_start(&task_qsh);
    mthread_init(&qtasks_tick, "qtasks_tick", 10, 1000, qtasks_tick_hdl, "qtasks_tick");
    mthread_start(&qtasks_tick);

    qsh_init();
    qdemo_init();

    for(;;) {
        if(close_all == 0) {
            qsh_exec();
            qtask_exec();
            usleep(10);
        } else {
            return 0;
        }
    }
    return 0;
}

void* task_qsh_hdl(void* param)
{
    char ch;
    for(;;) {
        mthread_task_begin(&task_qsh);
        system("stty raw -echo");
        ch = getchar();
        ch = (ch == 127) ? 8 : ch;
        if(ch != 3) {
            qsh_recv(ch);
        } else {
            system("stty -raw echo");
            printf("\33[2K");
            printf("\033[H\033[J");
            printf(" \r\n#! qsh input thread closed !\r\n\r\n");
            close_all = 1;
            mthread_stop(&task_qsh);
        }
        mthread_task_end(&task_qsh);
    }
}

void* qtasks_tick_hdl(void* param)
{
    for(;;) {
        mthread_task_begin(&qtasks_tick);

        qtask_tick();
        
        mthread_task_end(&qtasks_tick);
    }
}

