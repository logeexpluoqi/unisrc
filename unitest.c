/*
 * @Author: luoqi 
 * @Date: 2021-08-31 15:09:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-08-31 15:56:43
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "unitest_def.h"
#include "demo/demo_qsh.h"
#include "console/qsh.h"

static pthread_t tid_isr;
static void* thread_isr(void*);

int main()
{
    pthread_create(&tid_isr, NULL, thread_isr, NULL);

    qsh_task_init();    

    for(;;)
    {
        
    }

    return 0;
}

void* thread_isr(void* param)
{
    for(;;)
    {
        qsh_task_exec();
        sleep(1);
    }
}