/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:09:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-08-31 18:19:38
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "unitest_def.h"
#include "console/qsh.h"
#include "kernel/fsm.h"
#include "kernel/timeslice.h"
#include "demo/demo_qsh.h"
#include "demo/demo_fsm.h"
#include "demo/demo_timeslice.h"

static pthread_t tid_qsh_isr;
static void* thread_qsh_input_isr(void*);

static pthread_t tid_tasks;
static void* thread_tasks(void*);

int main()
{
    pthread_create(&tid_qsh_isr, NULL, thread_qsh_input_isr, NULL);
    pthread_create(&tid_tasks, NULL, thread_tasks, NULL);

    demo_fsm_init();
    demo_timeslice_init();
    demo_qsh_init();
    qsh_task_init();

    for(;;)
    {
        qsh_task_exec();
        timeslice_exec();
    }

    return 0;
}

void* thread_qsh_input_isr(void* param)
{
    char cmd[60];
    for(;;)
    {
        if(fgets(cmd, 60, stdin)) {
            char *find_LF = strchr(cmd, '\n');
            if (find_LF) {
                *find_LF = '\0';
            }
            qsh_gets_cmd(cmd);
            memset(cmd, 0, sizeof(cmd));
        }
        usleep(1);
    }
}

void* thread_tasks(void* param)
{
    for(;;)
    {
        timeslice_tick();
        usleep(1000);
    }
}

