/*
 * @Author: luoqi 
 * @Date: 2022-09-03 23:31:42 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-09-03 23:36:38
 */

#include "qdemo.h"
#include "../linuxbase/mthread.h"

static MThread thread;
static void *thread_hdl(void *args);

int demo_mthread_init()
{
    mthread_init(&thread, "mthread_demo", 50, 500000, thread_hdl, "mthread demo");
    mthread_start(&thread);
    return 0;
}

void *thread_hdl(void *args)
{
    for(;;){
        mthread_task_begin(&thread);


        mthread_task_end(&thread);
    }
}
