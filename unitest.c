/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:09:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-09-16 19:10:52
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "unitest_def.h"
#include "console/qsh.h"
#include "kernel/fsm.h"
#include "kernel/timeslice.h"
#include "demo/demo_qsh.h"
#include "demo/demo_fsm.h"
#include "demo/demo_timeslice.h"
#include "demo/demo_msg.h"

static pthread_t tid_qsh_isr;
static void* thread_qsh_input_isr(void*);

static pthread_t tid_tasks;
static void* thread_tasks(void*);

static int close_all = 0;

int main()
{
    pthread_create(&tid_qsh_isr, NULL, thread_qsh_input_isr, NULL);
    pthread_create(&tid_tasks, NULL, thread_tasks, NULL);

    qsh_init();

    demo_fsm_init();
    demo_timeslice_init();
    demo_msg_init();
    demo_qsh_init();

    for(;;)
    {
        if(close_all == 0)
        {
            qsh_task_exec();
            timeslice_exec();
        }
        else
            return 0;
    }

    return 0;
}

void* thread_qsh_input_isr(void* param)
{
    char ch;
    for(;;)
    {
        system("stty raw -echo");
        ch = getchar();
        if(ch == 127)
            ch = 8;
        if(ch != 3)
            qsh_get_cmd(ch);
        else
        {
            system("stty -raw echo");
            printf("\33[2K");
            printf(" \r\n>> qsh input thread closed !\r\n\r\n");
            close_all = 1;
            pthread_cancel(tid_tasks);
            pthread_cancel(tid_qsh_isr);
        }

        usleep(1e4);
    }
}

void* thread_tasks(void* param)
{
    for(;;)
    {
        timeslice_tick();
        usleep(1e3);
    }
}

